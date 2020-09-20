#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#define M 1000
#define N 1000
/*work_kernels that is provided*/
void work_kernel_dynamic(double** U, int m, int n)
{
    int i, j;
    double a = 1.0, b = 0.5, c;
    for(i=1; i < (m-1); i++) {
        for(j=1; j < (n-1); j++) {
            c = (a * (U[i+1][j] + U[i-1][j])) + (b * (U[i][j+1] + U[i][j-1])) + ((1 - (2*a) - (2*b)) * U[i][j]);
        } 
    }
    return; 
}

void work_kernel_static(double U[M][N], int m, int n)
{
    int i, j;
    double a = 1.0, b = 0.5, c;
    for(i=1; i < (m-1); i++) {
        for(j=1; j < (n-1); j++) {
            c = (a * (U[i+1][j] + U[i-1][j])) + (b * (U[i][j+1] + U[i][j-1])) + ((1 - (2*a) - (2*b)) * U[i][j]);
        }
    }
    return ;
}
/*initialize the matirx in different ways*/
double** dmatrix_contiguous(size_t m, size_t n){
    int i, j;
    double* tab;
    double** starts;
    double* current;
    tab = (double*)malloc(sizeof(double) * m * n);/*malloc the whole matirx of double*/
    starts = (double**)malloc(sizeof(double*) * m);/*malloc the address the starts of row*/
    current = tab;
    for(i = 0; i < m; i++){
        starts[i] = current;
        for(j = 0; j < n; j++){
            starts[i][j] = (rand() % 100) / 100;/*Assign value*/
            current++;
        }
    }
    return starts;
}

double** dmatrix_non_contiguous(size_t m, size_t n){
    int i, j;
    double** tab;
    tab = (double**)malloc(sizeof(double*) * m);/*Do the overall malloc for the whole matrix*/
    for(i = 0; i < m; i++){
        tab[i] = (double*)malloc(sizeof(double) * n);/*Do the malloc for each row*/
        for(j = 0; j < n; j++){
            tab[i][j] = (rand() % 100) / 100;/*Assign value*/
        }
    }
    return tab;
}

static double tab_static[M][N];/*declare the static matrix globally*/
void dmatrix_static(size_t m, size_t n){
    int i, j;
    for(i = 0; i < M; i++){
        for(j = 0; j < N; j++)
            tab_static[i][j] = (rand() % 100) / 100;/* assign value;*/
    }
}

int main(int argc, char* argv[])
{
    /* Declare and allocate your arrays A,B,C */
    /* Initialize arrays */
    double** B = dmatrix_non_contiguous(M, N);
    double** C = dmatrix_contiguous(M, N);
    clock_t start,finish;
    double timeA;
    double timeB;
    double timeC;

    start = clock();/*start to calculate the time.*/
    /* time A */
    work_kernel_static(tab_static, M, N);
    finish = clock();/*end calculating the time.*/
    timeA = (double)(finish-start)/CLOCKS_PER_SEC;

    start = clock();/*start to calculate the time.*/
    /* time B */
    work_kernel_dynamic(B, M, N);
    finish = clock(); /*end calculating the time.*/
    timeB = (double)(finish-start)/CLOCKS_PER_SEC;

    start = clock();/*start to calculate the time.*/
    /* time C */
    work_kernel_dynamic(C, M, N);
    finish = clock(); /*end calculating the time.*/
    timeC = (double)(finish-start)/CLOCKS_PER_SEC;
    /* Print out timing to stdout in seconds for each array */
    printf("The operation on the static matrix has run for \t %.4f seconds.\n", timeA);
    printf("The operation on the non-contiguous matrix has run for \t %.4f seconds.\n", timeB);
    printf("The operation on the contiguous matrix has run for \t %.4f seconds.\n", timeC);

    /* Free memory*/
    free(B);
    free(C);
    return 0; 
}
