#include<stdio.h>
#include<stdlib.h>

double** dmatrix_non_contiguous(size_t m, size_t n){
	int i, j;
	double count = 0.0;
	double** tab;
	tab = (double**)malloc(sizeof(double*) * m);/*Do the overall malloc for the whole matrix*/
	for(i = 0; i < m; i++){
		tab[i] = (double*)malloc(sizeof(double) * n);/*Do the malloc for each row*/
		for(j = 0; j < n; j++){
			tab[i][j] = count;/*Assign value*/
			count++;
		}
	}

	return tab;
}


int main(int agrc, char* agrv[]){
	int n = atoi(agrv[1]);/*Get the parameter*/
	int m = n;
	int i, j;
	double** result = dmatrix_non_contiguous(m, n);
	for(i = 0; i < m; i++){/*print out the matrix*/
		for(j = 0; j < n; j++){
			printf("%.1f\t", result[i][j]);

		}
		printf("\n");
	}
	free(result);/*free memory*/
}
