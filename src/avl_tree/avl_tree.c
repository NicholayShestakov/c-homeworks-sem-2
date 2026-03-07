#include "avl_tree.h"

#include <assert.h>
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
} AVLTree;

AVLTree* avlTreeCreate()
{
    AVLTree* tree = calloc(1, sizeof(*tree));
    assert(tree != NULL && "Memory allocation error.");
    return tree;
}

void avlTreeFreeRecursion(Node* node)
{
    if (node != NULL) {
        avlTreeFreeRecursion(node->leftChild);
        avlTreeFreeRecursion(node->rightChild);
        free(node);
    }
}

void avlTreeFree(AVLTree* tree)
{
    avlTreeFreeRecursion(tree->root);
    free(tree);
}

char* avlTreeFind(AVLTree* tree, char* key)
{
    Node* currentNode = tree->root;
    while (currentNode != NULL) {
        if (strcmp(key, currentNode->key) == 0) {
            return currentNode->value;
        }
        if (strcmp(key, currentNode->key) < 0) {
            currentNode = currentNode->leftChild;
        }
        if (strcmp(key, currentNode->key) > 0) {
            currentNode = currentNode->rightChild;
        }
    }
    return NULL;
}

// Возвращает ноду, если она есть. Иначе NULL
Node* avlTreeFindNode(AVLTree* tree, char* key)
{
    Node* currentNode = tree->root;
    while (currentNode != NULL) {
        if (strcmp(key, currentNode->key) == 0) {
            return currentNode;
        }
        if (strcmp(key, currentNode->key) < 0) {
            currentNode = currentNode->leftChild;
        }
        if (strcmp(key, currentNode->key) > 0) {
            currentNode = currentNode->rightChild;
        }
    }
    return NULL;
}

// Может использоваться для нахождения высоты какого-либо поддерева с корнем в node
size_t avlTreeHeightRecursion(Node* node)
{
    if (node == NULL) {
        return 0;
    }

    size_t leftHeight = avlTreeHeightRecursion(node->leftChild);
    size_t rightHeight = avlTreeHeightRecursion(node->rightChild);
    return (leftHeight > rightHeight) ? (leftHeight + 1) : (rightHeight + 1);
}

size_t avlTreeHeight(AVLTree* tree)
{
    return avlTreeHeightRecursion(tree->root);
}

Node* rotateLeft(Node* node)
{
    if (node->rightChild->balance == 0) {
        node->balance = -1;
        node->rightChild->balance = 1;
    } else {
        node->balance = 0;
        node->rightChild->balance = 0;
    }

    Node* a = node;
    Node* b = a->rightChild;
    Node* C = b->leftChild;

    a->rightChild = C;
    b->leftChild = a;

    return b;
}

Node* rotateRight(Node* node)
{
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
    Node* C = b->rightChild;

    a->leftChild = C;
    b->rightChild = a;

    return b;
}

Node* bigRotateLeft(Node* node)
{
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
    Node* M = c->leftChild;
    Node* N = c->rightChild;

    a->rightChild = M;
    b->leftChild = N;
    c->leftChild = a;
    c->rightChild = b;

    return c;
}

Node* bigRotateRight(Node* node)
{
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
    Node* M = c->leftChild;
    Node* N = c->rightChild;

    a->leftChild = N;
    b->rightChild = M;
    c->leftChild = b;
    c->rightChild = a;

    return c;
}

Node* balance(Node* node)
{
    if (node->balance == 2) {
        if (node->rightChild->balance >= 0) {
            return rotateLeft(node);
        }
        return bigRotateLeft(node);
    }
    if (node->balance == -2) {
        if (node->leftChild->balance >= 0) {
            return rotateRight(node);
        }
        return bigRotateRight(node);
    }
    return node;
}

Node* avlTreeAddRecursion(Node* node, char* key, char* value)
{
    if (node == NULL) {
        Node* newNode = calloc(1, sizeof(*newNode));
        assert(newNode != NULL && "Memory allocation error.");
        newNode->key = key;
        newNode->value = value;
        return newNode;
    }
    if (strcmp(key, node->key) < 0) {
        node->leftChild = avlTreeAddRecursion(node->leftChild, key, value);
        --node->balance;
    }
    if (strcmp(key, node->key) > 0) {
        node->rightChild = avlTreeAddRecursion(node->rightChild, key, value);
        ++node->balance;
    }
    return balance(node);
}

bool avlTreeAdd(AVLTree* tree, char* key, char* value)
{
    if (avlTreeFind(tree, key) != NULL) {
        return false;
    }

    Node* newNode = avlTreeAddRecursion(tree->root, key, value);
    if (tree->root == NULL) {
        tree->root = newNode;
    }
    return true;
}

bool isLeaf(Node* node)
{
    return node->rightChild == NULL && node->leftChild == NULL;
}

// Свапает ноды. Возвращает указатель на b, которая теперь находится на месте a.
Node* swapNodes(Node* a, Node* b)
{
    Node* tempLeft = a->leftChild;
    Node* tempRight = a->rightChild;
    a->leftChild = b->leftChild;
    a->rightChild = b->rightChild;
    b->leftChild = tempLeft;
    b->rightChild = tempRight;

    Node* temp = a;
    a = b;
    b = temp;

    return a;
}

Node* avlTreeDeleteRecursion(Node* node, char* key)
{
    if (strcmp(key, node->key) == 0) {
        if (isLeaf(node)) {
            free(node);
            return NULL;
        }

        Node* nodeToSwap = node->rightChild;
        while (nodeToSwap->leftChild != NULL) {
            nodeToSwap = nodeToSwap->leftChild;
        }
        avlTreeDeleteRecursion(swapNodes(nodeToSwap, node), key);
    }
    if (strcmp(key, node->key) < 0) {
        avlTreeDeleteRecursion(node->leftChild, key);
        ++node->balance;
    }
    if (strcmp(key, node->key) > 0) {
        avlTreeDeleteRecursion(node->leftChild, key);
        --node->balance;
    }
    return balance(node);
}

bool avlTreeDelete(AVLTree* tree, char* key)
{
    if (avlTreeFind(tree, key) == NULL) {
        return false;
    }

    avlTreeDeleteRecursion(tree->root, key);
    return true;
}

void avlTreeKeysToArrayRecursion(Node* node, char** keysArray, size_t arraySize, int currentNumber)
{
    if (node == NULL) {
        return;
    }
    keysArray[currentNumber - 1] = node->key;
    avlTreeKeysToArrayRecursion(node->leftChild, keysArray, arraySize, currentNumber * 2);
    avlTreeKeysToArrayRecursion(node->rightChild, keysArray, arraySize, currentNumber * 2 + 1);
}

// Возвращает массив указателей на ключи поддерева конкретной ноды, включая её, в порядке обхода в ширину.
// Если ноды нету, ставит на её месте NULL. Освобождается простым free.
char** avlTreeKeysToArray(Node* node)
{
    size_t arraySize = (size_t)pow(2, avlTreeHeightRecursion(node)) - 1;
    char** keysArray = calloc(arraySize, sizeof(*keysArray));
    avlTreeKeysToArrayRecursion(node, keysArray, arraySize, 1);
    return keysArray;
}

bool isPowerOfTwo(int a)
{
    while (a != 1) {
        if (a % 2 == 1) {
            return false;
        }
        a /= 2;
    }
    return true;
}

void avlTreePrint(AVLTree* tree)
{
    // size_t height = avlTreeHeight(tree);
    // size_t width = 4 * (size_t)pow(2, (height - 1)) - 1;
    size_t size = (size_t)pow(2, avlTreeHeight(tree)) - 1;
    char** keysArray = avlTreeKeysToArray(tree->root);

    for (size_t i = 0; i < size; ++i) {
        printf("%s ", keysArray[i]);
    }

    free(keysArray);
}
