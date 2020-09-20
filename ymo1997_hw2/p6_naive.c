/* Begin by importing your finished code from problem 4 */
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include <time.h>

#define WORD_LEN 32
#define DEF_LEN 1024
#define COMMAND_LEN 32
#define LINE_LEN 1024


typedef struct tree_node{
	struct tree_node * left;
	struct tree_node * right;
	char word[WORD_LEN];      /* key   */
	char definition[DEF_LEN]; /* value */
} Tree_node;

int add_word(Tree_node ** dictionary, char * word, char * definition)
{
	if(*dictionary == NULL){
		*dictionary = (Tree_node*)calloc(1, sizeof(Tree_node));
//		printf("Just Allocated: %s %x\n", word, *dictionary);
		strncpy((*dictionary)->word, word, strlen(word) + 1);
		strncpy((*dictionary)->definition, definition, strlen(definition) + 1);
		return 0;
	}
	Tree_node* parent = *dictionary;
	Tree_node* current = parent;
	int compareResult;
	while(current != NULL){
		parent = current;
//		printf("Before Comparing: %s left : %x right : %x \n", current->word, current->left, current->right);
//		printf("Comparing: %s %s %x\n", word, current->word, current);
		compareResult = strcmp(word, current->word);

		if(compareResult < 0) current = current->left;
		else if(compareResult > 0) current = current->right;
		else{
			printf("A duplicate key is added!\n");
			return 1;
		}
	}
	if(compareResult < 0) {
		parent->left = (Tree_node*)calloc(1, sizeof(Tree_node));
//		printf("Just Allocated: %s %x\n", word, parent->left);
//		printf("Got Allocated with: %s all: %x left:%x right:%x\n", word, parent->left, parent->left->left, parent->left->right);
		strncpy(parent->left->word, word, strlen(word) + 1);
		strncpy(parent->left->definition, definition, strlen(definition) + 1);
	}
	else {
		parent->right = (Tree_node*)calloc(1, sizeof(Tree_node));
//		printf("Just Allocated: %s %x\n", word, parent->right);
//		printf("Got Allocated with: %s all: %x left:%x right:%x\n", word, parent->right, parent->right->left, parent->right->right);

		strncpy(parent->right->word, word, strlen(word) + 1);
		strncpy(parent->right->definition, definition, strlen(definition) + 1);
	}
	return 0;
}

int find_word(Tree_node * dictionary, char * word)
{
	if(dictionary == NULL) return 1;
	int compareResult = strcmp(word, dictionary->word);
	if(compareResult == 0) {
//	    printf("Find successfully!\n");
//	    printf("%s %s\n", dictionary->word, dictionary->definition);
	    return 0;
	}
	else{
		if(compareResult < 0)
			return find_word(dictionary->left, word);
		else{
			return find_word(dictionary->right, word);
		}
	}
}

int deleted;
Tree_node* findSuccesor(Tree_node* root){
	root = root->right;
    while(root->left) root = root->left;
    return root;
}

Tree_node* search(Tree_node* root, char* word){
	if(root == NULL) return root;

	int compareResult = strcmp(word, root->word);
//	printf("CompareResult : %d word: %s  %s\n", compareResult, word, root->word);
    if(compareResult == 0){
        deleted = 1;
        if(root->left == NULL) {
            return root->right;
        }
        if(root->right == NULL) return root->left;
        Tree_node* successor = findSuccesor(root);
        strncpy(root->word, successor->word, strlen(successor->word) + 1);
        root->right = search(root->right, successor->word);
        return root;
    }
    if(compareResult < 0) root->left = search(root->left, word);
    else root->right = search(root->right, word);
    return root;
}

int delete_word(Tree_node ** dictionary, Tree_node * parent, char * word)
{
	deleted = 0;
	*dictionary = search(parent, word);
	if(deleted == 0){
		printf("Fail to delete.\n");
		return 1;
	}
	printf("Delete successfully! \n");
	return 0;
}

void print_dictionary(Tree_node * dictionary)
{
	if(dictionary == NULL) return;
	print_dictionary(dictionary->left);
	printf("%s %s\n", dictionary->word, dictionary->definition);
	print_dictionary(dictionary->right);
}

void clear(Tree_node ** dictionary){
	Tree_node* current = *dictionary;
    if(current == NULL) return;
	clear(&current->left);
	clear(&current->right);
	free(current);
	*dictionary = NULL;
}

void clear_dictionary(Tree_node ** dictionary)
{
    clear(dictionary);
    printf("Clear successfully!\n");
}

int import_file(char * fname, Tree_node ** dictionary )
{
	FILE *content = fopen(fname, "r");
	char buff[LINE_LEN];
	if(content == NULL) return 1;
	while(NULL != fgets(buff, LINE_LEN, content)){
		char word[WORD_LEN];
		bzero(word, WORD_LEN);
		char definition[DEF_LEN];
		bzero(definition, DEF_LEN);

		char* spacePos = strstr(buff, " ");
		char* startPos = strstr(buff, "\"");
		char* endPos = strrchr(buff, '\"');
		int wordLen = spacePos - buff;
		int definitionLen = endPos - startPos;
		strncpy(word, buff, wordLen);
		strncpy(definition, startPos, definitionLen + 1);

		add_word(dictionary, word, definition);
	}

	fclose(content);
	return 0;
}

int read_words(char** words, char * fname)
{
	FILE *content = fopen(fname, "r");
	char buff[LINE_LEN];
	int i = 0;
	if(content == NULL) return 1;
	while(NULL != fgets(buff, LINE_LEN, content)){
		char word[WORD_LEN];
		bzero(word, WORD_LEN);

		char* spacePos = strstr(buff, " ");

		int wordLen = spacePos - buff;
		strncpy(word, buff, wordLen);
		words[i] = (char*)malloc(sizeof(char) * WORD_LEN);
		strncpy(words[i], buff, wordLen);
		i++;
	}

	fclose(content);
	return i;
}

int main(void)
{
	Tree_node * dictionary = NULL;
	char ** words = (char**)malloc(sizeof(char*) * 100000);

	int error;

	clock_t start,finish;
    double timeA;
   	double timeB;
   	char fname[DEF_LEN] = "alphabetized_dictionary.txt";

    start = clock();/*start to calculate the time.*/
	error = import_file(fname, &dictionary);
  	finish = clock();/*end calculating the time.*/

   	timeA = (double)(finish-start)/CLOCKS_PER_SEC;
    printf("Naive time for import : %lf seconds.\n", timeA);
   	int wordCount = read_words(words, fname);
   	start = clock();
   	int i, j = 0;
   	for(i = 0; i < 100000; i++){
   		find_word(dictionary, words[j]);
   		j++;
   		if(j == wordCount - 1) j = 0;
   	}
   	finish = clock();

   	timeB = (double)(finish-start) * 10 /CLOCKS_PER_SEC;
   	printf("Naive time for finding : %lf microseconds per word\n", timeB);
   	for(int i = 0; i < wordCount; i++) free(words[i]);
   	free(words);

	return 0;
}
