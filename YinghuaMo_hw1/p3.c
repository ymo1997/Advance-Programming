#include<stdio.h>
#include<stdlib.h>

double** dmatrix_contiguous(size_t m, size_t n){
	int i, j;
	double count = 0.0;
	double* tab;
	double** starts;
	double* current;
	tab = (double*)malloc(sizeof(double) * m * n);/*malloc the whole matirx of double*/
	starts = (double**)malloc(sizeof(double*) * m);/*malloc the address the starts of row*/
	current = tab;
	for(i = 0; i < m; i++){
		starts[i] = current;
		for(j = 0; j < n; j++){
			starts[i][j] = count;/*Assign value*/
			count++;
			current++;
		}
	}
	return starts;
}


int main(int agrc, char* agrv[]){
	int n = atoi(agrv[1]);/*Get the parameter*/
	int m = n;
	int i, j;
	double** result;
	double* d1;
	result = dmatrix_contiguous(m, n);
	d1 = result[0];
	printf("Matrix with 1D indexing:\n");
	for(i = 0; i < m * n; i++){/*print out the matrix using 1D access*/
		printf("%.1f\t", d1[i]);
		if((i + 1) % n == 0) printf("\n");
	}
	printf("\n");
	printf("Matrix with 2D indexing:\n");
	for(i = 0; i < m; i++){/*print out the matrix using 2D access*/
		for(j = 0; j < n; j++){			
			printf("%.1f\t", result[i][j]);
		}
		printf("\n");
	}
	free(result);/*free the space*/


	
}
