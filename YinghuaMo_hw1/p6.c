#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/stat.h>
#include <string.h>
#include <fcntl.h>
#define MAX_LINES 1000
#define MAX_CHAR_PER_LINE 1000

char text[MAX_LINES][MAX_CHAR_PER_LINE];
int read_file(char* fileName){
	FILE *f;
	char* line;
	size_t size = 0;
	ssize_t readSize = 0;
	char* directory = getcwd(NULL, 0);/*Get directory of so far*/
	int num = 0, i;
	char* finalD = strcat(strcat(directory, "/"), fileName);/*Get the whole directory*/
	f = fopen(finalD, "r");/*open file*/
	if(!f){
		printf("Read file failed.\n");
		return 0;
	}
	while((readSize = getline(&line, &size, f)) != -1){
		for(i = 0; i < readSize; i++){/*Get everyline*/
			text[num][i] = line[i];
		} 
		num++;
	}
	fclose(f);/*close file*/
	free(line);/*free space*/
	return num;
}


int main(int argc, char* argv[]){
	int i, j;
	int num = read_file(argv[1]);
	for(i = 0; i < num; i++){
		for(j = 0; j < MAX_CHAR_PER_LINE; j++){/*print out the char[MAXLINES][MAXCHARPERLINE]*/
			printf("%c", text[i][j]);
			if(j == MAX_CHAR_PER_LINE - 1) printf("");
		}

	}

}
