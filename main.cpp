#include <iostream>
#include "ArgsParser.h"
#include <boost/program_options.hpp>
#include "GfaGraph.h"
#include "AlignmentGraph.h"
#include "exSAM.h"
#include "fastqloader.h"
#include "Aligner.h"
using namespace std;


int main(int argc, char** argv) {

    boost::program_options::variables_map vm = parse_args_to_map(argc,argv);
    Args args = parse_args(vm);
    cout<<"Load graph from "<<args.graphFile<<endl;
    GfaGraph gfaGraph = GfaGraph::LoadFromFile(args.graphFile);
    AlignmentGraph alignmentGraph = AlignmentGraph::trans_GfaGraph_AlignmentGraph(gfaGraph);
    exSAM exSam = exSAM::build_exSAM_from_AlignmentGraph(alignmentGraph);
    Aligner aligner(alignmentGraph,args,exSam);
    aligner.alignReads();
    return 0;
}
