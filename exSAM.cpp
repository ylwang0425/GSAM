//
// Created by wyl on 2022/8/5.
//
#include "exSAM.h"
int exSAM::trans(char c) {
    int num;
    switch (c) {
        case 'A':
        case 'a':num = 0;break;
        case 'c':
        case 'C':num = 1;break;
        case 'G':
        case 'g':num = 2;break;
        case 't':
        case 'T':num = 3;break;
    }
    return num;
}

void exSAM::init(int MAXN,int CHAR_NUM) {  // 初始化函数
    this->MAXN = MAXN;
    this->CHAR_NUM = CHAR_NUM;
    this->len = new int[2*MAXN];
    this->link = new int[2*MAXN];
    this->next = new int*[2*MAXN];
    for(int i=0;i<2*MAXN;i++){
        this->next[i] = (int*)calloc(0,sizeof(int)*CHAR_NUM);
    }
    this->node_id = new int[2*MAXN];
    this->id = new int[2*MAXN];
    this->first_pos = new int[2*MAXN];
    this->tot = 1;
    this->link[0] = -1;
}

int exSAM::insertSAM(int last, int c, int idx) {  // last 为父 c 为子
    int cur = next[last][c];
    if (len[cur]) return cur;
    len[cur] = len[last] + 1;
    first_pos[cur] = len[cur] - 1;
    id[cur] = idx;
    int p = link[last];
    while (p != -1) {
        if (!next[p][c])
            next[p][c] = cur;
        else
            break;
        p = link[p];
    }
    if (p == -1) {
        link[cur] = 0;
        return cur;
    }
    int q = next[p][c];
    if (len[p] + 1 == len[q]) {
        link[cur] = q;
        return cur;
    }
    int clone = tot++;
    for (int i = 0; i < CHAR_NUM; ++i)
        next[clone][i] = len[next[q][i]] != 0 ? next[q][i] : 0;
    len[clone] = len[p] + 1;
    while (p != -1 && next[p][c] == q) {
        next[p][c] = clone;
        p = link[p];
    }
    link[clone] = link[q];
    link[cur] = clone;
    link[q] = clone;
    first_pos[clone] = first_pos[q];
    id[clone] = id[q];
    return cur;
}

int exSAM::insertTrie(int cur, int c, int idx) {
    if (next[cur][c]) return next[cur][c];  // 已有该节点 直接返回
    node_id[tot] = idx;
    return next[cur][c] = tot++;            // 无该节点 建立节点
}


void exSAM::insert(const char *s, int n, int idx) {
    int root = 0;
    for (int i = 0; i < n; ++i)
        root =
                insertTrie(root, trans(s[i]), idx);  // 一边插入一边更改所插入新节点的父节点
}

void exSAM::insert(string& s, int idx) {
    int root = 0;
    for (int i = 0; i < s.size(); i++) {
//        cout<<s[i]<<" "<<trans(s[i])<<endl;
        root = insertTrie(root, trans(s[i]), idx);
    }
}

void exSAM::build() {
    queue<pair<int, int>> q;
    for (int i = 0; i < CHAR_NUM; ++i)
        if (next[0][i]) {
            q.push({i, 0});
        }
    while (q.size() > 0) {  // 广搜遍历
        auto item = q.front();
        q.pop();
        int idx = node_id[next[item.second][item.first]];
        auto last = insertSAM(item.second, item.first, idx);
        for (int i = 0; i < CHAR_NUM; ++i)
            if (next[last][i]) {
                q.push({i, last});
            }
    }
}

exSAM exSAM::build_exSAM_from_AlignmentGraph(AlignmentGraph& graph) {
     int sumn = 0;
     for(int i=0;i<graph.node.size();i++){
         sumn+=graph.node[i].length();
     }
     exSAM exSam(sumn,4);
     vector<int>* G = graph.G;
     for(int i=0;i<graph.node_num;i++){
         string u = graph.node[i];
         for(int j=0;j<G[i].size();j++){
             string v = graph.node[G[i][j]];
             exSam.insert(u,i);
             exSam.insert(v,G[i][j]);
         }
     }
     exSam.build();
     return exSam;
}
