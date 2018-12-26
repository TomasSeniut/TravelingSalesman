cmake_minimum_required(VERSION 3.7.2)
project(TravelingSalesman C)

find_package(MPI REQUIRED)

set(CMAKE_C_STANDARD 99)

set(CMAKE_C_FLAGS "-fopenmp")

include_directories(${MPI_INCLUDE_PATH})

SET(CMAKE_C_COMPILER mpicc)
SET(CMAKE_CXX_COMPILER mpicxx)

set(SOURCE_FILES
        main.c
        utils.c
        utils.h
        IO/IO.h
        IO/IO.c
        DataStructure/DataStructure.h
        DataStructure/LinkedStack.c
        DataStructure/LinkedStack.h
        DataStructure/ParallelStack.c
        DataStructure/ParallelStack.h
        Algorithms/algorithms.h
        Algorithms/heuristics.c
        Algorithms/simpleBranchAndBound.c
        Algorithms/parallelBrachAndBound.c
        Algorithms/hybridBrachAndBound.c DataStructure/LinkedQueue.c DataStructure/LinkedQueue.h MPI/mpiWrapper.c MPI/mpiWrapper.h)

add_executable(TravelingSalesman ${SOURCE_FILES})

target_link_libraries(TravelingSalesman m)