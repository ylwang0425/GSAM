//
// Created by wyl on 2022/8/6.
//

#ifndef GSAM_ALIGNER_H
#define GSAM_ALIGNER_H
#include "AlignmentGraph.h"
#include "exSAM.h"
#include "fastqloader.h"
#include "MatchResult.h"
#include "DpParser.h"
#include <ostream>
#include <cstdio>
#include "ArgsParser.h"
#include "AlignmentResult.h"
#include "concurrentqueue/concurrentqueue.h"
#include "boost/thread.hpp"
#include "json/json/json.h"
struct Aligner{
    bool beautiful_json;
    vector<string> file_names;
    AlignmentGraph graph;
    int tolerant_length;
    bool include_cigar;
    double penalty_radio;
    double penalty_coefficient;
    vector<string> reads;
    vector<string> outFile;
    exSAM exSam;
    DpParser* dpParser;
    vector<string> resString;
    vector<boost::thread*> threads;
    int thread_num;
    moodycamel::ConcurrentQueue<pair<int,FastQ>> aq;
    Aligner(AlignmentGraph& g,Args& args,exSAM& sam);
    void alignReads();
    string gfaAlignRead(FastQ& fastqs);
    string jsonAlignRead(FastQ& fastqs);
    void MutiAlignRead();
    void print_path_res(path_res& path,string& read_name);
    AlignmentResult transPathResToAlignmentResult(path_res& path,string& read_name,string& s,string &cigar);
    AlignmentResult transPathResToJsonResult(path_res& path,string& read_name,string& s,string &cigar);
    string transCigarToJsonString(string s,int start,string& cigar,vector<node_res>);
};
#endif //GSAM_ALIGNER_H
