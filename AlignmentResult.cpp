//
// Created by wyl on 2022/8/8.
//
#include "AlignmentResult.h"
string AlignmentResult::getGFALine(bool include_cigar){
    stringstream ss;
    ss<<queryName<<"\t"<<queryLen<<"\t"<<startPos<<"\t"<<endPos<<"\t"<<direction<<"\t"<<path<<"\t"
    <<pathLen<<"\t"<<start<<"\t"<<end<<"\t"<<matches<<"\t"<<blockLen<<"\t"<<mappingQuality;

        ss<< "\t" << "NM:i:" << (mismatches + deletes + inserts);
        ss << "\t" << "AS:f:" << alignmentScore;
        ss << "\t" << "dv:f:" << 1.0-((double)matches / (double)(matches + mismatches + deletes + inserts));
        ss << "\t" << "id:f:" << ((double)matches / (double)(matches + mismatches + deletes + inserts));
    if (include_cigar) ss << "\t" << "cg:Z:" << cigarStr;
    return ss.str();
}