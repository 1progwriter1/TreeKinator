#include <stdio.h>
#include "bin_tree.h"
#include "bin_tree_values.h"
#include <assert.h>
#include "../MyLibraries/headers/systemdata.h"
#include "../MyLibraries/headers/func.h"
#include <string.h>

// static int ReadNode(TreeStruct *tree, TreeNode *node, FILE *fn);

int TreeRootCtor(TreeStruct *tree) {

    assert(tree);

    tree->root = TreeNodeNew(tree, NULL);
    if (!tree->root)
        return NO_MEMORY;

    return SUCCESS;
}

TreeNode *TreeNodeNew(TreeStruct *tree, Tree_t value) {

    TreeNode *node = (TreeNode *) calloc (1, sizeof (TreeNode));
    if (!node)
        return NULL;

    node->value = value;
    node->right = NULL;
    node->left = NULL;

    tree->size++;

    return node;
}

int TreeRootDtor(TreeStruct *tree) {

    assert(tree);

    if (tree->root->left || tree->root->right) {
        TreeDtor(tree, tree->root);
    }
    free(tree->root);
    tree->root = NULL;
    tree->size = 0;

    return SUCCESS;
}

int TreeDtor(TreeStruct *tree, TreeNode *node) {

    assert(node);
    assert(tree);

    if (node->left)
        TreeDtor(tree, node->left);
    if (node->right)
        TreeDtor(tree, node->right);

    node->left = NULL;
    node->right = NULL;

    return SUCCESS;
}

int NodeDtor(TreeStruct *tree, TreeNode *node) {

    assert(tree);
    assert(node);

    if (node->left)
        NodeDtor(tree, node->left);
    if (node->right)
        NodeDtor(tree, node->right);

    node->left = NULL;
    node->right = NULL;
    free(node);

    tree->size--;

    return SUCCESS;
}

int PrintPreOrder(const TreeNode *node, FILE *file) {

    assert(file);

    if (!node) {
        fprintf(file, "nil ");
        return SUCCESS;
    }

    fprintf(file, "( ");

    fprintf(file, list_output_id " ", node->value);
    PrintPreOrder(node->left, file);
    PrintPreOrder(node->right, file);

    fprintf(file, ") ");

    return SUCCESS;
}

int TreeInsertNum(TreeStruct *tree, const Tree_t number) {

    assert(tree);

    if (!tree->root) {
        tree->root->value = number;
        return SUCCESS;
    }

    TreeNode *ptr = tree->root;
    while (1) {
        if (number < ptr->value)
            if (!ptr->left) {
                ptr->left = TreeNodeNew(tree, number);
                if (!ptr->left)
                    return NO_MEMORY;
                break;
            }
            else {
                ptr = ptr->left;
            }
        else
            if (!ptr->right) {
                ptr->right = TreeNodeNew(tree, number);
                if (!ptr->right)
                    return NO_MEMORY;
                break;
            }
            else {
                ptr = ptr->right;
            }
    }

    return SUCCESS;
}

int PrintInOrder(TreeNode *node, FILE *file) {

    assert(file);

    if (!node) {
        fprintf(file, "nil ");
        return SUCCESS;
    }

    fprintf(file, "( ");

    PrintPostOrder(node->left, file);
    fprintf(file, list_output_id " ", node->value);
    PrintPostOrder(node->right, file);

    fprintf(file, ") ");

    return SUCCESS;

}

int PrintPostOrder(TreeNode *node, FILE *file) {

    assert(file);

    if (!node) {
        fprintf(file, "nil ");
        return SUCCESS;
    }

    fprintf(file, "( ");

    PrintPostOrder(node->left, file);
    PrintPostOrder(node->right, file);
    fprintf(file, list_output_id " ", node->value);

    fprintf(file, ") ");

    return SUCCESS;

}

int PrintSortTree(TreeNode *node) {

    if (!node)
        return SUCCESS;


    PrintSortTree(node->left);
    printf(list_output_id " ", node->value);
    PrintSortTree(node->right);

    return SUCCESS;
}