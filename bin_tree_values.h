#ifndef BIN_TREE_VAL
#define BIN_TREE_VAL

#include <stdio.h>

typedef char* Tree_t;
#define list_output_id "%s"

#define GRAPH_FILE_TREE "Graphviz/tree.dot"
#define GRAPH_FILE_TREE_OUT "Graphviz/tree.png"
#define GRAPH_FILE_AKINATOR "Graphviz/akinator.dot"
#define GRAPH_FILE_AKINATOR_OUT "Graphviz/akinator.png"
#define TREE_DATA "tree_data.txt"
#define AKINATOR_DATA "akinator_eng.txt"

const size_t MAX_STRLEN = 50;
const char START_WORD[] = "неизвестно кто";

struct TreeNode {
    Tree_t value;
    TreeNode *left;
    TreeNode *right;
};

struct TreeStruct {
    TreeNode *root;
    size_t size;
};

#endif