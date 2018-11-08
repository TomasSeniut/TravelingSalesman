//
// Created by tomas on 18.9.30.
//

#include <math.h>
#include <stdlib.h>
#include "DataStructure/DataStructure.h"

double calculate_distance(City point1, City point2) {
    return sqrt(pow(point1.y - point2.y, 2) + pow(point1.x - point2.x, 2));
}

void PopulateDistanceMatrix(int n, City *array, double **distanceMatrix) {
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            double temp = calculate_distance(array[i], array[j]);
            distanceMatrix[i][j] = temp;
        }
    }
}

void InitializeArray(int n, int ** array) {
    *array = malloc(n * sizeof(int));

    for (int i = 0; i < n; ++i) {
        (*array)[i] = 0;
    }
}

void CopyArray(int n, int from[], int to[]) {
    for (int i = 0; i < n; ++i) {
        to[i] = from[i];
    }
}


void InitializeAndCopyArray(int n, int *from, int **to) {
    *to = malloc(n * sizeof(int));

    for (int i = 0; i < n; ++i) {
        (*to)[i] = from[i];
    }
}

void InitializeSimpleArray(int n, int array[]) {
    for (int i = 0; i < n; ++i) {
        array[i] = 0;
    }
}

void InvertCopyArray(int n, int from[], int to[]) {
    from[0] = 0;
    for (int i = 0; i < n; ++i) {
        to[from[i]] = i;
    }
}