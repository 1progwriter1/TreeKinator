#ifndef AKINATOR_TREE
#define AKINATOR_TREE

#include "str_buffer.h"

#define SAY
#define VOICE ""

#include "bin_tree_values.h"

enum Answers {
    YES       = 0,
    NO        = 1,
    NO_ANSWER = 2,
};

enum Modes {
    GUESS      = 1,
    DEFINITION = 2,
    DIFFERENCE = 3,
    PRINT_TREE = 4,
};

struct Akinator {
    TreeStruct tree;
    StringBuffer buf;
};

int StartGame(Akinator *data);
int ReadFromFile(Akinator *data, const char *filename);
int AkinatorCtor(Akinator *data);
int AkinatorDtor(Akinator *data);
char *AkinatorPushStr(Akinator *data, char *str);
int SetStartWord(Akinator *data, const char *word);
int ReadFromFileBuf(Akinator *data, const char *filename);

#endif