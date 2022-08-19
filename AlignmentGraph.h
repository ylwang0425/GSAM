//
// Created by wyl on 2022/8/5.
//

#ifndef GSAM_ALIGNMENTGRAPH_H
#define GSAM_ALIGNMENTGRAPH_H
#include<vector>
#include<iostream>

#include "GfaGraph.h"
using namespace std;
struct Node{
    string originalNodeName;
    char direction;
};
struct AlignmentGraph{
    int node_num,edge_num;
    vector<int>* rG;//反向图
    vector<int>* G;//正向图
    vector<string> node;
    vector<string> originalNodeName;
    AlignmentGraph(int n);
    static AlignmentGraph trans_GfaGraph_AlignmentGraph(GfaGraph graph);
    static int cast_node_pos(NodePos pos);
    Node cast_id_to_node(int id);
};

#endif //GSAM_ALIGNMENTGRAPH_H
