#include<stdio.h>
#include<stdlib.h>
#define DIGIT 100
typedef struct{
	float ** arrays;
	int m;   /* Number of arrays */
	int * n; /* Length of each array */
	/* Add Additional Fields Here */
	int ** positions;
} Grid;

void improved_grid_search( float p, Grid grid, int * results )
{
	/* Your Code Here */
	for(int i = 0; i < grid.m; i++){
		int pos = p * DIGIT;
		results[i] = grid.positions[i][pos];
	}

}

void init_improved_grid_search( Grid * grid )
{
	int ** a = (int**)malloc(sizeof(int*) * grid->m);
	/* Your Code Here */
	for(int i = 0; i < grid->m; i++){
	    a[i] = (int*) malloc(sizeof(int) * (DIGIT + 1));
		for(int j = 1; j < grid->n[i]; j++){
			int lowerBound = grid->arrays[i][j - 1] * 100;
			int upperBound = grid->arrays[i][j] * 100;
			for(int k = lowerBound; k < upperBound; k++){
				a[i][k] = j - 1;
			}
		}
		a[i][DIGIT] = grid->n[i] - 1;
	}
	grid->positions = a;
}

/* Add any additional functions you need */
/* Add any additional functions you need */
/* Add any additional functions you need */

/* Builds and returns a small test grid */
Grid * build_test_grid(void)
{
	Grid * grid = (Grid *) malloc(sizeof(Grid));
	grid->m = 5;
	grid->n = (int *) malloc( grid->m * sizeof(int));

	float ** a = (float **) malloc( grid->m * sizeof(float *));
	grid->n[0] = 6; grid->n[1] = 7; grid->n[2] = 4; grid->n[3] = 6; grid->n[4] = 8;
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
	init_improved_grid_search( grid );
	int * results = (int *) malloc( grid->m * sizeof(int));

	/* First test */
	float p = 0.23;
	improved_grid_search(p, *grid, results);
	printf("Test p = %.2f\n", p);
	printf("Expected: 1 1 1 2 0\n");
	printf("Results : ");
	for( int i = 0; i < grid->m; i++ )
		printf("%d ", results[i]);
	printf("\n");

	/* Second test */
	p = 0.94;
	improved_grid_search(p, *grid, results);
	printf("Test p = %.2f\n", p);
	printf("Expected: 4 5 2 4 3\n");
	printf("Results : ");
	for( int i = 0; i < grid->m; i++ )
		printf("%d ", results[i]);
	printf("\n");

	/* Third test */
	p = 0.54;
	improved_grid_search(p, *grid, results);
	printf("Test p = %.2f\n", p);
	printf("Expected: 3 2 1 3 1\n");
	printf("Results : ");
	for( int i = 0; i < grid->m; i++ )
		printf("%d ", results[i]);
	printf("\n");
    free(grid->positions);
	return 0;
}
