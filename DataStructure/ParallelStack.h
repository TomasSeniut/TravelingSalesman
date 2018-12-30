//
// Created by tomas on 18.11.11.
//

#ifndef TRAVELINGSALESMAN_PARALLELSTACK_H
#define TRAVELINGSALESMAN_PARALLELSTACK_H

#include "DataStructure.h"

int isEmptyParallel();
void pushParallel(stack_data data);
int popParallel(stack_data *data);
void initStackParallel();
void destroyStack();
int stackSize();

#endif //TRAVELINGSALESMAN_PARALLELSTACK_H

