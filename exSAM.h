//
// Created by wyl on 2022/8/5.
//

#ifndef GSAM_EXSAM_H
#define GSAM_EXSAM_H
#include<iostream>
#include<queue>
#include "AlignmentGraph.h"

using namespace std;
struct exSAM {
    int CHAR_NUM;
    int MAXN;
    int* len;             // 节点长度
    int* link;            // 后缀链接，link
    int **next;  // 转移
    int* node_id;
    int tot;                   // 节点总数：[0, tot)
    int* id;              // 每个状态对应的广搜顺序第一个节点编号
    int* first_pos;        // 第一次出现的位置
    int trans(char c);
    void init(int MAXN,int CHAR_NUM);
    int insertSAM(int last, int c, int idx);
    int insertTrie(int cur, int c, int idx);
    void insert(const char *s, int n, int idx);
    void insert(string& s, int idx);
    void build();
    exSAM(int MAXN,int CHAR_NUM){
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
    static exSAM build_exSAM_from_AlignmentGraph(AlignmentGraph& graph);
};
#endif //GSAM_EXSAM_H
