#include<stdio.h>
#include<stdlib.h>

typedef struct{
	float ** arrays;
	int m;   /* Number of arrays */
	int * n; /* Length of each array */
} Grid;

int binary_search(float *array, float p, int left, int right){
	if(left >= right) return left;
	int mid = left + (right - left) / 2;
	// printf("left: %d %f mid: %d %f right: %d %f \n", left, array[left], mid, array[mid], right, array[right]);
	if(p == array[mid] || (p > array[mid] && p < array[mid + 1])) return mid;
	if(p < array[mid]) return binary_search(array, p, left, mid);
	else return binary_search(array, p, mid + 1, right);
}

void grid_search( float p, Grid grid, int * results )
{
	/* Your Code Here */
	for(int i = 0; i < grid.m; i++){
		results[i] = binary_search(grid.arrays[i], p, 0, grid.n[i] - 1);
	}
	return ;
}

/* Builds and returns a small test grid */
Grid * build_test_grid(void)
{
	Grid * grid = (Grid *) malloc(sizeof(Grid));
	grid->m = 5;
	grid->n = (int *) malloc( grid->m * sizeof(int));
	grid->n[0] = 6; grid->n[1] = 7; grid->n[2] = 4; grid->n[3] = 6; grid->n[4] = 8;

	float ** a = (float **) malloc( grid->m * sizeof(float *)); 
	a[0] = (float *) malloc(6 * sizeof(float));
	a[1] = (float *) malloc(7 * sizeof(float));
	a[2] = (float *) malloc(4 * sizeof(float));
	a[3] = (float *) malloc(6 * sizeof(float));
	a[4] = (float *) malloc(8 * sizeof(float));

	a[0][0] = 0.00; a[0][1] = 0.13; a[0][2] = 0.32; a[0][3] = 0.38; a[0][4] = 0.57; a[0][5] = 1.00;
	a[1][0] = 0.00; a[1][1] = 0.23; a[1][2] = 0.34; a[1][3] = 0.68; a[1][4] = 0.69; a[1][5] = 0.85; a[1][6] = 1.00;
	a[2][0] = 0.00; a[2][1] = 0.03; a[2][2] = 0.67; a[2][3] = 1.00;
	a[3][0] = 0.00; a[3][1] = 0.15; a[3][2] = 0.16; a[3][3] = 0.45; a[3][4] = 0.93; a[3][5] = 1.00;
	a[4][0] = 0.00; a[4][1] = 0.35; a[4][2] = 0.78; a[4][3] = 0.90; a[4][4] = 0.95; a[4][5] = 0.96; a[4][6] = 0.99; a[4][7] = 1.00;

	grid->arrays = a;

	return grid;
}

int main(void)
{
	Grid * grid = build_test_grid();
	int * results = (int *) malloc( grid->m * sizeof(int));

	/* First test */
	float p = 0.23;
	grid_search(p, *grid, results);
	printf("Test p = %.2f\n", p);
	printf("Expected: 1 1 1 2 0\n");
	printf("Results : ");
	for( int i = 0; i < grid->m; i++ )
		printf("%d ", results[i]);
	printf("\n");

	/* Second test */
	p = 0.94;
	grid_search(p, *grid, results);
	printf("Test p = %.2f\n", p);
	printf("Expected: 4 5 2 4 3\n");
	printf("Results : ");
	for( int i = 0; i < grid->m; i++ )
		printf("%d ", results[i]);
	printf("\n");

	/* Third test */
	p = 0.54;
	grid_search(p, *grid, results);
	printf("Test p = %.2f\n", p);
	printf("Expected: 3 2 1 3 1\n");
	printf("Results : ");
	for( int i = 0; i < grid->m; i++ )
		printf("%d ", results[i]);
	printf("\n");

	return 0;
}
