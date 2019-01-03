#include <stdio.h>
#include <omp.h>
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

    double start = omp_get_wtime();
    stack_data solution = hybridBranchAndBound(params, bestHeuristics);
    double duration = omp_get_wtime() - start;

    printf("It took %f seconds for algorithm.\n", duration);

    PrintResult(params, solution);
    //WriteResultToPythonFile(params.cities, solution.visited, dataFileName);

    return 0;
}