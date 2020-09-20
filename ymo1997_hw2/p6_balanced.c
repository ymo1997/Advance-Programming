/* Begin by importing your code from Problem 5 */
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include <time.h>

#define WORD_LEN 32
#define DEF_LEN 1024
#define COMMAND_LEN 32
#define LINE_LEN 1024
#define BLACK 1
#define RED 0

/* Note your red-black node will need a few more items than
 * your naive node did (e.g., color, parent) */
typedef struct tree_node{
	struct tree_node * left;
	struct tree_node * right;
	char word[WORD_LEN];      /* key   */
	char definition[DEF_LEN]; /* value */
	struct tree_node * parent;
	int color;
} Tree_node;

void left_rotation(Tree_node* n){
    Tree_node* temp;
    char temp_word[WORD_LEN];
    char temp_definition[DEF_LEN];
    if(n->right == NULL || n->left == NULL) return ;
    temp = n->left;
    strncpy(temp_word, n->word, strlen(n->word) + 1);
    strncpy(temp_definition, n->definition, strlen(n->definition) + 1);
    n->left = n->right;
    strncpy(n->word, n->right->word, strlen(n->right->word) + 1);
    strncpy(n->definition, n->right->definition, strlen(n->right->definition) + 1);
    n->right = n->left->right;
    n->left->right = n->left->left;
    n->left->left = temp;
    strncpy(n->left->word, temp_word, strlen(temp_word) + 1);
    strncpy(n->left->definition, temp_definition, strlen(temp_definition) + 1);
}

void right_rotation(Tree_node* n){
    Tree_node* temp;
    char temp_word[WORD_LEN];
    char temp_definition[DEF_LEN];
    if(n->right == NULL || n->left == NULL) return ;
    temp = n->right;
    strncpy(temp_word, n->word, strlen(n->word) + 1);
    strncpy(temp_definition, n->definition, strlen(n->definition) + 1);
    n->right = n->left;
//    printf("n : %x, n->left %x, n->right: %x \n", n, n->left, n->right);
    strncpy(n->word, n->left->word, strlen(n->left->word) + 1);
    strncpy(n->definition, n->left->definition, strlen(n->left->definition) + 1);
    n->left = n->right->left;
    n->right->left = n->right->right;
    n->right->right = temp;
    strncpy(n->right->word, temp_word, strlen(temp_word) + 1);
    strncpy(n->right->definition, temp_definition, strlen(temp_definition) + 1);
}

void insert_case1(Tree_node* n, Tree_node* parent, Tree_node* grandparent);
void insert_case2(Tree_node* n, Tree_node* parent, Tree_node* grandparent);
void insert_case3(Tree_node* n, Tree_node* parent, Tree_node* grandparent);
void insert_case4(Tree_node* n, Tree_node* parent, Tree_node* grandparent);
void insert_case5(Tree_node* n, Tree_node* parent, Tree_node* grandparent);

//Tree_node* grandparent(Tree_node* n){
//    if(n == NULL) return NULL;
//    Tree_node* parent = n->parent;
//    if(parent != NULL) return parent->parent;
//    else return NULL;
//}
//
//Tree_node* uncle(Tree_node* n){
//    Tree_node *g = grandparent(n);
//    if(g == NULL) return NULL;
//    if(n->parent == g->left) return g->right;
//    return g->left;
//}
void insert_case5(Tree_node* n, Tree_node* parent, Tree_node* grandparent){
    if(n == NULL) return ;
    Tree_node* g = grandparent;
    if(g == NULL) return ;
    n->parent->color = BLACK;
    g->color = RED;
    if(n == n->parent->left) right_rotation(g);
    else left_rotation(g);
}

void insert_case4(Tree_node* n, Tree_node* parent, Tree_node* grandparent){
    Tree_node *g = grandparent;
    if(g == NULL) return ;
    if(n == n->parent->right && n->parent == g->left){
        left_rotation(n->parent);
        n = n->left;
    }
    else if(n == n->parent->left && n->parent == g->right){
        right_rotation(n->parent);
        n = n->right;
    }
    insert_case5(n, parent, grandparent);
}

void insert_case3(Tree_node* n, Tree_node* parent, Tree_node* grandparent){
    if(parent == NULL || grandparent == NULL) return;
    Tree_node *u = n == parent->left ? parent->right : parent->left, *g = grandparent;
    if(u != NULL && u->color == RED){
        n->parent->color = BLACK;
        u->color = BLACK;
//        g = grandparent(n);
        g->color = RED;
        insert_case1(g, NULL, NULL);
    }
    else insert_case4(n, parent, grandparent);
}

void insert_case2(Tree_node* n, Tree_node* parent, Tree_node* grandparent){
    if(n->parent->color == BLACK){
        return ;
    }
    else insert_case3(n, parent, grandparent);
}

void insert_case1(Tree_node* n, Tree_node* parent, Tree_node* grandparent){
    if(n->parent == NULL){
        n->color = BLACK;
    }
    else insert_case2(n, parent, grandparent);
}


/* This is the same interface as was in Problem 4, but you will
 * need to implement a number of helper functions to achieve the
 * ability to self balance your dictionary whenever nodes are added.
 * The interfaces for these functions (e.g., rotations,
 * grandparent/sibling helper functions, node swaps, etc) are up
 * for you to design and implement. */
int add_word(Tree_node ** dictionary, char * word, char * definition)
{
    Tree_node *grandparent = NULL;
    if(*dictionary == NULL){
		*dictionary = (Tree_node*)calloc(1, sizeof(Tree_node));
		strncpy((*dictionary)->word, word, strlen(word) + 1);
		strncpy((*dictionary)->definition, definition, strlen(definition) + 1);
        (*dictionary)->color = RED;
        (*dictionary)->parent = NULL;
        insert_case1(*dictionary, NULL, NULL);
		return 0;
	}
	Tree_node* parent = *dictionary;
	Tree_node* current = parent;
	int compareResult;
	while(current != NULL){
	    grandparent = parent;
		parent = current;
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
		strncpy(parent->left->word, word, strlen(word) + 1);
		strncpy(parent->left->definition, definition, strlen(definition) + 1);
		parent->left->color = RED;
		parent->left->parent = parent;
		insert_case1(parent->left, parent, grandparent);
	}
	else {
		parent->right = (Tree_node*)calloc(1, sizeof(Tree_node));
		strncpy(parent->right->word, word, strlen(word) + 1);
		strncpy(parent->right->definition, definition, strlen(definition) + 1);
		parent->right->color = RED;
		parent->right->parent = parent;
		insert_case1(parent->right, parent, grandparent);
	}
	return 0;
}

/* This should be identical to your find_word() function from Problem 4 */
int find_word(Tree_node * dictionary, char * word)
{
	if(dictionary == NULL) return 1;
	int compareResult = strcmp(word, dictionary->word);
	if(compareResult == 0) {
	    // printf("Find successfully!\n");
	    // printf("%s %s\n", dictionary->word, dictionary->definition);
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

/* This should be identical to your print_dictionary() function from Problem 4 */
void print_dictionary(Tree_node * dictionary)
{
	if(dictionary == NULL) return;
	print_dictionary(dictionary->left);
	printf("%s %s\n", dictionary->word, dictionary->definition);
	print_dictionary(dictionary->right);
}

/* This should be identical to your clear_dictionary() function from Problem 4 */
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

/* This should be identical to your import_file() function from Problem 4 */
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
    printf("balance time for import : %lf seconds.\n", timeA);
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
   	printf("balance time for finding : %lf microseconds per word\n", timeB);
   	for(int i = 0; i < wordCount; i++) free(words[i]);
   	free(words);

	return 0;
}
