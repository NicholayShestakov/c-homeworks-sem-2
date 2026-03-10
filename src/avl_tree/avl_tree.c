#include "avl_tree.h"

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Node {
    char* key;
    char* value;
    int balance;
    struct Node* leftChild;
    struct Node* rightChild;
} Node;

typedef struct AVLTree {
    Node* root;
    size_t size;
} AVLTree;

static bool isLeaf(Node* node)
{
    return node != NULL && node->rightChild == NULL && node->leftChild == NULL;
}

AVLTree* avlTreeCreate(void)
{
    return calloc(1, sizeof(AVLTree));
}

char* avlTreeFind(AVLTree* tree, char* key)
{
    if (tree == NULL || key == NULL) {
        return NULL;
    }

    Node* currentNode = tree->root;
    while (currentNode != NULL) {
        if (strcmp(key, currentNode->key) < 0) {
            currentNode = currentNode->leftChild;
        } else if (strcmp(key, currentNode->key) > 0) {
            currentNode = currentNode->rightChild;
        } else {
            return currentNode->value;
        }
    }

    return NULL;
}

/*
 * Освобождает память ноды и зануляет указатель на неё.
 * Для предотвращения появления висячих нод работает только на листьях.
 * Не зануляет указатель родителя этой ноды.
 */
static void avlTreeFreeNode(Node** node)
{
    if (node != NULL && isLeaf(*node)) {
        free((*node)->key);
        free((*node)->value);
        free(*node);
        *node = NULL;
    }
}

static void avlTreeFreeRecursion(Node* node)
{
    if (node != NULL) {
        avlTreeFreeRecursion(node->leftChild);
        avlTreeFreeRecursion(node->rightChild);
        node->leftChild = NULL;
        node->rightChild = NULL;
        avlTreeFreeNode(&node);
    }
}

bool avlTreeFree(AVLTree** tree)
{
    if (tree == NULL || *tree == NULL) {
        return false;
    }

    avlTreeFreeRecursion((*tree)->root);
    free(*tree);
    *tree = NULL;

    return true;
}

static size_t avlTreeHeightRecursion(Node* node)
{
    if (node == NULL) {
        return 0;
    }

    size_t leftHeight = avlTreeHeightRecursion(node->leftChild);
    size_t rightHeight = avlTreeHeightRecursion(node->rightChild);
    return (leftHeight > rightHeight) ? (leftHeight + 1) : (rightHeight + 1);
}

bool avlTreeHeight(AVLTree* tree, size_t* height)
{
    if (tree == NULL || height == NULL) {
        return false;
    }

    *height = avlTreeHeightRecursion(tree->root);
    return true;
}

bool avlTreeSize(AVLTree* tree, size_t* size)
{
    if (tree == NULL || size == NULL) {
        return false;
    }

    *size = tree->size;
    return true;
}

static Node* rotateLeft(Node* node)
{
    if (node == NULL || node->rightChild == NULL) {
        return NULL;
    }

    switch (node->rightChild->balance) {
    case 0:
        node->balance = -1;
        node->rightChild->balance = 1;
        break;

    default: // case 1
        node->balance = 0;
        node->rightChild->balance = 0;
        break;
    }

    Node* a = node;
    Node* b = a->rightChild;
    Node* c = b->leftChild;

    a->rightChild = c;
    b->leftChild = a;

    return b;
}

static Node* rotateRight(Node* node)
{
    if (node == NULL || node->leftChild == NULL) {
        return NULL;
    }

    switch (node->leftChild->balance) {
    case 0:
        node->balance = -1;
        node->leftChild->balance = 1;
        break;

    default: // case 1
        node->balance = 0;
        node->leftChild->balance = 0;
        break;
    }

    Node* a = node;
    Node* b = a->leftChild;
    Node* c = b->rightChild;

    a->leftChild = c;
    b->rightChild = a;

    return b;
}

static Node* bigRotateLeft(Node* node)
{
    if (node == NULL || node->rightChild == NULL || node->rightChild->leftChild == NULL) {
        return NULL;
    }

    switch (node->rightChild->leftChild->balance) {
    case -1:
        node->balance = 1;
        node->rightChild->balance = 0;
        break;
    case 0:
        node->balance = 0;
        node->rightChild->balance = 0;
        break;
    default: // case 1
        node->balance = 0;
        node->rightChild->balance = -1;
        break;
    }
    node->rightChild->leftChild->balance = 0;

    Node* a = node;
    Node* b = a->rightChild;
    Node* c = b->leftChild;
    Node* m = c->leftChild;
    Node* n = c->rightChild;

    a->rightChild = m;
    b->leftChild = n;
    c->leftChild = a;
    c->rightChild = b;

    return c;
}

static Node* bigRotateRight(Node* node)
{
    if (node == NULL || node->leftChild == NULL || node->leftChild->rightChild == NULL) {
        return NULL;
    }

    switch (node->leftChild->rightChild->balance) {
    case -1:
        node->balance = 1;
        node->leftChild->balance = 0;
        break;
    case 0:
        node->balance = 0;
        node->leftChild->balance = 0;
        break;
    default:
        node->balance = 0;
        node->leftChild->balance = -1;
        break;
    }
    node->leftChild->rightChild->balance = 0;

    Node* a = node;
    Node* b = a->leftChild;
    Node* c = b->rightChild;
    Node* m = c->leftChild;
    Node* n = c->rightChild;

    a->leftChild = n;
    b->rightChild = m;
    c->leftChild = b;
    c->rightChild = a;

    return c;
}

static Node* balance(Node* node)
{
    if (node == NULL) {
        return NULL;
    }

    if (node->balance == 2) {
        if (node->rightChild == NULL) {
            return NULL;
        }
        if (node->rightChild->balance >= 0) {
            return rotateLeft(node);
        }
        return bigRotateLeft(node);
    }
    if (node->balance == -2) {
        if (node->leftChild == NULL) {
            return NULL;
        }
        if (node->leftChild->balance <= 0) {
            return rotateRight(node);
        }
        return bigRotateRight(node);
    }

    return node;
}

static Node* avlTreeAddRecursion(Node* node, char* key, char* value, bool* isBalanced, bool* error)
{
    if (node == NULL) {
        Node* newNode = calloc(1, sizeof(*newNode));
        if (newNode == NULL) {
            *error = true;
            return NULL;
        }

        newNode->key = malloc((strlen(key) + 1) * sizeof(char));
        if (newNode->key == NULL) {
            *error = true;
            return NULL;
        }
        strlcpy(newNode->key, key, sizeof(newNode->key));

        newNode->value = malloc((strlen(value) + 1) * sizeof(char));
        if (newNode->value == NULL) {
            *error = true;
            return NULL;
        }
        strlcpy(newNode->value, value, sizeof(newNode->value));

        return newNode;
    }

    if (strcmp(key, node->key) < 0) {
        node->leftChild = avlTreeAddRecursion(node->leftChild, key, value, isBalanced, error);
        if (!(*isBalanced) && !(*error)) {
            --node->balance;
        }
    }
    if (strcmp(key, node->key) > 0) {
        node->rightChild = avlTreeAddRecursion(node->rightChild, key, value, isBalanced, error);
        if (!(*isBalanced) && !(*error)) {
            ++node->balance;
        }
    }

    node = balance(node);
    if (node->balance == 0) {
        *isBalanced = true;
    }
    return node;
}

bool avlTreeAdd(AVLTree* tree, char* key, char* value)
{
    if (tree == NULL || key == NULL || value == NULL || avlTreeFind(tree, key) != NULL) {
        return false;
    }

    bool error = false;
    bool isBalanced = false;
    Node* newRoot = avlTreeAddRecursion(tree->root, key, value, &isBalanced, &error);
    if (error) {
        return false;
    }
    tree->root = newRoot;

    ++(tree->size);
    return true;
}

bool avlTreeAddFromFile(AVLTree* tree, char* filename)
{
    if (tree == NULL || filename == NULL) {
        return false;
    }

    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        return false;
    }

    char* key = calloc(4, sizeof(*key));
    if (key == NULL) {
        return false;
    }
    size_t keyCapacity = 4;
    size_t keySize = 1; // Размер сразу 1 на \0

    char* value = calloc(64, sizeof(*value));
    if (value == NULL) {
        free(key);
        return false;
    }
    size_t valueCapacity = 64;
    size_t valueSize = 1; // Размер сразу 1 на \0

    bool isPreColon = true;
    while (!feof(file)) {
        char currentSymbol = (char)fgetc(file);
        switch (currentSymbol) {
        case ':':
            if (isPreColon) {
                isPreColon = false;
            } else {
                free(key);
                free(value);
                fclose(file);
                return false;
            }
            break;
        case '\n':
            if (!isPreColon) {
                avlTreeAdd(tree, key, value);

                free(key);
                free(value);

                key = calloc(4, sizeof(*key));
                if (key == NULL) {
                    return false;
                }
                keyCapacity = 4;
                keySize = 1;

                value = calloc(64, sizeof(*value));
                if (value == NULL) {
                    free(key);
                    return false;
                }
                valueCapacity = 64;
                valueSize = 1;

                isPreColon = true;
            } else {
                free(key);
                free(value);
                fclose(file);
                return false;
            }
            break;
        default:
            if (isPreColon) {
                if (keySize == keyCapacity) {
                    keyCapacity *= 2;
                    key = realloc(key, keyCapacity * sizeof(*key));
                    if (key == NULL) {
                        free(value);
                        return false;
                    }
                }
                key[keySize - 1] = currentSymbol;
                key[keySize++] = '\0';
            } else {
                if (valueSize == valueCapacity) {
                    valueCapacity *= 2;
                    value = realloc(value, valueCapacity * sizeof(*value));
                    if (value == NULL) {
                        free(key);
                        return false;
                    }
                }
                value[valueSize - 1] = currentSymbol;
                value[valueSize++] = '\0';
            }
            break;
        }
    }

    free(key);
    free(value);
    fclose(file);
    return true;
}

void avlTreeSaveInFileRecursion(Node* node, FILE* file, bool* error)
{
    if (file == NULL) {
        *error = true;
        return;
    }
    if (node == NULL) {
        return;
    }
    if (node->key == NULL || node->value == NULL) {
        *error = true;
        return;
    }

    fprintf(file, "%s:%s\n", node->key, node->value);
    avlTreeSaveInFileRecursion(node->leftChild, file, error);
    avlTreeSaveInFileRecursion(node->rightChild, file, error);
}

bool avlTreeSaveInFile(AVLTree* tree, char* filename)
{
    if (tree == NULL || filename == NULL) {
        return false;
    }

    FILE* file = fopen(filename, "w");
    if (file == NULL) {
        return false;
    }

    bool error = false;
    avlTreeSaveInFileRecursion(tree->root, file, &error);
    fclose(file);
    return !error;
}

/*
 * Меняет местами две ненулевые ноды.
 * В случае нулёвости одной из нод ничего не делает.
 */
static void swapNodes(Node* a, Node* b)
{
    if (a == NULL || b == NULL) {
        return;
    }

    char* tempKey = a->key;
    char* tempValue = a->value;
    a->key = b->key;
    a->value = b->value;
    b->key = tempKey;
    b->value = tempValue;
}

static Node* avlTreeDeleteRecursion(Node* node, char* key, bool* isBalanced, bool* error)
{
    if (node == NULL || key == NULL) {
        *error = true;
        return NULL;
    }

    if (strcmp(key, node->key) < 0) {
        node->leftChild = avlTreeDeleteRecursion(node->leftChild, key, isBalanced, error);
        if (!(*isBalanced) && !(*error)) {
            ++node->balance;
        }
    } else if (strcmp(key, node->key) > 0) {
        node->rightChild = avlTreeDeleteRecursion(node->rightChild, key, isBalanced, error);
        if (!(*isBalanced) && !(*error)) {
            --node->balance;
        }
    } else {
        if (isLeaf(node)) {
            avlTreeFreeNode(&node);
            return NULL;
        }

        if (node->leftChild == NULL) {
            Node* nodeToSwap = node->rightChild;
            while (nodeToSwap->leftChild != NULL) {
                nodeToSwap = nodeToSwap->leftChild;
            }
            swapNodes(nodeToSwap, node);

            node->rightChild = avlTreeDeleteRecursion(node->rightChild, key, isBalanced, error);
        } else {
            Node* nodeToSwap = node->leftChild;
            while (nodeToSwap->rightChild != NULL) {
                nodeToSwap = nodeToSwap->rightChild;
            }
            swapNodes(nodeToSwap, node);

            node->leftChild = avlTreeDeleteRecursion(node->leftChild, key, isBalanced, error);
        }
    }

    node = balance(node);
    if (node->balance == 1 || node->balance == -1) {
        *isBalanced = true;
    }
    return node;
}

bool avlTreeDelete(AVLTree* tree, char* key)
{
    if (tree == NULL || avlTreeFind(tree, key) == NULL) {
        return false;
    }

    bool error = false;
    bool isBalanced = false;
    Node* newRoot = avlTreeDeleteRecursion(tree->root, key, &isBalanced, &error);
    if (error) {
        return false;
    }
    tree->root = newRoot;

    --(tree->size);
    return true;
}
