//
// Created by tomas on 18.10.4.
//

#include <stdio.h>
#include <float.h>
#include <malloc.h>
#include "../DataStructure/DataStructure.h"
#include "../DataStructure/LinkedStack.h"
#include "../utils.h"

static double GetLowerBound(tsp_global params, const int citiesVisited[]);
static int IsAllCitiesVisited(int n, int currentStep);

stack_data simpleBranchAndBound(tsp_global params, stack_data bestKnown) {
    stack_node *stack = NULL;
    init(stack);

    stack_data initialProblem;
    initialProblem.city = 0;
    initialProblem.pathLength = 0;
    initialProblem.step = 0;
    InitializeArray(params.cities, &initialProblem.visited);
    initialProblem.visited[initialProblem.city] = initialProblem.step + 1;

    stack = push(stack, initialProblem);

    while (!isEmpty(stack)) {
        stack_data problem;
        stack = pop(stack, &problem);

        for (int i = 0; i < params.cities; ++i) {
            if (problem.visited[i]) {
                continue;
            }

            stack_data subproblem;
            subproblem.city = i;
            subproblem.step = problem.step + 1;
            subproblem.pathLength =
                    problem.pathLength + params.distanceMatrix[problem.city][subproblem.city];
            InitializeAndCopyArray(params.cities, problem.visited, &subproblem.visited);
            subproblem.visited[subproblem.city] = problem.step + 1;


            if (IsAllCitiesVisited(params.cities, subproblem.step)) {
                double pathLength = subproblem.pathLength + params.distanceMatrix[subproblem.city][0];

                if (bestKnown.pathLength >= pathLength) {
                    printf("Better solution found: %f\n", pathLength);
                    bestKnown.pathLength = pathLength;
                    CopyArray(params.cities, subproblem.visited, bestKnown.visited);
                }

                continue;
            }

            double pathEstimate = subproblem.pathLength + GetLowerBound(params, subproblem.visited);
            if (bestKnown.pathLength < pathEstimate) {
                continue;
            }

            stack = push(stack, subproblem);
        }

        free(problem.visited);
    }

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

    return lowerBound / 2 ;
}

static int IsAllCitiesVisited(int n, int currentStep) {
    return  n == (currentStep + 1);
}