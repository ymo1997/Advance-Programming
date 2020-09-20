#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <omp.h>

#define MAX_LEN 10000
#define MAX_LEN2 100
#define MINUS_ONE 16843009

int nodeNum = 100;
int level;

//basic info
int** VE;
//periods for each node
int* periods;
//mark
int* visited;
int* lastLevel;

char* temp;

//read the vertex and edge info to a 100 * 100 tabel
void readVE(char* markov) {
	VE = (int**)malloc(sizeof(int*) * nodeNum);

	char line[MAX_LEN];
	temp = (char*)malloc(sizeof(char) * MAX_LEN2);
	periods = (int*)malloc(sizeof(int) * nodeNum);
	memset(periods, 1, sizeof(int) * nodeNum);
	visited = (int*)malloc(sizeof(int) * nodeNum);
	lastLevel = (int*)malloc(sizeof(int) * nodeNum);
	memset(visited, 1, sizeof(int) * nodeNum);
	memset(lastLevel, 1, sizeof(int) * nodeNum);

	FILE *fp;
	fp = fopen(markov, "r");
	//every line include the next node info of that node
	while(fgets(line, MAX_LEN, fp) != NULL) {
		// printf("%s", line);
		// if(strncmp(line, "", MAX_LEN) == 0) break;
		int current = atoi(strtok(line, " ")) - 1;
		VE[current] = (int*)malloc(sizeof(int) * nodeNum);
		memset(VE[current], 1, sizeof(int) * nodeNum);
		int count = 0;
		while(strtok(NULL, "[") != NULL) {
			temp = strtok(NULL, ",");
			if(temp == NULL) continue;
			int vNum = atoi(temp) - 1;
			float eNum = atof(strtok(NULL, "]"));
			if(eNum == 0.0) continue;
			VE[current][count++] = vNum;
			// printf("%d\t%lf\n", vNum, eNum);
		}
		
	}
	fclose(fp);
	// for(int i = 0; i < nodeNum; i++) {
	// 	printf("%d:\n", i);
	// 	for(int j = 0; j < nodeNum; j++) {
	// 		if(VE[i][j] == MINUS_ONE) break;
	// 		printf("%d\t", VE[i][j]);
	// 	}
	// 	printf("\n");
	// }
}

void graph_period(int source) {
	visited[source] = 0;
	lastLevel[source] = level;
	for(int i = 0; i < nodeNum; i++) {
		int next = VE[source][i];
		if(next == MINUS_ONE) return ;
		if(visited[next] == 0) {//if visted
			if(periods[next] == MINUS_ONE) {// if met this node before, and we meet it again
				// printf("%d: %d \t%d\n", next, level, lastLevel[next]);
				if(level - lastLevel[next] >= 1) {
					periods[next] = level - lastLevel[next];
				}
				else{
					periods[next] = lastLevel[next] - level;
				}
			}
			continue;
		}
		//recursion, next layer, dfs
		level++;
		graph_period(next);
		level--;
	}
	return ;
}

void freeAll() {
	for(int i = 0; i < nodeNum; i++) free(VE[i]);
	free(VE);
	free(temp);

	free(periods);
	free(visited);
	free(lastLevel);
}

//gcd calculation
int gcd() {
	int a = 0, b = 0, c = 0;
	a = periods[0];
	for(int i = 1; i < nodeNum; i++) {
		b = periods[i];
		c = a % b;
		while(c) {
			a = b;
			b = c;
			c = a % b;
		}
		if(b == 1) return b; //if 1, break immediatly
		a = b;
	}
	return b;
}

int main() {
	//test;
	char markov[] = "markov-graph.txt";

	readVE(markov);

	double start, end;
	start = omp_get_wtime();
	graph_period(0);
	end = omp_get_wtime();
	printf("markov-graph graph_period() runtime: %lf\n", end - start);

	int result = gcd();
	printf("markov-graph graph_period() result:%d\n", result);


	freeAll();
}