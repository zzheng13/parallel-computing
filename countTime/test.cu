/*
 ============================================================================
 Name        : prog4.c:  Program Assignment 4 Serial Code
 Version     :
 Copyright   :
 Description : Gaussian Elimiation Code to solve a system of linear equations
 ============================================================================
 */
#include<time.h>
#include <stdio.h>
#include <stdlib.h>
#include <cuda.h>
#include <cublas.h>
#include <cuda_runtime.h>
#define N 500
void forwardElimination(double mat[N][N+1]);
int forwardElim(double mat[N][N+1]);

//Prints a matrix
void printMat(double mat[N][N+1]);


#include "./inputArray500.txt"


/*-------------------------------------------------------------------
 * Function:    main
 * Purpose:     initializes matrix and vector for gaussian elimination
 * In args:     none
 * Outputs:     None
 *
 */
int main()
{
    clock_t begin =clock();
   // double x[N];  // An array to store solution

    printf("print mat:\n");
    printMat(mat);

    //use forward elimination to create diagonal matrix
    forwardElimination(mat);
    
    double* d_A, * d_B;
    int jj,i, j;

    double A[N][N];
    double B[N];

    //this is where you assign data to the matrixes

    //for loop with a counter
    //start with matrix B and use mat 

    //for loop with another counter
    //start with matrix A and use mat 


    for (i=0; i < N;i++){
        B[i]=mat[i][N];
        for (j=0;j<N;j++){
            A[i][j]=mat[i][j];
        }
    }

  

    
  
    cudaMalloc((void **) &d_A, N*N*sizeof(double));
    cudaMemcpy(d_A, A, N*N*sizeof(double), cudaMemcpyHostToDevice);
    cudaMalloc((void **) &d_B, N*sizeof(double));
    cudaMemcpy(d_B, B, N*sizeof(double), cudaMemcpyHostToDevice);

    cublasDtrsm('l','l','t','n', N, N, 1.0, d_A, N, d_B, N);

    for(jj=0; jj < 4; jj++) B[jj]=0.0;


     


    cudaDeviceSynchronize();
    
    cudaMemcpy(B, d_B, N*sizeof(double), cudaMemcpyDeviceToHost);
    
    cudaFree(d_A);
    
    cudaFree(d_B);
    
    printf("\nSolution to Linear Equations\n");
    for (int row = 0; row < N; row++)
        printf("x[%d] = %f\n", row, B[row]);




    //from textbook programming problem 5.4 method 1
    //Row oriented back substitution to find solution to linear equations
    /*
    for(int row = n-1; row >= 0; row--) {
        x[row] = b[row];
        for(int col = row+1; col < n; col++)
            x[row] -= mat[row][col] * x[col];
        x[row] /= mat[row][row];
    }
    */

    //from textbook programming problem 5.4 method 2
    //Column oriented back substitutionto find solution to linear equations
    
    //printf("\nSolution to Linear Equations\n");
    //for (int row = 0; row < N; row++)
        //printf("x[%d] = %f\n", row, B[row]);
    clock_t end = clock();
    double time_spent = (double) (end-begin)/CLOCKS_PER_SEC;
    printf("The time spent is :%lf\n", time_spent);
    return 0;
}


/*-------------------------------------------------------------------
 * Function:    forwardElimination
 * Purpose:     calculates triangular matrix
 * In args:     matrix derived from system of linear equations
 * Outputs:     triangularized matrix
 *
 */
// function to get matrix content
void forwardElimination(double mat[N][N+1])
{
    /* reduction into r.e.f. */
    int singular_flag = forwardElim(mat);

    /* if matrix is singular */
    if (singular_flag != -1)
    {
        printf("Singular Matrix.\n");

        /* if the RHS of equation corresponding to
           zero row  is 0, * system has infinitely
           many solutions, else inconsistent*/
        if (mat[singular_flag][N])
            printf("Inconsistent System.");
        else
            printf("May have infinitely many "
                   "solutions.");

        exit(1);
    }

}
/*-------------------------------------------------------------------
 * Function:    swap_row
 * Purpose:     function for elemntary operation of swapping two rows
 * In args:     matrix derived from system of linear equations, row numbers i and j
 * Outputs:     matrix with swapped rows
 *
 */

// function for elemntary operation of swapping two rows
void swap_row(double mat[N][N+1], int i, int j)
{
    //printf("Swapped rows %d and %d\n", i, j);

    for (int k=0; k<=N; k++)
    {
        double temp = mat[i][k];
        mat[i][k] = mat[j][k];
        mat[j][k] = temp;
    }
}


/*-------------------------------------------------------------------
 * Function:    printMat
 * Purpose:     function to print matrix content at any stage
 * In args:     matrix derived from system of linear equations
 * Outputs:     none
 *
 */
void printMat(double mat[N][N+1])
{
    for (int i=0; i<N; i++, printf("\n"))
        for (int j=0; j<=N; j++)
            printf("%.1lf ", mat[i][j]);

    printf("\n");
}

/*-------------------------------------------------------------------
 * Function:    forwardElim
 * Purpose:     function to reduce matrix to r.e.f. triangular form
 * In args:     matrix derived from system of linear equations
 * Outputs:     triangularized matrix
 *
 *///
int forwardElim(double mat[N][N+1])
{
    for (int k=0; k<N; k++)
    {
        // Initialize maximum value and index for pivot
        int i_max = k;
        int v_max = mat[i_max][k];

        /* find greater amplitude for pivot if any */
        for (int i = k+1; i < N; i++)
            if (abs(mat[i][k]) > v_max)
                v_max = mat[i][k], i_max = i;

        /* if a prinicipal diagonal element  is zero,
         * it denotes that matrix is singular, and
         * will lead to a division-by-zero later. */
        if (!mat[k][i_max])
            return k; // Matrix is singular

        /* Swap the greatest value row with current row */
        if (i_max != k)
            swap_row(mat, k, i_max);


        for (int i=k+1; i<N; i++)
        {
            /* factor f to set current row kth elemnt to 0,
             * and subsequently remaining kth column to 0 */
            double f = mat[i][k]/mat[k][k];

            /* subtract fth multiple of corresponding kth
               row element*/
            for (int j=k+1; j<=N; j++)
                mat[i][j] -= mat[k][j]*f;

            /* filling lower triangular matrix with zeros*/
            mat[i][k] = 0;
        }

        //print(mat);        //for matrix state
    }
    //print(mat);            //for matrix state
    return -1;
}

