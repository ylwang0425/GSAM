//
// Created by wyl on 2022/8/8.
//
#include "AlignmentResult.h"
string AlignmentResult::getGFALine(bool include_cigar){
    stringstream ss;
    ss<<queryName<<"\t"<<queryLen<<"\t"<<startPos<<"\t"<<endPos<<"\t"<<direction<<"\t"<<path<<"\t"
    <<pathLen<<"\t"<<start<<"\t"<<end<<"\t"<<matches<<"\t"<<blockLen<<"\t"<<mappingQuality;

        ss<< "\t" << "NM:i:" << (mismatches + deletes + inserts);
        ss << "\t" << "AS:f:" << alignmentScore;
        ss << "\t" << "dv:f:" << 1.0-((double)matches / (double)(matches + mismatches + deletes + inserts));
        ss << "\t" << "id:f:" << ((double)matches / (double)(matches + mismatches + deletes + inserts));
    if (include_cigar) ss << "\t" << "cg:Z:" << cigarStr;
    return ss.str();
}


string AlignmentResult::getJsonLine(bool beautiful_json) {
    Json::Value root;
//    cout<<s<<endl;
//    cout<<startPos<<" "<<endPos-startPos+1<<endl;
    root["sequence"] = s.substr(startPos,endPos-startPos+1);
    root["path"]=path;
    root["name"] = queryName;
    root["score"]= mismatches + deletes + inserts;
    root["query_position"] = startPos;
    root["identity"] = ((double)matches / (double)(matches + mismatches + deletes + inserts));
    string res;
    if(beautiful_json){
        Json::StyledWriter writer;
        res = writer.write(root);
    }else{
        Json::FastWriter writer;
        res = writer.write(root);
    }
    return res.substr(0,res.size()-1);
}
