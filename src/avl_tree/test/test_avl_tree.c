#include "../avl_tree.c" // NOLINT
#include <assert.h>

void emptyTest(void)
{
    AVLTree* tree = avlTreeCreate();
    assert(!avlTreeDelete(tree, "abc") && "Delete operation works incorrect on empty tree.");
    assert(avlTreeFind(tree, "abc") == NULL && "Find operation works incorrect on empty tree.");
    size_t height = 1;
    assert(avlTreeHeight(tree, &height) && height == 0 && "Height operation works incorrect on empty tree.");
    size_t size = 1;
    assert(avlTreeSize(tree, &size) && size == 0 && "Size operation works incorrect on empty tree.");
    assert(avlTreeFree(&tree) && "Free operation works incorrect on empty tree.");
}

void nullTreePointerTest(void)
{
    assert(!avlTreeAdd(NULL, "abc", "some data") && "Delete operation works incorrect with NULL pointer.");
    assert(!avlTreeDelete(NULL, "abc") && "Delete operation works incorrect with NULL pointer.");
    assert(avlTreeFind(NULL, "abc") == NULL && "Find operation works incorrect with NULL pointer.");
    size_t height = 0;
    assert(!avlTreeHeight(NULL, &height) && "Height operation works incorrect with NULL pointer.");
    size_t size = 0;
    assert(!avlTreeSize(NULL, &size) && "Size operation works incorrect with NULL pointer.");
    AVLTree* nullTree = NULL;
    assert(!avlTreeFree(&nullTree) && "Free operation works incorrect with NULL pointer.");
}

void leftRotationTest(void)
{
    AVLTree* tree = avlTreeCreate();
    avlTreeAdd(tree, "a", "some data");
    avlTreeAdd(tree, "b", "some data");
    avlTreeAdd(tree, "c", "some data");
    assert(tree->root->key != NULL && strcmp(tree->root->key, "b") == 0 && "Left rotation works incorrect.");
    assert(tree->root->leftChild->key != NULL && strcmp(tree->root->leftChild->key, "a") == 0 && "Left rotation works incorrect.");
    assert(tree->root->rightChild->key != NULL && strcmp(tree->root->rightChild->key, "c") == 0 && "Left rotation works incorrect.");
    avlTreeFree(&tree);
}

void rightRotationTest(void)
{
    AVLTree* tree = avlTreeCreate();
    avlTreeAdd(tree, "c", "some data");
    avlTreeAdd(tree, "b", "some data");
    avlTreeAdd(tree, "a", "some data");
    assert(tree->root->key != NULL && strcmp(tree->root->key, "b") == 0 && "Right rotation works incorrect.");
    assert(tree->root->leftChild->key != NULL && strcmp(tree->root->leftChild->key, "a") == 0 && "Right rotation works incorrect.");
    assert(tree->root->rightChild->key != NULL && strcmp(tree->root->rightChild->key, "c") == 0 && "Right rotation works incorrect.");
    avlTreeFree(&tree);
}

void bigLeftRotationTest(void)
{
    AVLTree* tree = avlTreeCreate();
    avlTreeAdd(tree, "a", "some data");
    avlTreeAdd(tree, "c", "some data");
    avlTreeAdd(tree, "b", "some data");
    assert(tree->root->key != NULL && strcmp(tree->root->key, "b") == 0 && "Big left rotation works incorrect.");
    assert(tree->root->leftChild->key != NULL && strcmp(tree->root->leftChild->key, "a") == 0 && "Big left rotation works incorrect.");
    assert(tree->root->rightChild->key != NULL && strcmp(tree->root->rightChild->key, "c") == 0 && "Big left rotation works incorrect.");
    avlTreeFree(&tree);
}

void bigRightRotationTest(void)
{
    AVLTree* tree = avlTreeCreate();
    avlTreeAdd(tree, "c", "some data");
    avlTreeAdd(tree, "a", "some data");
    avlTreeAdd(tree, "b", "some data");
    assert(tree->root->key != NULL && strcmp(tree->root->key, "b") == 0 && "Big right rotation works incorrect.");
    assert(tree->root->leftChild->key != NULL && strcmp(tree->root->leftChild->key, "a") == 0 && "Big right rotation works incorrect.");
    assert(tree->root->rightChild->key != NULL && strcmp(tree->root->rightChild->key, "c") == 0 && "Big right rotation works incorrect.");
    avlTreeFree(&tree);
}

void deleteLeftRotationTest(void)
{
    AVLTree* tree = avlTreeCreate();
    avlTreeAdd(tree, "b", "some data");
    avlTreeAdd(tree, "a", "some data");
    avlTreeAdd(tree, "c", "some data");
    avlTreeAdd(tree, "d", "some data");
    avlTreeDelete(tree, "a");
    assert(tree->root->key != NULL && strcmp(tree->root->key, "c") == 0 && "Left rotation after delete works incorrect.");
    assert(tree->root->leftChild->key != NULL && strcmp(tree->root->leftChild->key, "b") == 0 && "Left rotation after delete works incorrect.");
    assert(tree->root->rightChild->key != NULL && strcmp(tree->root->rightChild->key, "d") == 0 && "Left rotation after delete works incorrect.");
    avlTreeFree(&tree);
}

void deleteRightRotationTest(void)
{
    AVLTree* tree = avlTreeCreate();
    avlTreeAdd(tree, "c", "some data");
    avlTreeAdd(tree, "b", "some data");
    avlTreeAdd(tree, "a", "some data");
    avlTreeAdd(tree, "d", "some data");
    avlTreeDelete(tree, "d");
    assert(tree->root->key != NULL && strcmp(tree->root->key, "b") == 0 && "Right rotation after delete works incorrect.");
    assert(tree->root->leftChild->key != NULL && strcmp(tree->root->leftChild->key, "a") == 0 && "Right rotation after delete works incorrect.");
    assert(tree->root->rightChild->key != NULL && strcmp(tree->root->rightChild->key, "c") == 0 && "Right rotation after delete works incorrect.");
    avlTreeFree(&tree);
}

void deleteBigLeftRotationTest(void)
{
    AVLTree* tree = avlTreeCreate();
    avlTreeAdd(tree, "b", "some data");
    avlTreeAdd(tree, "a", "some data");
    avlTreeAdd(tree, "d", "some data");
    avlTreeAdd(tree, "c", "some data");
    avlTreeDelete(tree, "a");
    assert(tree->root->key != NULL && strcmp(tree->root->key, "c") == 0 && "Big left rotation after delete works incorrect.");
    assert(tree->root->leftChild->key != NULL && strcmp(tree->root->leftChild->key, "b") == 0 && "Big left rotation after delete works incorrect.");
    assert(tree->root->rightChild->key != NULL && strcmp(tree->root->rightChild->key, "d") == 0 && "Big left rotation after delete works incorrect.");
    avlTreeFree(&tree);
}

void deleteBigRightRotationTest(void)
{
    AVLTree* tree = avlTreeCreate();
    avlTreeAdd(tree, "c", "some data");
    avlTreeAdd(tree, "a", "some data");
    avlTreeAdd(tree, "b", "some data");
    avlTreeAdd(tree, "d", "some data");
    avlTreeDelete(tree, "d");
    assert(tree->root->key != NULL && strcmp(tree->root->key, "b") == 0 && "Big left rotation after delete works incorrect.");
    assert(tree->root->leftChild->key != NULL && strcmp(tree->root->leftChild->key, "a") == 0 && "Big left rotation after delete works incorrect.");
    assert(tree->root->rightChild->key != NULL && strcmp(tree->root->rightChild->key, "c") == 0 && "Big left rotation after delete works incorrect.");
    avlTreeFree(&tree);
}

void addFromFileAndSaveInFileTest(void)
{
    AVLTree* tree = avlTreeCreate();
    assert(avlTreeAddFromFile(tree, "../src/avl_tree/test/testfile.txt") && "Add from file works incorrect.");
    assert(tree->root->key != NULL && strcmp(tree->root->key, "b") == 0 && "Add from file works incorrect.");
    assert(tree->root->leftChild->key != NULL && strcmp(tree->root->leftChild->key, "a") == 0 && "Add from file works incorrect.");
    assert(tree->root->rightChild->key != NULL && strcmp(tree->root->rightChild->key, "c") == 0 && "Add from file works incorrect.");

    assert(avlTreeSaveInFile(tree, "testsavefile.txt") && "Save in file works incorrect.");
    FILE* testsavefile = fopen("testsavefile.txt", "r");
    char buffer[100];
    assert(strcmp(fgets(buffer, sizeof(buffer), testsavefile), "b:some data\n") == 0 && "Save in file works incorrect.");
    assert(strcmp(fgets(buffer, sizeof(buffer), testsavefile), "a:some data\n") == 0 && "Save in file works incorrect.");
    assert(strcmp(fgets(buffer, sizeof(buffer), testsavefile), "c:some data\n") == 0 && "Save in file works incorrect.");

    fclose(testsavefile);
    remove("testsavefile.txt");
    avlTreeFree(&tree);
}

int main(void)
{
    emptyTest();
    nullTreePointerTest();
    leftRotationTest();
    rightRotationTest();
    bigLeftRotationTest();
    bigRightRotationTest();
    deleteLeftRotationTest();
    deleteRightRotationTest();
    deleteBigLeftRotationTest();
    deleteBigRightRotationTest();
    addFromFileAndSaveInFileTest();

    return 0;
}
