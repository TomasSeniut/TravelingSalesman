cmake_minimum_required(VERSION 3.7.2)
project(TravelingSalesman C)

set(CMAKE_C_STANDARD 99)

set(CMAKE_C_FLAGS "-fopenmp")

set(SOURCE_FILES
        main.c
        DataStructure/DataStructure.h
        DataStructure/LinkedStack.c
        IO/IO.h
        IO/IO.c
        utils.c
        utils.h
        Algorithms/heuristics.c
        Algorithms/algorithms.h
        Algorithms/simpleBranchAndBound.c DataStructure/ParallelStack.c Algorithms/parallelBrachAndBound.c DataStructure/parallelStack.h)

add_executable(TravelingSalesman ${SOURCE_FILES})

target_link_libraries(TravelingSalesman m)