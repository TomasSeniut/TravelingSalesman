//
// Created by tomas on 18.9.30.
//

#ifndef TRAVELINGSALESMAN_UTILS_H
#define TRAVELINGSALESMAN_UTILS_H

#include "DataStructure/DataStructure.h"

void PopulateDistanceMatrix(int n, City *array, double *distanceMatrix[]);
void InitializeArray(int n, int ** array);
void InitializeSimpleArray(int n, int array[]);
void InitializeAndCopyArray(int n, int *from, int **to);
void CopyArray(int n, int from[], int to[]);
void InvertCopyArray(int n, int from[], int to[]);

#endif //TRAVELINGSALESMAN_UTILS_H
