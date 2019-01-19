#ifndef TRAVELINGSALESMAN_IO_H
#define TRAVELINGSALESMAN_IO_H

#include "../DataStructure/DataStructure.h"

tsp_global ReadGlobalData(const char *fileName);
void PrintResult(tsp_global params, stack_data solution);
void WriteResultToPythonFile(int n, int visitedCities[], char *dataFileName);

#endif //TRAVELINGSALESMAN_IO_H
