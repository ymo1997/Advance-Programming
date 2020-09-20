#include<stdio.h>
#include<stdlib.h>

double*** d3darr_contiguous(size_t l, size_t m, size_t n){
	int i, j, k;
	double count = 0.0;
	double*** tab;
	double** starts;
	double* current;
	/*malloc the whole space*/
	current = (double*)malloc(sizeof(double) * m * n * l);
	/*malloc the addresses for the every row*/
	starts = (double**)malloc(sizeof(double*) * m * n);
	/*malloc the addresses for the every start of the matrix*/
	tab = (double***)malloc(sizeof(double**) * m);

	for(i = 0; i < m; i++){
		tab[i] = starts;/*Assign address valut to every start of the matrix*/
		for(j = 0; j < n; j++){
			/*Assign address valut to every start of the row*/
			*starts = current;
			for(k = 0; k < l; k++){
				*current = count;/*Assign value*/
				count++;
				current++;
			}
			starts++;
		}
	}
	return tab;
}


int main(int agrc, char* agrv[]){
	int n = atoi(agrv[1]);/*Get the parameter*/
	int m = n, l = n;
	int i, j, k;
	double*** result;
	double* d1;

	result = d3darr_contiguous(m, n, l);
	d1 = result[0][0];
	printf("Matrix with 1D indexing:\n");
	for(i = 0; i < m * n * l; i++){/*print out the space using 1 dimension*/
		printf("%.1f\t", d1[i]);
		if((i + 1) % n == 0) printf("\n");
		if((i + 1) % (n * n) == 0) printf("\n");
	}
	printf("\n");
	printf("Matrix with 3D indexing:\n");
	for(i = 0; i < m; i++){/*print out the matrix*/
		for(j = 0; j < n; j++){
			for(k = 0; k < l; k++){/*print out the space using 3 dimension*/
				printf("%.1f\t", result[i][j][k]);
			}
			printf("\n");
		}
		printf("\n");
	}

	
}
