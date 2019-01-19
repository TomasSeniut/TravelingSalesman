#include <stdio.h>
#include <stdlib.h>
#include "../DataStructure/DataStructure.h"
#include "../utils.h"

void ExtractCoordinates(FILE *fp, City cities[]) {
    char buffer[255];
    int i = 0;
    while (!feof (fp)) {
        fgets(buffer, 255, fp);

        if (buffer[0] == '#')
            continue;

        sscanf(buffer, "%*d %d %d\n", &(cities[i].x), &(cities[i].y));
        i++;
    }
}

tsp_global ReadGlobalData(const char *fileName)
{
    FILE *fp;
    fp = fopen(fileName, "r");

    int cities;

    fscanf(fp, "# Cities: %d\n", &cities);

    tsp_global params;
    params.cities = cities;

    City city[params.cities];
    ExtractCoordinates(fp, city);

    fclose(fp);

    params.distanceMatrix  = (double **)malloc(sizeof(double *) * params.cities);
    for (int j = 0; j < params.cities; j++)
        params.distanceMatrix[j] = (double *)malloc(params.cities * sizeof(double));

    PopulateDistanceMatrix(params.cities, city, params.distanceMatrix);

    return params;
}

void PrintResult(tsp_global params, stack_data result)
{
    int solution[params.cities];
    InvertCopyArray(params.cities, result.visited, solution);

    printf("Length of the path found: %10.4f\n", result.pathLength);
    printf("Cities visited in this order:\n");
    for (int i = 0; i < params.cities; ++i) {
        printf("%d ", solution[i]);
    }
    printf("\n");
}

void WriteResultToPythonFile(int n, int visitedCities[], char *dataFileName) {
    int solution[n];
    InvertCopyArray(n, visitedCities, solution);

    FILE* resultsFile = fopen("../Plotting/solution.py", "w");

    fprintf(resultsFile, "solution = [");
    for (int i = 0; i < n - 1; ++i) {
        fprintf(resultsFile, "%d, ", solution[i]);
    }
    fprintf(resultsFile, "%d]\n", solution[n-1]);

    fprintf(resultsFile, "dataFileName = \"%s\"", dataFileName);
}