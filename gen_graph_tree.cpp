#include <stdio.h>
#include "bin_tree_values.h"
#include <assert.h>
#include "../MyLibraries/headers/systemdata.h"
#include "../MyLibraries/headers/file_func.h"

static int GenTreeNodes(FILE *fn, const TreeNode *node, int *index);
static int GenAkinatorNodes(FILE *fn, const TreeNode *node, int *index);

int GenGraphTree(const TreeStruct *tree) {

    assert(tree);

    FILE *fn = fileopen(GRAPH_FILE_TREE, WRITE);
    if (!fn)
        return FILE_OPEN_ERROR;

    int index = 0;
    fprintf(fn, "digraph G {\n\tgcharset=latin-1\n\t");
    fprintf(fn, "-10 [shape = Mrecord, style = filled, fillcolor = \"#FFFFCC\", color = \"#CC0000\", label = \"root: %p \\n size: %lu\"]\n\t", tree->root, tree->size);
    fprintf(fn, "-10->0 [color = \"#FFFFFF\", weight = 1000]");
    GenTreeNodes(fn, tree->root, &index);

    fprintf(fn, "\n");
    fprintf(fn, "}");

    fileclose(fn);

    system("dot " GRAPH_FILE_TREE " -T png -o " GRAPH_FILE_TREE_OUT);

    return SUCCESS;
}

static int GenTreeNodes(FILE *fn, const TreeNode *node, int *index) {

    assert(fn);
    assert(node);

    fprintf(fn, "%d [shape = Mrecord, style = filled, fillcolor = \"#CCFF99\", color = \"#331900\", label = \"{" list_output_id " |{ <fl> %p | <fr> %p}}\"]\n\t", *index ,node->value, node->left, node->right);
    int main_index = *index;

    if (node->left) {
        *index += 1;
        fprintf(fn, "%d: fl->%d\n\t [constraint = true, headclip = true]", main_index, *index);
        GenTreeNodes(fn, node->left, index);
    }
    if (node->right) {
        *index += 1;
        fprintf(fn, "%d: fr->%d\n\t", main_index, *index);
        GenTreeNodes(fn, node->right, index);
    }

    return SUCCESS;
}

int GenGraphAkinator(const TreeStruct *tree) {

    assert(tree);

    FILE *fn = fileopen(GRAPH_FILE_AKINATOR, WRITE);
    if (!fn)
        return FILE_OPEN_ERROR;

    int index = 0;
    fprintf(fn, "digraph G {\n\t");

    GenAkinatorNodes(fn, tree->root, &index);

    fprintf(fn, "\n");
    fprintf(fn, "}");

    fileclose(fn);

    system("dot " GRAPH_FILE_AKINATOR " -T png -o " GRAPH_FILE_AKINATOR_OUT);

    return SUCCESS;
}

static int GenAkinatorNodes(FILE *fn, const TreeNode *node, int *index) {

    assert(fn);
    assert(node);

    fprintf(fn, "%d [shape = Mrecord, style = filled, fillcolor = \"#99FFFF\", color = \"#331900\", label = \"" list_output_id "\"]\n\t", *index ,node->value);
    int main_index = *index;

    if (node->left) {
        *index += 1;
        fprintf(fn, "%d->%d [label = \"да\"]\n\t", main_index, *index);
        GenAkinatorNodes(fn, node->left, index);
    }
    if (node->right) {
        *index += 1;
        fprintf(fn, "%d->%d\n\t [label = \"нет\"]\n\t", main_index, *index);
        GenAkinatorNodes(fn, node->right, index);
    }

    return SUCCESS;
}