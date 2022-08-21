//
// Created by wyl on 2022/8/4.
//
#include<boost/program_options.hpp>
using namespace std;
#ifndef GSAM_ARGSPARSER_H
#define GSAM_ARGSPARSER_H
struct Args{
    string graphFile;
    vector<string> readFiles;
    vector<string> outGafFile;
    int tolerant_length;
    double penalty_ratio;
    bool beautiful_json;
    bool include_cigar;
    bool accurate_model;
    int threads;
    Args parse_args(boost::program_options::variables_map vm);
    boost::program_options::variables_map parse_args_to_map(int argc, char** argv);
};


boost::program_options::variables_map parse_args_to_map(int argc, char** argv);
Args parse_args(boost::program_options::variables_map vm);
#endif //GSAM_ARGSPARSER_H
