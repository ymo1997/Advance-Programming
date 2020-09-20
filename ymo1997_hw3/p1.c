#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

// Computes the 2D matrix product C = AB, with matrix dimensions N x N
void matrix_product_serial( int N, int ** C, int ** A, int ** B ){
	int i, j, sum;
	for(i = 0; i < N; i++){
		sum = 0;
		for(j = 0; j < N; j++){
			sum += (A[i][j] * B[j][i]);
		}
		C[0][i] = sum; 
	}
}

int main(){
	int n = 1000;
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
	matrix_product_serial(n, C, A, B);
	double end = omp_get_wtime();
	printf("matrix_product_serial runtime: %lf\n", end - start);
	// for(i = 0; i < n; i++){
	// 	printf("%d \n", C[0][i]);
	// }
	free(A);
	free(B);
	free(C);
}
