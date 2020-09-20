#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/stat.h>
#include <string.h>
#include <fcntl.h>
#define MAX_LINES 1000
#define MAX_CHAR_PER_LINE 1000

char* text[MAX_LINES];
void swap(char* text[], int i, int j){/*swap two lines*/
	char* temp = text[i];
	text[i] = text[j];
	text[j] = temp;
}

void alphabetize(char *text[], int nlines){
	int i, j;
	int a, b;
	int pos;
	for(i = 0; i < nlines; i++){
		for(j = 1; j < nlines - i; j++){
			if(text[j - 1][0] > text[j][0]){/*compare the first char*/
				swap(text, j - 1, j);
			}
			else if(text[j - 1][0] == text[j][0]){/*compare the following char*/
				a = (int)text[j - 1][0];
				b = (int)text[j][0];
				pos = 0;
				while(a == b && text[j - 1][pos] != '\n' && text[j][pos] != '\n'){
					a += (int)text[j - 1][pos];
					b += (int)text[j][pos];
					pos++;
					/*printf("%d %d %d %s%s\n", pos, a, b, text[j - 1], text[j]);*/
				}
				if(a == b && text[j][pos] == '\n') swap(text, j, j - 1);
				if(a > b) swap(text, j, j - 1);
			}
		}
	}
}

int read_file(char* fileName){
	FILE *f;
	char* line;
	size_t size = 0;
	ssize_t readSize = 0;
	char* directory = getcwd(NULL, 0);
	int num = 0, i;
	char* finalD = strcat(strcat(directory, "/"), fileName);/*final path of the file*/
	f = fopen(finalD, "r");/*open file*/
	if(!f){
		printf("Read file failed.\n");
		return 0;
	}
	while((readSize = getline(&line, &size, f)) != -1){
		text[num] = (char*)malloc(sizeof(char) * 1000);
		for(i = 0; i < readSize; i++){/*Get everyline*/
			text[num][i] = line[i];/*copy line to the text*/
		}
		num++;
	}
	
	fclose(f);
	free(line);

	return num;
}


int main(int argc, char* argv[]){
	int lineNums, i;
	lineNums = read_file(argv[1]);
	printf("");
	printf("");
	alphabetize(text, lineNums);/*sorting*/
	
	for(i = 0; i < lineNums; i++){/*print out the result*/
		printf("%s", text[i]);
	}
	

}
