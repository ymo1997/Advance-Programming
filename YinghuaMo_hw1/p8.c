#include <stdio.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <fcntl.h>
#define MAX_LENGTH 50
#define MAX_NUM 2000
#define TOTAL 100000

char text[TOTAL];
char word[MAX_NUM][MAX_LENGTH];
int wordTimes[MAX_NUM];

int read_file(char* fileName){
	FILE *f;
	char* line;
	size_t size = 0;
	ssize_t readSize = 0;
	char* directory = getcwd(NULL, 0);/*get the current directory*/
	int num = 0, i;
	char* finalD = strcat(strcat(directory, "/"), fileName);/*get the path of the file*/

	line = (char*)malloc(10000);
	f = fopen(finalD, "r");/*open file*/
	if(!f){
		printf("Read file failed.\n");
		return 0;
	}
	while((readSize = getline(&line, &size, f)) != -1){
		for(i = 0; i < readSize; i++){/*Get everyline*/
			text[num] = line[i];/*copy into the text*/
			num++;
		}
	}
	fclose(f);/*close*/
	free(line);
	return num;
}

char thisWord[MAX_LENGTH];

int isSame(int lastPos, int wordNum){
	int i, j;
	for(j = 0; j < lastPos; j++){
		if(thisWord[j] < 'Z' && thisWord[j] > 'A'){/*covert the upper case before comparing*/
			thisWord[j] += 32;
		}
	}
	for(i = 0; i < wordNum; i++){
		for(j = 0; j < lastPos; j++){
			if(thisWord[j] != word[i][j]){
				break;/*if something diffrent, break*/
			}
		}
		if(j == lastPos) return i;/*all the things are the same, return the position of word int the result*/
	}
	return -1;
}

int count(int nums){
	int lastPos = 0;
	int i, k;
	int wordPos;
	int wordNum = 0;
	for(i = 0; i < nums; i++){
		if(text[i] == ' ' || text[i] == '.' || text[i] == '!' || text[i] == '\n' ||
		 text[i] == ',' || text[i] == '?' || text[i] == ';' || text[i] == ':'){
			/*split the word*/
			thisWord[lastPos] = 0;
			lastPos++;

			wordPos = isSame(lastPos, wordNum);/*compare to the existing word*/
			if(lastPos > 0){
				if(wordPos == -1){
					for(k = 0; k < lastPos; k++){
						word[wordNum][k] = thisWord[k];
					}
					wordTimes[wordNum] = 1;/*new a word in the final result*/
					wordNum++;
				}
				else{
					wordTimes[wordPos]++;/*increase the time if exists*/
				}
			}
			lastPos = 0;/*reset to zero*/
		}
		else{/*keep finding the next char of the word*/
			thisWord[lastPos] = text[i];
			lastPos++;
		}
	}
	return wordNum;
}

void swap(int i, int j){
	int k;
	char temp[MAX_LENGTH];
	int tempInt = wordTimes[i];
	for(k = 0; k < MAX_LENGTH; k++){/*swap the word*/
		temp[k] = word[i][k];
		word[i][k] = word[j][k];
		word[j][k] = temp[k];
	}

	wordTimes[i] = wordTimes[j];/*swap the time*/
	wordTimes[j] = tempInt;	
}

void alphabetize(int nlines){
	int i, j;
	int ret;
	for(i = 0; i < nlines; i++){
		for(j = 1; j < nlines - i; j++){
			ret = strcmp(word[j - 1], word[j]);/*concat string*/
			if(ret > 0){
				swap(j - 1, j);/*swap position*/
			}
		}
	}
}


int main(int argc, char* argv[]){
	int i, j;
	int nums = read_file(argv[1]);/*read_file*/
	int wordNum = count(nums);/*count the words*/
	alphabetize(wordNum);/*sorting*/
	for(i = 0; i < wordNum; i++){/*print out the final result*/
		for(j = 0; word[i][j]; j++){
            printf("%c", word[i][j]);
		}
		printf(" : %d\n", wordTimes[i]);
	}
}
