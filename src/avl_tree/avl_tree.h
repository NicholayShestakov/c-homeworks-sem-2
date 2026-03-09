#pragma once

#include <stdbool.h>
#include <stdlib.h>

typedef struct AVLTree AVLTree;

/*
 * Creates AVL-tree with string keys and string values and returns pointer to it.
 */
AVLTree* avlTreeCreate(void);

/*
 * Deletes AVL-tree, frees memory used by it and NULLs pointer.
 * Returns true if all correct and false if somethong went wrong.
 */
bool avlTreeFree(AVLTree** tree);

/*
 * Finds node with given key.
 * Returns this node pointer to the value.
 * If something went wrong returns NULL.
 */
char* avlTreeFind(AVLTree* tree, char* key);

/*
 * Finds height of tree and writes it in 'height' variable.
 * Height - count of not empty layers of nodes.
 * If something went wrong returns false, else returns true.
 */
bool avlTreeHeight(AVLTree* tree, size_t* height);

/*
 * Finds size of tree and writes it in 'size' variable.
 * Size - count of nodes in tree.
 * If something went wrong returns false, else returns true.
 */
bool avlTreeSize(AVLTree* tree, size_t* size);

/*
 * If given key does not exists, adds node with given key and value in tree and returns true.
 * If key is already exists or something went wrong returns false.
 */
bool avlTreeAdd(AVLTree* tree, char* key, char* value);

/*
 * Adds in tree nodes with keys and values from file <filename>.
 * Keys and values in file must be in format '<key>:<value>\n'.
 * <key> and <value> must not include ':' and '\n'.
 * If node with the <key> already exists in tree, skips it.
 * If something went wrong returns false, else returns true.
 */
bool avlTreeAddFromFile(AVLTree* tree, char* filename);

/*
 * Deletes node with given key and returns true, if node is exists.
 * If node with given key does not exists or something went wrong returns false.
 */
bool avlTreeDelete(AVLTree* tree, char* key);
