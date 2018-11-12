//
// Created by tomas on 18.11.10.
//

#include <stddef.h>
#include <stdlib.h>
#include <omp.h>
#include <stdio.h>
#include "DataStructure.h"

static stack_node* head;

static omp_lock_t lock;
//static omp_lock_t* workFlags;
//static int numberOfThreads;

void initStackParallel() {
    head = NULL;

//    numberOfThreads = omp_get_num_threads();
//    workFlags = (omp_lock_t*)malloc(sizeof(omp_lock_t) * numberOfThreads);
//    for (int i = 0; i < numberOfThreads; ++i) {
//        omp_init_lock(&workFlags[i]);
//    }

    omp_init_lock(&lock);
}

void pushParallel(stack_data data) {
    omp_set_lock(&lock);


    stack_node *tmp = (stack_node *) malloc(sizeof(stack_node));
    if (tmp == NULL) {
        printf("Something wrong: 111");
        exit(0);
    }

    tmp->data = data;
    tmp->next = head;
    head = tmp;

    omp_unset_lock(&lock);
}

int popParallelAndFlagWorking(stack_data *element) {

    omp_set_lock(&lock);

    if (head == NULL) {
        omp_unset_lock(&lock);
        return 0;
    }

    stack_node *tmp = head;
    *element = head->data;
    head = head->next;
    free(tmp);

    omp_unset_lock(&lock);
    return 1;
}

int isEmptyParallel() {
    return head == NULL ? 1 : 0;
}

int isWorkingFlagged() {
    return 0;
}

void unflagWorking() {

}