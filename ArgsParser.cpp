//
// Created by wyl on 2022/8/4.
//
#include <iostream>
#include "ArgsParser.h"
using namespace std;
boost::program_options::variables_map parse_args_to_map(int argc, char** argv){
    boost::program_options::options_description mandatory("Mandatory parameters");
    mandatory.add_options()
            ("graph,g", boost::program_options::value<std::string>(), "input graph (.gfa / .vg)")
            ("reads,f", boost::program_options::value<std::vector<std::string>>()->multitoken(), "input reads (fasta or fastq, uncompressed or gzipped)")
            ("alignments-out,a", boost::program_options::value<std::vector<std::string>>(), "output alignment file (.gaf/.gam/.json)")
    ;
    boost::program_options::options_description sam("sam parameters");
    sam.add_options()
            ("penalty-ratio,p",boost::program_options::value<double>(),"Penalty ratio to caculate the Alignment Score")
            ("tolerant-length", boost::program_options::value<int>(), "the distance between two sam match result")
    ;
    boost::program_options::options_description general("general options");
    general.add_options()
            ("include-cigar",boost::program_options::value<bool>(),"append cigar string[default true],only useful for gaf file")
            ("beautiful-json",boost::program_options::value<bool>(),"if false the json string will be ziped in one line,if true the json will be spread[default false],only use ful for json")
            ("help,h","show the help sheet")
            ("threads,t",boost::program_options::value<int>(),"the num of threads")
    ;
    boost::program_options::options_description cmdline_options;
    cmdline_options.add(mandatory).add(sam).add(general);
    boost::program_options::variables_map vm;
    boost::program_options::store(boost::program_options::parse_command_line(argc, argv, cmdline_options), vm);
    boost::program_options::notify(vm);
    if(vm.size()==0||vm.count("help")){
        std::cout<<mandatory<<std::endl<<sam<<std::endl<<general<<std::endl;
        std::exit(0);
    }
    return vm;
}

Args parse_args(boost::program_options::variables_map vm){
    Args args;
    args.include_cigar = true;
    args.tolerant_length = 80;
    args.penalty_ratio = 0.66;
    args.threads = 1;
    args.beautiful_json = false;
    if(vm.count("graph"))args.graphFile = vm["graph"].as<string>();
    if(vm.count("reads"))args.readFiles = vm["reads"].as<vector<string>>();
    if(vm.count("alignments-out"))args.outGafFile = vm["alignments-out"].as<vector<string>>();
    if(vm.count("tolerant-length")){
        args.tolerant_length = vm["tolerant-length"].as<int>();
    }
    if(vm.count("penalty-ratio")){
        args.penalty_ratio = vm["penalty-ratio"].as<double>();
    }
    if(vm.count("beautiful-json")){
        args.beautiful_json = vm["beautiful-json"].as<bool>();
    }
    if(vm.count("include-cigar")){
        args.include_cigar = vm["include-cigar"].as<bool>();
    }
    if(vm.count("threads")){
        args.threads = vm["threads"].as<int>();
    }
    return args;
}