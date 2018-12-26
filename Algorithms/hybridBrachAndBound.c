//
// Created by tomas on 18.12.13.
//

#include <stdio.h>
#include <float.h>
#include <omp.h>
#include <malloc.h>
#include <mpi.h>
#include <stdlib.h>
#include "../utils.h"
#include "../DataStructure/DataStructure.h"
#include "../DataStructure/ParallelStack.h"
#include "../DataStructure/LinkedQueue.h"
#include "../MPI/mpiWrapper.h"

static double GetLowerBound(tsp_global params, const int citiesVisited[]);

static int IsAllCitiesVisited(int n, int currentStep);

stack_data hybridBranchAndBound(tsp_global params, stack_data bestKnown) {

    int size, rank;
    MPI_Wrapper_Init(params.cities, &size, &rank);

    stack_data initialProblem;
    initialProblem.city = 0;
    initialProblem.pathLength = 0;
    initialProblem.step = 0;
    InitializeArray(params.cities, &initialProblem.visited);
    initialProblem.visited[initialProblem.city] = initialProblem.step + 1;

    //split init problem
    initQueue();
    enQueue(initialProblem);

    int initNodesPerProcess = 10;
    while (queueSize() < initNodesPerProcess * size) {
        stack_data problem = deQueue();

        for (int i = 0; i < params.cities; ++i) {
            if (problem.visited[i]) {
                continue;
            }

            stack_data subProblem;
            subProblem.city = i;
            subProblem.step = problem.step + 1;
            subProblem.pathLength = problem.pathLength + params.distanceMatrix[problem.city][subProblem.city];
            InitializeAndCopyArray(params.cities, problem.visited, &subProblem.visited);
            subProblem.visited[subProblem.city] = problem.step + 1;

            enQueue(subProblem);
        }
    }

    // create stack
    #pragma omp parallel
    {
        #pragma omp single
        {
            initStackParallel();
        }
    }

    MPI_Barrier(MPI_COMM_WORLD);

    // distribute
    while (!isQueueEmpty()) {
        int left = queueSize();
        stack_data problem = deQueue();

        int destination = left % size;
        if (destination == rank) {
            pushParallel(problem);
        }
    }

    printf("I am process %d and have stack of size %d\n", rank, stackSize());

    #pragma omp parallel
    {
        while (!isEmptyParallel()) {
            stack_data problem;
            int success = popParallel(&problem);
            if (!success) {
                continue;
            }

            for (int i = 0; i < params.cities; ++i) {
                if (problem.visited[i]) {
                    continue;
                }

                stack_data subProblem;
                subProblem.city = i;
                subProblem.step = problem.step + 1;
                subProblem.pathLength = problem.pathLength + params.distanceMatrix[problem.city][subProblem.city];
                InitializeAndCopyArray(params.cities, problem.visited, &subProblem.visited);
                subProblem.visited[subProblem.city] = problem.step + 1;

                if (IsAllCitiesVisited(params.cities, subProblem.step)) {
                    double pathLength = subProblem.pathLength + params.distanceMatrix[subProblem.city][0];

                    if (bestKnown.pathLength >= pathLength) {
                        #pragma omp critical
                        {
                            MPI_Wrapper_Receive_Bound(&bestKnown);
                        }

                        if (bestKnown.pathLength >= pathLength) {
                            #pragma omp critical
                            {
                                if (bestKnown.pathLength >= pathLength) {
                                    //printf("Better solution found: %f\n", pathLength);
                                    bestKnown.pathLength = pathLength;
                                    CopyArray(params.cities, subProblem.visited, bestKnown.visited);

                                    MPI_Wrapper_Share_Bound(bestKnown);
                                }
                            }
                        }
                    }

                    continue;
                }

                double pathEstimate = subProblem.pathLength + GetLowerBound(params, subProblem.visited);
                if (bestKnown.pathLength < pathEstimate) {
                    continue;
                }

                pushParallel(subProblem);
            }

            free(problem.visited);
        }
    }

    destroyStack();

    MPI_Wrapper_Synchronize(&bestKnown);
    printf("Finished work on process %d\n", rank);

    MPI_Wrapper_Finalize();

    return bestKnown;
}

static double GetLowerBound(tsp_global params, const int citiesVisited[]) {
    double lowerBound = 0;

    for (int i = 0; i < params.cities; ++i) {
        if (citiesVisited[i]) {
            continue;
        }

        double firstNearestCity = DBL_MAX;
        double secondNearestCity = DBL_MAX;

        for (int j = 0; j < params.cities; ++j) {
            if (citiesVisited[j] || i == j) {
                continue;
            }

            double distance = params.distanceMatrix[i][j];

            if (distance < secondNearestCity) {
                if (distance < firstNearestCity) {
                    if (firstNearestCity < secondNearestCity) {
                        secondNearestCity = firstNearestCity;
                    }

                    firstNearestCity = distance;
                } else {
                    secondNearestCity = distance;
                }
            }
        }

        if (secondNearestCity == DBL_MAX) {
            return 0;
        }

        lowerBound += firstNearestCity + secondNearestCity;
    }

    return lowerBound / 2;
}

static int IsAllCitiesVisited(int n, int currentStep) {
    return n == (currentStep + 1);

}