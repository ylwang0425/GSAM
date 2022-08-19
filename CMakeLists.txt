cmake_minimum_required(VERSION 3.21)
project(GSAM)

set(CMAKE_CXX_STANDARD 11)

add_executable(GSAM main.cpp)
add_executable(ArgsParser.cpp ArgsParser.cpp)
add_executable(AlignmentGraph.cpp AlignmentGraph.cpp)
add_executable(exSAM.cpp exSAM.cpp)
add_executable(DpParser.cpp DpParser.cpp)
add_executable(fastqloader.cpp fastqloader.cpp)
add_executable(MatchResult.cpp MatchResult.cpp)