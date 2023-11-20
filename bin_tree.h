#ifndef BIN_TREE_FUNC
#define BIN_TREE_FUNC

#include "bin_tree_values.h"

int TreeRootCtor(TreeStruct *tree);
TreeNode *TreeNodeNew(TreeStruct *tree, Tree_t value);
int PrintPreOrder(const TreeNode *node, FILE *file);
int TreeInsertNum(TreeStruct *tree, const Tree_t number);
int TreeDtor(TreeStruct *tree, TreeNode *node);
int TreeRootDtor(TreeStruct *tree);
int NodeDtor(TreeStruct *tree, TreeNode *node);
int TreeInsertNum(TreeStruct *tree, const Tree_t number);
int PrintInOrder(TreeNode *node, FILE *file);
int PrintPostOrder(TreeNode *node, FILE *file);
int PrintSortTree(TreeNode *node);

#endif