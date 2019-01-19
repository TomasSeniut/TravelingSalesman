#ifndef TRAVELINGSALESMAN_LINKEDSTACK_H
#define TRAVELINGSALESMAN_LINKEDSTACK_H

#include "DataStructure.h"

int isEmpty(stack_node *s);
stack_node* push(stack_node *s, stack_data data);
stack_node* pop(stack_node *s, stack_data *data);
void init(stack_node* s);

#endif //TRAVELINGSALESMAN_LINKEDSTACK_H
