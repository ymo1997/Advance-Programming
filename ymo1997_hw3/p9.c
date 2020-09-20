#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <omp.h>

#define TEST_TIME 10000000
#define R 10
#define THREADNUM 16

double count;

int inRange(double x, double y){
	double dis = x * x + y * y;
	if(dis <= R * R) return 1;
	return 0;
}

void pi_serial(){
	srand(time(NULL));
	for(int i = 0; i < TEST_TIME; i++){
		double a = (rand() % 101) / 10;
		double b = (rand() % 101) / 10;
		if(inRange(a, b)) count++;
	}
}

void pi_atomic(int nthreads){
	srand(time(NULL));
	omp_set_dynamic(0);
	omp_set_num_threads(nthreads);
	#pragma omp parallel for
	for(int i = 0; i < TEST_TIME; i++){
		double a = (rand() % 101) / 10;
		double b = (rand() % 101) / 10;
		if(inRange(a, b)) {
			#pragma omp atomic
			count++;
		}
	}
}

void pi_critical(int nthreads){
	srand(time(NULL));
	omp_set_dynamic(0);
	omp_set_num_threads(nthreads);
	#pragma omp parallel for
	for(int i = 0; i < TEST_TIME; i++){
		double a = (rand() % 101) / 10;
		double b = (rand() % 101) / 10;
		if(inRange(a, b)) {
			#pragma omp critical
			{
			count++;
			}
		}
	}
}

void pi_manualA(int nthreads){
	srand(time(NULL));
	omp_set_dynamic(0);
	omp_set_num_threads(nthreads);

	int* countSep = (int*)calloc(nthreads, sizeof(int));

	#pragma omp parallel
	{
	int id = omp_get_thread_num();
  	int Nthr = omp_get_num_threads();

	for(int i = 0; i < TEST_TIME / Nthr; i++){
		double a = (rand() % 101) / 10;
		double b = (rand() % 101) / 10;
		if(inRange(a, b)) {
			countSep[id]++;
		}
	}
	}
	for(int i = 0; i < nthreads; i++){
		count += countSep[i];
	}
	free(countSep);
}

void pi_manualB(int nthreads){
	srand(time(NULL));
	omp_set_dynamic(0);
	omp_set_num_threads(nthreads);

	int countSep = 0;

	#pragma omp parallel
	{
	// int id = omp_get_thread_num();
  	int Nthr = omp_get_num_threads();
  	#pragma omp parallel for lastprivate(countSep) firstprivate(countSep)
	for(int i = 0; i < TEST_TIME / Nthr; i++){
		double a = (rand() % 101) / 10;
		double b = (rand() % 101) / 10;
		if(inRange(a, b)) {
			countSep++;
		}
	}
	count += countSep;
	}
}

void pi_auto(int nthreads){
	srand(time(NULL));
	omp_set_dynamic(0);
	omp_set_num_threads(nthreads);

  	#pragma omp parallel for reduction(+:count)
	for(int i = 0; i < TEST_TIME; i++){
		double a = (rand() % 101) / 10;
		double b = (rand() % 101) / 10;
		if(inRange(a, b)) {
			count++;
		}
	}
}


int main(){

	double start, end;
	count = 0;
	start = omp_get_wtime();
	pi_serial();
	double pi_s = count / TEST_TIME * 4;
	end = omp_get_wtime();
	printf("pi_serial runtime: \t%lf \tresult : \t%lf \n", end - start, pi_s);

	count = 0;
	start = omp_get_wtime();
	pi_atomic(THREADNUM);
	double pi_a = count / TEST_TIME * 4;
	end = omp_get_wtime();
	printf("pi_atomic runtime: \t%lf \tresult : \t%lf \n", end - start, pi_a);

	count = 0;
	start = omp_get_wtime();
	pi_atomic(THREADNUM);
	double pi_c = count / TEST_TIME * 4;
	end = omp_get_wtime();
	printf("pi_critical runtime: \t%lf \tresult : \t%lf \n", end - start, pi_c);

	count = 0;
	start = omp_get_wtime();
	pi_manualA(THREADNUM);
	double pi_ma = count / TEST_TIME * 4;
	end = omp_get_wtime();
	printf("pi_manualA runtime: \t%lf \tresult : \t%lf \n", end - start, pi_ma);

	count = 0;
	start = omp_get_wtime();
	pi_manualB(THREADNUM);
	double pi_mb = count / TEST_TIME * 4;
	end = omp_get_wtime();
	printf("pi_manualB runtime: \t%lf \tresult : \t%lf \n", end - start, pi_mb);

	count = 0;
	start = omp_get_wtime();
	pi_auto(THREADNUM);
	double pi_auto = count / TEST_TIME * 4;
	end = omp_get_wtime();
	printf("pi_auto runtime: \t%lf \tresult : \t%lf \n", end - start, pi_auto);

}