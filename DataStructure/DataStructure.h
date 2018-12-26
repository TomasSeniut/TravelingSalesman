//
// Created by tomas on 18.9.30.
//

#ifndef TRAVELINGSALESMAN_DATASTRUCTURE_H
#define TRAVELINGSALESMAN_DATASTRUCTURE_H

typedef struct {
    int cities;
    double **distanceMatrix;
} tsp_global;

typedef struct {
    int x;
    int y;
} City;

typedef struct {
    int city;
    int step;
    double pathLength;
    int *visited;
} stack_data;

typedef struct stack_node stack_node;
struct stack_node
{
    stack_data data;
    stack_node* next;
};

#endif //TRAVELINGSALESMAN_DATASTRUCTURE_H
