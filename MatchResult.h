//
// Created by wyl on 2022/8/6.
//

#ifndef GSAM_MATCHRESULT_H
#define GSAM_MATCHRESULT_H
#include<iostream>
#include<vector>
#include "AlignmentGraph.h"
using namespace std;

struct match_res {
    int node_id;
    pair<int, int> node_range;
    int match_size;
    pair<int, int> string_range;
    friend ostream& operator<<(ostream& out,match_res res);
    static match_res extend_next(AlignmentGraph& graph,string &s, int &i, int id, vector<match_res> &v);
    static match_res extend_pre(AlignmentGraph& graph,string &s, int i, int id, vector<match_res> &v);
    static match_res extend_left(AlignmentGraph& graph,string &s, int &py, int pos, int idx, int i, vector<match_res> &v);
};

struct node_res {
    pair<int, int> node_range;
    int node_id;
    friend ostream& operator<<(ostream& out,node_res res);
};

struct merge_res {
    int match_size;
    int delete_size;
    int mismatch_size;
    int insert_size;
    pair<int, int> string_range;
    vector<node_res> v;
    merge_res(){
        match_size=0;
        delete_size=0;
        mismatch_size=0;
        insert_size=0;
    }
    friend ostream& operator<<(ostream& out,merge_res res);
    static merge_res merge_match_res(vector<match_res> &v);
};

struct path_res {
    merge_res mergeRes;
    vector<merge_res> path;
    static path_res merge_path(AlignmentGraph& graph,vector<merge_res> &res, int tolerant_length, int tot_length);
};
#endif //GSAM_MATCHRES_H
