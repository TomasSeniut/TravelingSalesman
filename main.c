#include <stdio.h>
#include "DataStructure/DataStructure.h"
#include "IO/IO.h"
#include "Algorithms/algorithms.h"

int main(int argc, char* argv[]) {

    if (argc != 2) {
        printf("Usage: ./TravelingSalesman problemFileName\n");
        return 1;
    }

    char* dataFileName = argv[1];

    tsp_global params = ReadGlobalData(dataFileName);

    stack_data bestHeuristics = NearestNeighbourMethod(params);

    stack_data solution = simpleBranchAndBound(params, bestHeuristics);

    PrintResult(params, solution);
    WriteResultToPythonFile(params.cities, solution.visited, dataFileName);

    return 0;
}