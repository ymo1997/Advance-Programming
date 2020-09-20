#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <omp.h>

// Computes an N x N julia set, storing the results in the pixels matrix 
// Using nthreads number of OpenMP threads
// (pixel matrix allocated by caller, not within function)
void julia_set_openmp( int N, int ** pixels, int nthreads ){
	double cx = -0.7;
	double cy = 0.26;
	double deltaX = 3.0 / N;
	double deltaY = 3.0 / N;
	
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
			double zx = -1.5 + deltaX * i;
			double zy = -1.0 + deltaY * j;
			int iteration = 0;
			int max = 1000;
			while(zx * zx + zy * zy < 4.0 && iteration <= max){
				double temp = zx * zx - zy * zy;
				zy = 2 * zx * zy + cy;
				zx = temp + cx;
				iteration++;
			}
			pixels[i][j] = iteration;
		}
	}

	}
}


// Saves an integer matrix of size N x N to file
void save_integer_matrix(int N, int ** pixels, char * fname){
	FILE *fp;
	fp = fopen(fname, "w");
	for(int i = 0; i < N; i++){
		for(int j = 0; j < N; j++){
			fputc('0' + pixels[i][j], fp);
		}
		fputc('\n', fp);
	}
	fclose(fp);
}

int main(){
	int N = 1000;
	int nthreads = 4;
	double start, end;
	int** pixels = (int**)malloc(N * sizeof(int*));
	for(int i = 0; i < N; i++){
		pixels[i] = (int*)malloc(N * sizeof(int));
	}

	start = omp_get_wtime();
	julia_set_openmp(N, pixels, nthreads);
	end = omp_get_wtime();
	printf("julia_set_openmp runtime: %lf\n", end - start);
	
	save_integer_matrix(N, pixels, "julia_openmd.txt");
	// for(int i = 0; i < N; i++){
	// 	for(int j = 0; j < N; j++){
	// 		printf("%d", pixels[i][j]);
	// 	}
	// 	printf("\n");
	// }
	
	free(pixels);
}