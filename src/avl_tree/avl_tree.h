#pragma once

#include <stdbool.h>

typedef struct AVLTree AVLTree;

// Creates AVL-tree with 3 symbol string keys and string values and returns pointer to it.
AVLTree* avlTreeCreate();

// Deletes AVL-tree and frees memory used by it.
void avlTreeFree(AVLTree* tree);

/*
Finds node with given key.
Returns this node pointer on the string.
If key does not exists returns NULL.
*/
char* avlTreeFind(AVLTree* tree, char* key);

/*
If given key does not exists, adds node with given key and value in tree and returns true.
If key is already exists, returns false.
*/
bool avlTreeAdd(AVLTree* tree, char* key, char* value);

/*
Deletes node with given key and returns true, if node is exists.
If node with given key does not exists, returns false.
*/
bool avlTreeDelete(AVLTree* tree, char* key);

/*
Prints tree.
*/
void avlTreePrint(AVLTree* tree);
