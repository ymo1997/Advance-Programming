#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <omp.h>

// Computes the 2D matrix product C = AB, with matrix dimensions N x N
// using nthreads number of OpenMP threads
void matrix_product_openmp(int N, int ** C, int ** A, int ** B, int nthreads){
	omp_set_dynamic(0);
	omp_set_num_threads(nthreads);
	#pragma omp parallel
	{
	int id = omp_get_thread_num();
    int Nthr = omp_get_num_threads();

    int istart = id * N / Nthr;
    int iend = (id + 1) * N / Nthr;

	for(int i = istart; i < iend; i++){
		for(int j = 0; j < N; j++){
			C[0][i] += (A[i][j] * B[j][i]);
		}
	}

	}
}

int main(){
	int n = 1000;
	int nthreads = 4;
	int **A, **B, **C;
	int i, j;
	A = (int**)malloc(n * sizeof(int*));
	B = (int**)malloc(n * sizeof(int*));
	C = (int**)malloc(1 * sizeof(int*));
	C[0] = (int*)calloc(n, sizeof(int));

	for(i = 0; i < n; i++){
		A[i] = (int*)malloc(n * sizeof(int));
		B[i] = (int*)malloc(n * sizeof(int));
		for(j = 0; j < n; j++){
			A[i][j] = rand() % 100;
			B[i][j] = rand() % 100;
		}
	}

	double start = omp_get_wtime();
	matrix_product_openmp(n, C, A, B, nthreads);
	double end = omp_get_wtime();
	printf("matrix_product_openmp runtime: %lf\n", end - start);
	// for(i = 0; i < n; i++){
	// 	printf("%d \n", C[0][i]);
	// }
	free(A);
	free(B);
	free(C);
}
