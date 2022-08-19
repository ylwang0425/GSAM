//
// Created by wyl on 2022/8/6.
//

#include "Aligner.h"

Aligner::Aligner(AlignmentGraph& g,Args& args,exSAM& sam): graph(g),reads(args.readFiles),exSam(sam),include_cigar(args.include_cigar),thread_num(args.threads),
tolerant_length(args.tolerant_length),penalty_radio(args.penalty_ratio),penalty_coefficient(1.0+(args.penalty_ratio/(1.0-args.penalty_ratio))){

}
void Aligner::MutiAlignRead(){
    while(aq.size_approx()>0){
        pair<int,FastQ> t;
        aq.try_dequeue(t);
        resString[t.first]=alignRead(t.second);
    }
}
void Aligner::alignReads() {
    dpParser = new DpParser(1000);
    if(reads.size()<outFile.size()){
        cerr<<"the number of out file can not larger than the read file";
    }
    for(int i=0;i<reads.size();i++){
        vector<FastQ> fastQs = loadFastqFromFile(reads[i]);
        resString.clear();
        resString.resize(fastQs.size());
        for(int j=0;j<fastQs.size();j++){
            aq.enqueue({j,fastQs[j]});
        }
        boost::thread_group gt;
        for(int j=0;j<thread_num;j++){
            threads.push_back(gt.create_thread(bind(&Aligner::MutiAlignRead,this)));
//            cout<<threads.size()<<endl;
        }
        gt.join_all();
        for(int j=0;j<resString.size();j++){
            cout<<resString[j]<<endl;
        }
    }
}

void Aligner::print_path_res(path_res& path,string& read_name){
    merge_res res = path.mergeRes;
    cout << read_name << "[" << res.string_range.first << "-" << res.string_range.second << "]" << " matched " << res.match_size
         << " char "<< " deleted  "<<res.delete_size<<" char "<< " insert "<<res.insert_size<<" "<<" mismatch "<<res.mismatch_size<<" char at ";
    for (int i = 0; i < res.v.size(); i++) {
        cout << "->" << graph.originalNodeName[res.v[i].node_id]  << "+-"[(res.v[i].node_id & 1)] << ":[" << res.v[i].node_range.first << "-" << res.v[i].node_range.second
            << "]";
    }
    cout<<endl;
}


AlignmentResult Aligner::transPathResToAlignmentResult(path_res& path,string& read_name,string& s,string &cigar){
    AlignmentResult alignmentResult;
    alignmentResult.queryName = read_name;
    alignmentResult.queryLen = s.length();
    alignmentResult.startPos = path.mergeRes.string_range.first;
    alignmentResult.endPos = path.mergeRes.string_range.second;
    alignmentResult.direction = '+';
    stringstream ss;
    alignmentResult.pathLen=0;
    merge_res res = path.mergeRes;
    for(int i=0;i<res.v.size();i++){
        alignmentResult.pathLen+=res.v[i].node_range.second-res.v[i].node_range.first+1;
        ss<<"><"[(res.v[i].node_id & 1)]<<graph.originalNodeName[res.v[i].node_id];
    }
    alignmentResult.mismatches =res.mismatch_size;
    alignmentResult.deletes = res.delete_size;
    alignmentResult.inserts =res.insert_size;
    alignmentResult.path = ss.str();
    alignmentResult.start = res.v[0].node_range.first;
    alignmentResult.end = alignmentResult.start+alignmentResult.pathLen;
    alignmentResult.matches = res.match_size;
    alignmentResult.blockLen = res.match_size+res.delete_size+res.insert_size+res.mismatch_size;
    alignmentResult.mappingQuality = 60;
    alignmentResult.alignmentScore = 1.0*(path.mergeRes.string_range.second-path.mergeRes.string_range.first+1)-penalty_coefficient*(path.mergeRes.mismatch_size+path.mergeRes.insert_size+path.mergeRes.delete_size);
    if(include_cigar){
        alignmentResult.cigarStr = cigar;
    }
    return alignmentResult;
}

string Aligner::alignRead(FastQ& fastQ) {
    string read_name = fastQ.seq_id;
    string s = fastQ.sequence;
    int u = 0, idx, pos;
    vector<match_res> v;
    vector<merge_res> res;
    int py = 0;
    char tag = 'l';
    for (int i = 0; i < s.size(); i++) {
        u = exSam.next[u][exSam.trans(s[i])];
        if (u == 0) {
            int end_pos = pos + py;
            v.clear();
            match_res::extend_left(graph,s, py, pos, idx, i, v);
            tag = 'l';
            if (end_pos == graph.node[idx].size()) {
                for (int j = 0; j < graph.G[idx].size(); j++) {
                    int id = graph.G[idx][j];
//                        cout<<strs[id][0]<<" "<<s[i]<<endl;
                    if (graph.node[id][0] == s[i]) {
                        match_res::extend_next(graph,s, i, id, v);
                        tag = 'n';
                    }
                }
            }
            merge_res t = merge_res::merge_match_res(v);
            res.push_back(t);
            py = -1;
        }
        idx = exSam.id[u];
        pos = exSam.first_pos[u] - py;
        py++;
    }
    if (tag == 'l') {
        v.clear();
        match_res::extend_left(graph,s, py, pos, idx, s.size(), v);
        merge_res t = merge_res::merge_match_res(v);
        res.push_back(t);
    }
    path_res path = path_res::merge_path(graph,res, tolerant_length, s.size());
    vector<merge_res> path_node = path.path;
    if(path_node.size()==0){
        return "match failed";
    }else{
//            for(int i=0;i<path_node.size();i++){
//                cout<<path_node[i]<<endl;
//            }

        string cigarStr = dpParser->get_cigar_str(graph,path,s);
        AlignmentResult res = transPathResToAlignmentResult(path,read_name,s,cigarStr);
        return res.getGFALine(include_cigar);
    }
}
