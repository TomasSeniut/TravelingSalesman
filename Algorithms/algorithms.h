//
// Created by tomas on 18.10.1.
//

#ifndef TRAVELINGSALESMAN_ALGORITHMS_H
#define TRAVELINGSALESMAN_ALGORITHMS_H

#include "../DataStructure/DataStructure.h"

stack_data NearestNeighbourMethod(tsp_global params);
stack_data simpleBranchAndBound(tsp_global params, stack_data bestKnown);

#endif //TRAVELINGSALESMAN_ALGORITHMS_H
