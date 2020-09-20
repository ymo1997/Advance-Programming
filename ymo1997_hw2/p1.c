#include<stdio.h>
#include<stdlib.h>
#include<limits.h>

typedef struct tree_node{
	struct tree_node * left;
	struct tree_node * right;
	int data;
} Tree_node;

/* Determines if the given tree is a valid BST or not */
int is_tree_recursive(Tree_node * tree, int min, int max)
{
	if(tree == NULL) return 1;

	int val = tree->data;
	if(val <= min) return 0;
	if(val >= max) return 0;

	if(!is_tree_recursive(tree->left, min, val)) return 0;
	if(!is_tree_recursive(tree->right, val, max)) return 0;

	return 1;
}

Tree_node * build_good_tree(void)
{
	/* Allocate head */
	int n_nodes = 11;
	int i;
	Tree_node * nodes = (Tree_node *) malloc(n_nodes * sizeof(Tree_node));
	for( i = 0; i < n_nodes; i++ )
	{
		nodes[i].data  = i;
		nodes[i].left  = NULL;
		nodes[i].right = NULL;
	}

	/* Top Level */
	nodes[5].left =  &nodes[3];
	nodes[5].right = &nodes[7];
	/* Second Level */
	nodes[3].left =  &nodes[1];
	nodes[3].right = &nodes[4];
	nodes[7].left =  &nodes[6];
	nodes[7].right = &nodes[9];
	/* Third Level */
	nodes[1].left =  &nodes[0];
	nodes[1].right = &nodes[2];
	nodes[9].left =  &nodes[8];
	nodes[9].right = &nodes[10];
	/* Everything else is a leaf */

	return &nodes[5];
}

Tree_node * build_bad_tree(void)
{
	/* Allocate head */
	int n_nodes = 12;
	int i;
	Tree_node * nodes = (Tree_node *) malloc(n_nodes * sizeof(Tree_node));
	for( i = 0; i < n_nodes; i++ )
	{
		nodes[i].data  = i;
		nodes[i].left  = NULL;
		nodes[i].right = NULL;
	}

	/* Top Level */
	nodes[5].left =  &nodes[3];
	nodes[5].right = &nodes[8];
	/* Second Level */
	nodes[3].left =  &nodes[1];
	nodes[3].right = &nodes[6];
	nodes[8].left =  &nodes[4];
	nodes[8].right = &nodes[10];
	/* Third Level */
	nodes[1].left =  &nodes[0];
	nodes[1].right = &nodes[2];
	nodes[6].right = &nodes[9];
	nodes[10].left =  &nodes[7];
	nodes[10].right = &nodes[11];

	return &nodes[5];
}

int main(void)
{
	Tree_node * good_test_tree = build_good_tree();
	Tree_node * bad_test_tree;

	int isBST = is_tree_recursive(good_test_tree, INT_MIN, INT_MAX);

	if( isBST )
		printf("Tree is valid!\n");
	else
		printf("Tree is invalid!\n");

	bad_test_tree = build_bad_tree();

	isBST = is_tree_recursive(bad_test_tree, INT_MIN, INT_MAX);

	if( isBST )
		printf("Tree is valid!\n");
	else
		printf("Tree is invalid!\n");


	return 0;
}
