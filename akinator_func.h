#ifndef AKINATOR_FUNC_TREE
#define AKINATOR_FUNC_TREE

#include "akinator.h"
#include "../Stack_Functions/stack.h"

int GetMode(int *mode);
int GetAnswer();
char *GetString();
char *GetStringFile(FILE *fn);
int ReadNodeAkinator(Akinator *data, TreeNode *node, FILE *fn);
int AkinatorGuess(Akinator *data, bool *is_changed);
int AkinatorDifference(Akinator *data);
int AkinatorDefinition(Akinator *data);
int GetNewObject(Akinator *data, TreeNode *node);
int FindObject(TreeNode *node, Stack *stk, const char *word);
int PrintDefinition(Akinator *data, Stack *stk, const size_t start_node);
int CompareObjects(Akinator *data, Stack *stk_fst, Stack *stk_snd, const char *wrd_fst, const char *wrd_snd);
char *AkinatorPushStr(Akinator *data, char *str);
bool IsNewNode(FILE *fn);
void EndNode(FILE *fn);
int CompareWithoutSpaces(const char *fst, const char *snd);
int WorkWithBuf(Akinator *data ,char *buf, const unsigned long len_of_file);
int ReadNodeFileBuf(Akinator *data, TreeNode *node, char *buf, const unsigned long len_of_file, size_t *index);
void EndNodeBuf(char *buf, size_t *index, const unsigned long len_of_file);
void SkipSpacesBuf(char *buf, size_t *index, const unsigned long len_of_file);
void SkipSpaces(FILE *fn);
char *ReadStr(char *buf, size_t *index, const unsigned long len_of_file);

#endif