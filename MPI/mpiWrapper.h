#ifndef TRAVELINGSALESMAN_MPI_H
#define TRAVELINGSALESMAN_MPI_H

enum { MPI_PROBLEM_TAG = 11, MPI_BOUND_TAG = 22 };

void MPI_Wrapper_Init(int problemSize, int *size, int *rank);
void MPI_Wrapper_Finalize();
void MPI_Wrapper_Send_Problem(stack_data node, int destination);
stack_data MPI_Wrapper_Receive_Problem();
void MPI_Wrapper_Share_Bound(stack_data bound);
void MPI_Wrapper_Receive_Bound(stack_data *bound);
void MPI_Wrapper_Synchronize(stack_data *bound);

#endif //TRAVELINGSALESMAN_MPI_H
