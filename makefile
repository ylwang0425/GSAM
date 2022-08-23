flag = -std=c++17 -O3 -Wall -Wextra
cpps = main.cpp ArgsParser.cpp GfaGraph.cpp AlignmentGraph.cpp exSAM.cpp Aligner.cpp fastqloader.cpp MatchResult.cpp DpParser.cpp AlignmentResult.cpp json/jsoncpp.cpp
options = -lz -lboost_program_options -lboost_thread -lboost_system -lpthread -g -Izstr/src

main: main.cpp ArgsParser.cpp
	g++ ${flag} ${cpps} -o gsam ${options}

clean:
	rm -rf gsam
