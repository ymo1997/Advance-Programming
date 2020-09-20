#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define WORD_LEN 32
#define DEF_LEN 256
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
	    printf("Find successfully!\n");
	    printf("%s %s\n", dictionary->word, dictionary->definition);
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

int main(void)
{
	Tree_node * dictionary = NULL;

	while(1)
	{
		int error;
		char command[COMMAND_LEN];

		/* Get the User's Command */
		printf(">$ ");
		scanf("%s", command);

		/* Add word */
		if( strncmp(command, "add", COMMAND_LEN) == 0 )
		{
			char word[WORD_LEN];
			char definition[DEF_LEN];
			char * pos;
			scanf("%s ",word);
			/* Gets the rest of the input */
			fgets(definition, DEF_LEN, stdin);
			if ((pos=strchr(definition, '\n')) != NULL)
				*pos = '\0';

			/* Adds to dictionary */
			error = add_word(&dictionary, word, definition);

			if( error == 0 )
				printf("The word \"%s\" has been added successfully.\n", word);
			else
				printf("Error when adding word \"%s\".\n", word);

		}
		/* Import file */
		if( strncmp(command, "import", COMMAND_LEN) == 0 )
		{
			char fname[DEF_LEN];
			scanf("%s",fname);
			error = import_file(fname, &dictionary );
			if( error == 0 )
				printf("The file \"%s\" has been imported successfully.\n", fname);
			else
				printf("Error when adding file \"%s\".\n", fname);
		}
		/* Delete word */
		if( strncmp(command, "delete", COMMAND_LEN) == 0 )
		{
			char word[WORD_LEN];
			scanf("%s",word);
			error = delete_word(&dictionary, dictionary, word);
			if( error == 0 )
				printf("The word \"%s\" has been deleted successfully.\n", word);
			else
				printf("Error when deleting word \"%s\".\n", word);
		}
		/* Find word */
		if( strncmp(command, "find", COMMAND_LEN) == 0 )
		{
			char word[WORD_LEN];
			scanf("%s",word);
			error = find_word(dictionary, word);
			if( error == 1 )
				printf("Error when finding word \"%s\".\n", word);
		}
		/* Print dictionary */
		if( strncmp(command, "print", COMMAND_LEN) == 0 )
			print_dictionary(dictionary);
		/* clear dictionary */
		if( strncmp(command, "clear", COMMAND_LEN) == 0 )
			clear_dictionary(&dictionary);
		/* quit program */
		if( strncmp(command, "quit", COMMAND_LEN) == 0 )
			break;
	}
	return 0;
}
