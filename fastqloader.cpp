#include <algorithm>
#include <iostream>
#include "fastqloader.h"

std::vector<FastQ> loadFastqFromFile(std::string filename, bool includeQuality)
{
	std::vector<FastQ> result;
	FastQ::streamFastqFromFile(filename, includeQuality, [&result](FastQ& fq) {
		result.emplace_back(std::move(fq));
	});
	return result;
}

//int main(){
//    std::vector<FastQ> fastq = loadFastqFromFile("/home/wyl/CLionProjects/GSAM/test/read1000.fa.gz");
//    for (int i = 0; i < fastq.size(); ++i) {
//        FastQ t = fastq[i];
//        std::cout<<t.seq_id<<" "<<t.sequence<<" "<<t.quality<<std::endl;
//    }
//}