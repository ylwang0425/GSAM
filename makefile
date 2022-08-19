flag = -std=c++17
cpps = main.cpp ArgsParser.cpp GfaGraph.cpp AlignmentGraph.cpp exSAM.cpp Aligner.cpp fastqloader.cpp MatchResult.cpp DpParser.cpp AlignmentResult.cpp
options = -lboost_program_options -lboost_thread -lboost_system

main: main.cpp ArgsParser.cpp
	g++ ${flag} ${cpps} -o gsam ${options}

clean:
	rm -rf gsam
