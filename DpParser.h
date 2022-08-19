//
// Created by wyl on 2022/8/5.
//
/**
 * 通过dp的方式获取cigar string
 */
#ifndef GSAM_DPPARSER_H
#define GSAM_DPPARSER_H
#include<iostream>
#include<sstream>
#include<stack>
#include "MatchResult.h"
using namespace std;
struct DpParser{
    int maxn;
    int** f;
    pair<int,int>** pre;
    char** opt;
    stack<char> s;
    string dp_get_cigar_str(string s1,string s2,int& size,int& delete_size,int &mismatch_size,int &insert_size);
    string get_cigar_str(AlignmentGraph& graph, path_res& res, string s);
    DpParser();
    DpParser(int tolerant_length);
};
struct WholeLineDpParser{
    unordered_map<int,unordered_map<int,int>> f;
    unordered_map<int,unordered_map<int,pair<int,int>>> pre;
    unordered_map<int,unordered_map<int,char>> opt;
    stack<char> s;
};
#endif //GSAM_DPPARSER_H
