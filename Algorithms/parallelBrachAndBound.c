#include <stdio.h>
#include <float.h>
#include <omp.h>
#include <malloc.h>
#include "../utils.h"
#include "../DataStructure/DataStructure.h"
#include "../DataStructure/ParallelStack.h"

static double GetLowerBound(tsp_global params, const int citiesVisited[]);

static int IsAllCitiesVisited(int n, int currentStep);

stack_data parallelBranchAndBound(tsp_global params, stack_data bestKnown) {

    stack_data initialProblem;
    initialProblem.city = 0;
    initialProblem.pathLength = 0;
    initialProblem.step = 0;
    InitializeArray(params.cities, &initialProblem.visited);
    initialProblem.visited[initialProblem.city] = initialProblem.step + 1;

    #pragma omp parallel
    {
        #pragma omp single
        {
            initStackParallel();
            pushParallel(initialProblem);
        }
    }

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
                            if (bestKnown.pathLength >= pathLength) {
                                //printf("Better solution found: %f\n", pathLength);
                                bestKnown.pathLength = pathLength;
                                CopyArray(params.cities, subProblem.visited, bestKnown.visited);
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

        printf("Thread: %d finished working\n", omp_get_thread_num());
    }

    destroyStack();

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