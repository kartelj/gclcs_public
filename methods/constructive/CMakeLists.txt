cmake_minimum_required(VERSION 3.14)
project(src)

set(CMAKE_CXX_STANDARD 11)

include_directories(.)
include_directories("/home/Christoph/mhlib/")

add_executable(src
        CLCS.C
        CLCS_inst.C
        CLCS_inst.h
        CLCS_solution.C
        CLCS_solution.h
        BeamSearch.C
        BeamSearch.h
        CLCS_evaluation.C
        CLCS_evaluation.h
        structures/BoundedHeap.C
        structures/BoundedHeap.h
        structures/VebTree.C
        structures/VebTree.h
        AStar.C
        AStar.h)

target_link_libraries(src "/home/Christoph/mhlib/libmh.a")
add_definitions(-DVERSION=0.1)