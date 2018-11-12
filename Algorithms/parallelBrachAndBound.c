//
// Created by tomas on 18.11.10.
//

//
// Created by tomas on 18.10.4.
//

#include <stdio.h>
#include <float.h>
#include <malloc.h>
#include <omp.h>
#include "../utils.h"
#include "../DataStructure/DataStructure.h"
#include "../DataStructure/parallelStack.h"

double GetLowerBound(tsp_global params, const int citiesVisited[]);

int IsAllCitiesVisited(int n, const int cityArray[]);

stack_data parallelBranchAndBound(tsp_global params, stack_data bestKnown) {
    initStackParallel();

    stack_data initialProblem;
    initialProblem.city = 0;
    initialProblem.pathLength = 0;
    initialProblem.step = 0;
    InitializeArray(params.cities, &initialProblem.visited);
    initialProblem.visited[initialProblem.city] = initialProblem.step + 1;

    pushParallel(initialProblem);

    #pragma omp parallel
    {
        while (!isEmptyParallel() || isWorkingFlagged()) {

            stack_data problem;
            if (!popParallelAndFlagWorking(&problem)) {
                continue;
            }

            for (int i = 0; i < params.cities; ++i) {
                if (problem.visited[i]) {
                    continue;
                }

                stack_data subProblem;
                subProblem.city = i;
                subProblem.step = problem.step + 1;
                subProblem.pathLength =
                        problem.pathLength + params.distanceMatrix[problem.city][subProblem.city];
                InitializeAndCopyArray(params.cities, problem.visited, &subProblem.visited);
                subProblem.visited[subProblem.city] = problem.step + 1;


                if (IsAllCitiesVisited(params.cities, subProblem.visited)) {
                    double pathLength = subProblem.pathLength + params.distanceMatrix[subProblem.city][0];

                    #pragma omp critical
                    {
                        if (bestKnown.pathLength >= pathLength) {
                            printf("Better solution found: %f\n", pathLength);
                            bestKnown.pathLength = pathLength;
                            CopyArray(params.cities, subProblem.visited, bestKnown.visited);
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
            unflagWorking();
        }

        printf("Thread: %d finished working\n", omp_get_thread_num());
    }

    return bestKnown;
}

double GetLowerBound(tsp_global params, const int citiesVisited[]) {
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

int IsAllCitiesVisited(int n, const int cityArray[]) {
    for (int i = 0; i < n; ++i) {
        if (!cityArray[i]) {
            return 0;
        }
    }

    return 1;
}