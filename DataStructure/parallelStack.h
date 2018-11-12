//
// Created by tomas on 18.11.11.
//

#ifndef TRAVELINGSALESMAN_PARALLELSTACK_H
#define TRAVELINGSALESMAN_PARALLELSTACK_H

#include "DataStructure.h"

int isEmptyParallel();
void pushParallel(stack_data data);
int popParallelAndFlagWorking(stack_data *data);
void initStackParallel();
int isWorkingFlagged();
void destroyStack();

#endif //TRAVELINGSALESMAN_PARALLELSTACK_H
