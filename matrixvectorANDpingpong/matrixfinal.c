#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mpi.h>
#define N 4

int *createMatrix (int nrows, int ncols) {
    int *matrix;
    int h, i, j;

    if (( matrix = malloc(nrows*ncols*sizeof(int))) == NULL) {
        printf("Malloc error");
        exit(1);
    }

    for (h=0; h<nrows*ncols; h++) {
        matrix[h] = h+1;
    }

    return matrix;
}

int *createVector (int nrows, int ncols){
    int *vector;
    int h,i,j;

    if (( vector = malloc(nrows*ncols*sizeof(int))) == NULL) {
        printf("Malloc error");
        exit(1);
    }

    for (h=0; h<nrows*ncols; h++) {
        vector[h] = h+1;
    }

    return vector; 
}

void printArray (int *row, int nElements) {
    int i;
    for (i=0; i<nElements; i++) {
        printf("%d ", row[i]);
    }
    printf("\n");
}

int main (int argc, char **argv) {

    if (MPI_Init(&argc, &argv) != MPI_SUCCESS) {
        perror("Error initializing MPI");
        exit(1);
    }

    int p, id,k,sum=0, shu =0;
    MPI_Comm_size(MPI_COMM_WORLD, &p); // Get number of processes
    MPI_Comm_rank(MPI_COMM_WORLD, &id); // Get own ID

    int *matrix, *vector;

    if (id == 0) {
        matrix = createMatrix(N, N); // Master process creates matrix
        printf("Initial matrix:\n");
        printArray(matrix, N*N);
        vector = createVector(N,1);
        printf("Initial vector:\n");
        printArray(vector,N*1);


    }

    int *procRow = malloc(sizeof(int) * N); // received row will contain p integers
    if (procRow == NULL) {
        perror("Error in malloc 3");
        exit(1);
    }

    if (MPI_Scatter(matrix, N, MPI_INT, // send one row, which contains p integers
                procRow, N, MPI_INT, // receive one row, which contains p integers
                0, MPI_COMM_WORLD) != MPI_SUCCESS) {

        perror("Scatter error");
        exit(1);
    }
    MPI_Bcast(vector,N,MPI_INT,0, MPI_COMM_WORLD);
    for (k=0;k < N;k++){
        procRow[k] = procRow[k]*vector[k];
    }
    

    for (k=0;k < p;k++){
        
        sum = sum+procRow[k];

    } 
    procRow[0] = sum;


    // for (shu;shu<p;shu++){
    //     if (id == shu){
    //         printf("The processor is %d, the first element in array is %d \n", id, procRow[0]);
    // }
    // }
    
    


     MPI_Gather(&procRow[0], 1, MPI_INT, vector, 1, MPI_INT, 0, MPI_COMM_WORLD);
     if (id ==0){
         printArray(vector,N);

     }
    
    MPI_Finalize();

    return 0;
}