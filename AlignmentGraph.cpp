//
// Created by wyl on 2022/8/5.
//
#include <algorithm>
#include "AlignmentGraph.h"

AlignmentGraph::AlignmentGraph(int n) {
    G=new vector<int>[n];
    rG=new vector<int>[n];
    node_num=n;
}

AlignmentGraph AlignmentGraph::trans_GfaGraph_AlignmentGraph(GfaGraph graph) {
    AlignmentGraph alignmentGraph(graph.nodes.size()*2+5);
    alignmentGraph.node_num = graph.nodes.size()*2;
    alignmentGraph.edge_num = graph.edges.size();
    vector<string> node = graph.nodes;
    vector<string> originalNodeName = graph.originalNodeName;
    for(int i=0;i<node.size();i++){
        alignmentGraph.node.push_back(node[i]);
        string s = node[i];
        reverse(s.begin(),s.end());
        alignmentGraph.node.push_back(s);
        alignmentGraph.originalNodeName.push_back(originalNodeName[i]);
        alignmentGraph.originalNodeName.push_back(originalNodeName[i]);
    }
    std::vector<std::tuple<NodePos, NodePos, size_t>> edges = graph.edges;
    for(int i=0;i<edges.size();i++){
        int from_id = cast_node_pos(get<0>(edges[i]));
        int to_id = cast_node_pos(get<1>(edges[i]));
//        cout<<alignmentGraph.originalNodeName[from_id]<<" "<<alignmentGraph.originalNodeName[to_id]<<endl;
        alignmentGraph.G[from_id].push_back(to_id);
        alignmentGraph.rG[to_id].push_back(from_id);
    }
    return alignmentGraph;
}

int AlignmentGraph::cast_node_pos(NodePos pos) {
    return 2*pos.id+pos.end;
}

Node AlignmentGraph::cast_id_to_node(int id) {
    Node node;
    node.originalNodeName = originalNodeName[id>>1];
    node.direction = "+-"[id&1];
    return node;
}
