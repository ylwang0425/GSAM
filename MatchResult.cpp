//
// Created by wyl on 2022/8/6.
//

#include "MatchResult.h"

ostream& operator<<(ostream& out,node_res res){
    out << "->" << res.node_id / 2 << "+-"[(res.node_id & 1)] << ":[" << res.node_range.first << "-" << res.node_range.second
         << "]";
    return out;
}

ostream& operator<<(ostream& out,merge_res res){
    cout << "s[" << res.string_range.first << "-" << res.string_range.second << "]" << " matched " << res.match_size
         << " char "<< " deleted  "<<res.delete_size<<" char "<< " insert "<<res.insert_size<<" "<<" mismatch "<<res.mismatch_size<<" char at ";
    for (int i = 0; i < res.v.size(); i++) {
        cout<<res.v[i];
    }
    return out;
}

ostream& operator<<(ostream& out,match_res res){
    out << "s[" << res.string_range.first << "-" << res.string_range.second << "]" << " matched " << res.match_size
         << " char at " << res.node_id / 2
         << "+-"[(res.node_id & 1)] << ":[" << res.node_range.first << "-" << res.node_range.second << "]";
    return out;
}

match_res match_res::extend_pre(AlignmentGraph& graph,string &s, int i, int id, vector<match_res> &v) {
    int right = graph.node[id].size() - 1;
    int left = graph.node[id].size() - 1;
    match_res res;
    res.string_range.second = i;
    bool tag = true;
    for (i, left; left >= 0, i >= 0; left--, i--) {
        if (graph.node[id][left] == s[i]) {}
        else {
            tag = false;
            break;
        }
    }
    res.string_range.first = i + 1;
    res.match_size = res.string_range.second - res.string_range.first + 1;
    res.node_range = {left + 1, right};
    res.node_id = id;
    if (tag == true) {
        for (int t = 0; t < graph.rG[id].size(); t++) {
            int j = graph.rG[id][t];
            int len = graph.node[j].size();
            if (s[i] == graph.node[j][len - 1] && i >= 0) {
                match_res::extend_pre(graph,s, i, j, v);
            }
        }
    }
    v.push_back(res);
    return res;
}

match_res match_res::extend_left(AlignmentGraph& graph,string &s, int &py, int pos, int idx, int i, vector<match_res> &v) {
    int match = py;
    int left = pos;
    int right = pos + py - 1;
    int t = i - py - 1;
    for (int j = left - 1; j >= 0 && t >= 0; j--, t--) {
        if (graph.node[idx][j] == s[t]) {
            left--;
            match++;
        } else break;
    }
    if (left == 0) {
        for (int i = 0; i < graph.rG[idx].size(); i++) {
            int j = graph.rG[idx][i];
            int len = graph.node[j].size();
            if (graph.node[j][len - 1] == s[t]) {
                match_res::extend_pre(graph,s, t, j, v);
            }
        }
    }
//    cout <<"s[" << t + 1 << "-" << t + match << "]" << " matched " << match << " char at " << idx / 2
//         << "+-"[(idx & 1)] << ":[" << left << "-" << right << "]" << endl;
//    py = -1;
    match_res res;
    res.node_id = idx;
    res.match_size = match;
    res.node_range = {left, right};
    res.string_range = {t + 1, t + match};
    v.push_back(res);
    return res;
}

/**
 *
 * @param s query string
 * @param i query string 的索引
 * @param idx 节点编号
 * @return
 */
match_res match_res::extend_next(AlignmentGraph& graph,string &s, int &i, int id, vector<match_res> &v) {
    int left = 0;
    int right = 0;
    match_res res;
    res.string_range.first = i;
    bool tag = true;
//    cout << strs[id].size() << endl;
    for (right; right < graph.node[id].size() && i < s.size(); i++, right++) {
//        cout << right << " " << i << strs[id][right] << " " << s[i] << endl;
        if (graph.node[id][right] == s[i]) {}
        else {
            tag = false;
            break;
        }
    }
    res.string_range.second = i - 1;
    res.match_size = res.string_range.second - res.string_range.first + 1;
    res.node_range = {left, right - 1};
    res.node_id = id;
//    cout << "s[" << res.string_range.first << "-" << res.string_range.second << "]" << "matched " << res.match_size
//         << " char at " << res.node_id / 2
//         << "+-"[(res.node_id & 1)] << ":[" << res.node_range.first << "-" << res.node_range.second << "]" << endl;
    v.push_back(res);
    if (tag == true) {
        for (int t = 0; t < graph.G[id].size(); t++) {
            int j = graph.G[id][t];
            if (s[i] == graph.node[j][0] && i < s.size()) {
                match_res::extend_next(graph,s, i, j, v);
            }
        }
    }
    return res;
}
merge_res merge_res::merge_match_res(vector<match_res> &v) {
    merge_res res;
    if (v.size() == 0) {
        return res;
    }
    if (v.size() >= 1) {
        res.match_size = v[0].match_size;
        res.string_range = v[0].string_range;
        node_res nd;
        nd.node_id = v[0].node_id;
        nd.node_range = v[0].node_range;
        res.v.push_back(nd);
    }
    if (v.size() == 1) {
        return res;
    }
    for (int i = 0, j = 1; j < v.size(); i++, j++) {
        if (v[i].string_range.second + 1 == v[j].string_range.first) {
            res.string_range.second = v[j].string_range.second;
            res.match_size += v[j].match_size;
            node_res t;
            t.node_id = v[j].node_id;
            t.node_range = v[j].node_range;
            res.v.push_back(t);
        }
    }
    return res;
}
path_res path_res::merge_path(AlignmentGraph& graph,vector<merge_res> &res, int tolerant_length, int tot_length) {
    vector<merge_res> path_nodes;
    merge_res path;
    for (int s = 0; s < res.size(); s++) {
        path_nodes.clear();
        path.v.clear();
        path.string_range = res[s].string_range;
        path.match_size = res[s].match_size;
        for (int i = 0; i < res[s].v.size(); i++) {
            path.v.push_back(res[s].v[i]);
        }
        path_nodes.push_back(res[s]);
        for (int i = s + 1; i < res.size(); i++) {
            pair<int,int> before = path_nodes.back().string_range;
            pair<int,int> now = res[i].string_range;
            if(before.first>=now.first&&before.second<=now.second){
                path_nodes.pop_back();
                path_nodes.push_back(res[i]);
                continue;
            }
            int len = path.v.size();
            int end_id = path.v[len - 1].node_id;
            int end = path.v[len - 1].node_range.second;
            vector<node_res> temp = res[i].v;
            if (temp.size() == 1) {
                node_res t = temp[0];
                if (end_id == t.node_id) {
                    if (end + tolerant_length >= t.node_range.first && t.node_range.first>end) {
                        path.string_range.second = res[i].string_range.second;
                        path.match_size += res[i].match_size;
                        path.v[len - 1].node_range.second = t.node_range.second;
                        path_nodes.push_back(res[i]);
                    }
                } else {
                    //TODO 深搜下一个节点
                    int u = end_id;
                    for (int tt = 0; tt < graph.G[u].size(); tt++) {
                        int v = graph.G[u][tt];
                        if (t.node_id == v) {
                            int tolerant_end = end + tolerant_length;
                            int cur_end = graph.node[u].size()+t.node_range.first;
                            if(cur_end<=tolerant_end){
                                path.string_range.second = res[i].string_range.second;
                                path.match_size+=res[i].match_size;
                                path.v[len-1].node_range.second = graph.node[u].size()-1;
                                path.v.push_back(t);
                                path_nodes.push_back(res[i]);
                            }
                        }
                    }
                }
            } else {
                node_res t = temp[0];
                if (end_id == t.node_id) {
                    if (end + tolerant_length >= t.node_range.first && t.node_range.first>end) {
                        path.string_range.second = res[i].string_range.second;
                        path.match_size += res[i].match_size;
                        path.v[len - 1].node_range.second = t.node_range.second;
                        for (int start = 1; start < temp.size(); start++) {
                            path.v.push_back(temp[start]);
                        }
                        path_nodes.push_back(res[i]);
                    }
                } else {
                    //TODO 深搜下一个节点
                    int u = end_id;
                    for (int tt = 0; tt < graph.G[u].size(); tt++) {
                        int v = graph.G[u][tt];
                        if (t.node_id == v) {
                            int tolerant_end = end + tolerant_length;
                            int cur_end = graph.node[u].size()+t.node_range.first;
                            if(cur_end<=tolerant_end){
                                path.string_range.second = res[i].string_range.second;
                                path.match_size+=res[i].match_size;
                                path.v[len-1].node_range.second = graph.node[u].size()-1;
                                for(int start = 0; start<temp.size();start++){
                                    path.v.push_back(temp[start]);
                                }
                                path_nodes.push_back(res[i]);
                            }
                        }
                    }
                }
            }
        }
        if (path.match_size >= tot_length / 2) {
            path_res path_res;
            path_res.mergeRes = path;
            path_res.path = path_nodes;
            return path_res;
        }
    }
    path_res path_res;
    return path_res;
}