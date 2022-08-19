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
#include "ArgsParser.h"
#include "AlignmentResult.h"
#include "concurrentqueue/concurrentqueue.h"
#include "boost/thread.hpp"
struct Aligner{

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
    string alignRead(FastQ& fastqs);
    void MutiAlignRead();
    void print_path_res(path_res& path,string& read_name);
    AlignmentResult transPathResToAlignmentResult(path_res& path,string& read_name,string& s,string &cigar);
};
#endif //GSAM_ALIGNER_H
