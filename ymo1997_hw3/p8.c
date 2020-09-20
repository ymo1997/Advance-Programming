#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <omp.h>

static int N;
static int** pixels;
static int nthreads;
static int nthreadsArr[5] = {1, 2, 4, 8, 16};
static int nthreadsArrSize = 5;


// Computes an N x N julia set, storing the results in the pixels matrix // Using nthreads number of pthreads
// (pixel matrix allocated by caller, not within function)
void* helper(void* m){
	int temp = *((int*) m);
	double cx = -0.7;
	double cy = 0.26;
	double deltaX = 3.0 / N;
	double deltaY = 3.0 / N;
	int taskPerThread = N / nthreads;
	int left = temp * taskPerThread;
	int right = (temp + 1) * taskPerThread;
	for(int i = left; i < right; i++){
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
	return NULL;
}

void julia_set_pthreads( int N, int ** pixels, int nthreads){
	int i;
	int* temp = NULL;
	pthread_t* threads = (pthread_t*)malloc(nthreads * sizeof(pthread_t));
	for(i = 0; i < nthreads; i++){
		temp = (int*)malloc(sizeof(int));
		*temp = i;
		pthread_create(&threads[i], NULL, &helper, temp);
	}
	for(i = 0; i < nthreads; i++){
		pthread_join(threads[i], NULL);
	}
	free(temp);
	free(threads);
}

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

int main(){
	N = 1000;
	pixels = (int**)malloc(N * sizeof(int*));
	double start, end;
	for(int i = 0; i < N; i++){
		pixels[i] = (int*)malloc(N * sizeof(int));
	}
	for(int i = 0; i < nthreadsArrSize; i++){
		nthreads = nthreadsArr[i];
		printf("#threads : %d \n", nthreads);
		start = omp_get_wtime();
		julia_set_pthreads(N, pixels, nthreads);
		end = omp_get_wtime();
		printf("julia_set_pthreads runtime: %lf\n", end - start);

		start = omp_get_wtime();
		julia_set_openmp(N, pixels, nthreads);
		end = omp_get_wtime();
		printf("julia_set_openmp runtime: %lf\n", end - start);
	}
	

	// for(int i = 0; i < N; i++){
	// 	for(int j = 0; j < N; j++){
	// 		printf("%d", pixels[i][j]);
	// 	}
	// 	printf("\n");
	// }
	
	free(pixels);
}