#include <stdio.h>
#include <assert.h>
#include "akinator_func.h"
#include <string.h>
#include "../MyLibraries/headers/func.h"
#include "bin_tree_values.h"
#include "akinator.h"
#include "bin_tree.h"
#include <ctype.h>

const size_t MAX_COMMAND_LEN = 50;
const size_t MAX_COl_SPACES = 5;

char *AkinatorPushStr(Akinator *data, char *str) {

    assert(data);

    if (PushStr(&data->buf, str) != SUCCESS)
        return NULL;

    return str;
}

int AkinatorGuess(Akinator *data, bool *is_changed) {

    assert(data);


    TreeNode *current_node = data->tree.root;
    while (current_node->left && current_node->right) {
        printf("%s?\n", current_node->value);
        if (GetAnswer() == YES)
            current_node = current_node->left;
        else
            current_node = current_node->right;
    }

    printf(BRIGHT_CYAN "Это %s?" END_OF_COLOR "\n", current_node->value);
    #ifdef SAY
    char command[MAX_COMMAND_LEN] = "";
    snprintf(command, MAX_COMMAND_LEN, "say " VOICE "Это %s?", current_node->value);
    system(command);
    #endif

    if (GetAnswer() == YES) {
        printf(BRIGHT_GREEN "Я так и знал!" END_OF_COLOR "\n");
        #ifdef SAY
        system("say " VOICE " Я так и знал!");
        #endif
        return SUCCESS;
    }
    else {
        GetNewObject(data, current_node);
        *is_changed = 1;
    }

    return SUCCESS;
}

int AkinatorDefinition(Akinator *data) {

    assert(data);

    char word[MAX_STRLEN]  = "";
    printf(BRIGHT_CYAN "Введите слово:" END_OF_COLOR "\n");
    #ifdef SAY
    system("say " VOICE " Введите слово");
    #endif
    scanf("%s", word);

    Stack stk = {};
    STACK_CTOR(stk);

    if (FindObject(data->tree.root, &stk, word) != SUCCESS) {
        printf(RED "Объект не найден" END_OF_COLOR "\n");
        #ifdef SAY
        system("say " VOICE " Объект не найден");
        #endif
        StackDtor(&stk);
        return ERROR;
    }

    printf(BRIGHT_CYAN "%s это: " END_OF_COLOR, word);
    PrintDefinition(data, &stk, 0);

    StackDtor(&stk);

    return SUCCESS;
}

int AkinatorDifference(Akinator *data) {

    assert(data);

    Stack stk_fst = {};
    Stack stk_snd = {};
    STACK_CTOR(stk_fst);
    STACK_CTOR(stk_snd);

    char wrd_fst[MAX_STRLEN] = "";
    char wrd_snd[MAX_STRLEN] = "";

    printf(BRIGHT_CYAN "Введите первое слово:" END_OF_COLOR "\n");
    scanf("%s", wrd_fst);

    if (FindObject(data->tree.root, &stk_fst, wrd_fst) != SUCCESS) {
        printf(RED "Объект %s не найден" END_OF_COLOR "\n", wrd_fst);
        #ifdef SAY
        system("say " VOICE " Объект не найден");
        #endif
        return ERROR;
    }

    printf(BRIGHT_CYAN "Введите второе слово:" END_OF_COLOR "\n");
    scanf("%s", wrd_snd);

    if (FindObject(data->tree.root, &stk_snd, wrd_snd) != SUCCESS) {
        printf(RED "Объект %s не найден" END_OF_COLOR "\n", wrd_snd);
        #ifdef SAY
        system("say " VOICE " Объект не найден");
        #endif
        return ERROR;
    }

    bool error = 0;
    if (CompareObjects(data, &stk_fst, &stk_snd, wrd_fst, wrd_snd) != SUCCESS)
        error = 1;

    StackDtor(&stk_fst);
    StackDtor(&stk_snd);

    if (error)
        return ERROR;

    return SUCCESS;
}

int CompareObjects(Akinator *data, Stack *stk_fst, Stack *stk_snd, const char *wrd_fst, const char *wrd_snd) {

    assert(data);
    assert(stk_fst);
    assert(stk_snd);
    assert(wrd_fst);
    assert(wrd_snd);

    size_t index = 0;
    if (stk_fst->data[index] == stk_snd->data[index]) {
        printf(BRIGHT_CYAN "%s и %s схожи по следующим признакам: "  END_OF_COLOR, wrd_fst, wrd_snd);
        TreeNode *ptr = data->tree.root;
        while (stk_fst->data[index] == stk_snd->data[index] && index < (size_t) stk_fst->size && index < (size_t) stk_snd->size) {
            if (!ptr->left || !ptr->right) {
                printf(RED "Error while printing definition" END_OF_COLOR "\n");
                return ERROR;
            }
            if (stk_fst->data[index] == 1) {
                printf("%s, ", ptr->value);
                ptr = ptr->left;
            }
            else {
                printf("не %s, ", ptr->value);
                ptr = ptr->right;
            }
            index++;
        }
        printf("\b\b \n");
    }
    printf(BRIGHT_CYAN "Различаются:" END_OF_COLOR "\n");
    if (index < (size_t) stk_fst->size) {
        printf(BRIGHT_CYAN "%s: " END_OF_COLOR, wrd_fst);
        if (PrintDefinition(data, stk_fst, index) != SUCCESS)
            return ERROR;
    }
    if (index < (size_t) stk_snd->size) {
        printf(BRIGHT_CYAN "%s: " END_OF_COLOR, wrd_snd);
        if (PrintDefinition(data, stk_snd, index) != SUCCESS)
            return ERROR;
    }

    return SUCCESS;

}

int FindObject(TreeNode *node, Stack *stk, const char *word) {

    assert(node);
    assert(stk);
    assert(word);

    Elem_t num = 0;

    if (node->left) {
        StackPush(stk, 1);
        if (FindObject(node->left, stk, word) == SUCCESS) {
            return SUCCESS;
        }
        StackPop(stk, &num);
    }

    if (node->right) {
        StackPush(stk, 0);
        if (FindObject(node->right, stk, word) == SUCCESS) {
            return SUCCESS;
        }
        StackPop(stk, &num);
    }

    if (!node->left && !node->right)
        if (CompareWithoutSpaces(word, node->value) == 0)
            return SUCCESS;

    return ERROR;
}

int GetNewObject(Akinator *data, TreeNode *node) {

    assert(node);
    assert(data);

    node->right = TreeNodeNew(&data->tree, node->value);
    if (!node->right)
        return NO_MEMORY;

    printf(BRIGHT_CYAN "А кто это был?" END_OF_COLOR "\n");
    #ifdef SAY
    system("say " VOICE " А кто это был?");
    #endif

    node->left = TreeNodeNew(&data->tree, AkinatorPushStr(data, GetString()));

    printf(BRIGHT_CYAN "Чем %s отличается от %s?" END_OF_COLOR "\n", node->left->value, node->right->value);
    #ifdef SAY
    system("say " VOICE " Чем новый объект отличается от старого?");
    #endif

    node->value = AkinatorPushStr(data, GetString());

    return SUCCESS;
}

int PrintDefinition(Akinator *data, Stack *stk, const size_t start_node) {

    assert(stk);
    assert(data);

    TreeNode *ptr = data->tree.root;
    for (size_t i = 0; i < (size_t) stk->size; i++) {
        if (!ptr->left || !ptr->right) {
            printf(RED "Error while printing definition" END_OF_COLOR "\n");
            return ERROR;
        }
        if (stk->data[i] == 1) {
            if (i >= start_node)
                printf("%s, ", ptr->value);
            ptr = ptr->left;
        }
        else {
            if (i >= start_node)
                printf("не %s, ", ptr->value);
            ptr = ptr->right;
        }
    }
    printf(END_OF_COLOR "\b\b \n");

    return SUCCESS;
}

int ReadNodeAkinator(Akinator *data, TreeNode *node, FILE *fn) {

    assert(data);
    assert(fn);
    assert(node);

    char *str = GetStringFile(fn);
    if (!str)
        return ERROR;

    node->value = AkinatorPushStr(data, str);

    if (IsNewNode(fn)) {
        node->left = TreeNodeNew(&data->tree, NULL);
        if (ReadNodeAkinator(data, node->left, fn) != SUCCESS)
            return ERROR;
    }
    else {
        node->left = NULL;
    }

    if (IsNewNode(fn)) {
        node->right = TreeNodeNew(&data->tree, NULL);
        if (ReadNodeAkinator(data, node->right, fn) != SUCCESS)
            return ERROR;
    }
    else {
        node->right = NULL;
    }

    EndNode(fn);

    return SUCCESS;
}

char *GetString() {

    char *str = (char *) calloc (MAX_STRLEN, sizeof (char));
    if (!str)
        return NULL;
    bool correct = 1;
    do {
        correct = 1;
        buf_clear();
        scanf("%[^\n]", str);
        if (strncasecmp(str, "не", 2) == 0 && str[2] == ' ') {
            printf(RED "Не начинайте признак с частицы \"не\"" END_OF_COLOR "\n");
            #ifdef SAY
            system("say " VOICE " Не начинайте признак с частицы не");
            #endif
            correct = 0;
        }
    } while (!correct);

    return str;
}

bool IsNewNode(FILE *fn) {

    assert(fn);

    char str[MAX_STRLEN] = "";
    fscanf(fn, "%s", str);

    return strcasecmp(str, "(") == 0;
}

void EndNode(FILE *fn) {

    assert(fn);

    char str[MAX_STRLEN] = "";
    fscanf(fn, "%s", str);
}

char *GetStringFile(FILE *fn) {

    assert(fn);

    char *str = (char *) calloc (MAX_STRLEN, sizeof (char));
    if (!str)
        return NULL;

    SkipSpaces(fn);

    fscanf(fn, "%[^\n()nil]", str);

    return str;
}

void SkipSpaces(FILE *fn) {

    assert(fn);

    char str[MAX_COl_SPACES] = "";
    fscanf(fn, "%[ ]", str);
}

int GetAnswer() {

    char answer[MAX_STRLEN] = "";

    bool correct = 1;
    do {
        buf_clear();
        correct = 1;
        scanf("%s", answer);
        if (strcasecmp(answer, "да") == 0) {
            return YES;
        }
        else if (strcasecmp(answer, "нет") == 0)
                return NO;
        else {
            printf(BRIGHT_CYAN "Вам нужно ответить \"да\" или \"нет\"" END_OF_COLOR "\n");
            #ifdef SAY
            system("say " VOICE " Вам нужно ответить да или нет");
            #endif
            correct = 0;
        }
    } while (!correct);

    return NO_ANSWER;
}

int GetMode(int *mode) {

    assert(mode);

    int correct = 0;
    do {
        correct = 1;
        if (scanf("%d", mode) != 1) {
            printf(RED "Некорректный ввод. Попробуйте ещё раз:\n" END_OF_COLOR);
            #ifdef SAY
            system("say " VOICE " Некорректный ввод попробуйте еще раз");
            #endif
            buf_clear();
            correct = 0;
        }
        else if (*mode < 1 || *mode > 4) {
            printf(RED "Неверный режим. Введите ещё раз:\ns" END_OF_COLOR);
            #ifdef SAY
            system("say " VOICE " Неверный режим. Введите ещё раз");
            #endif
            correct = 0;
        }
    } while (!correct);

    return SUCCESS;
}

int WorkWithBuf(Akinator *data, char *buf, const unsigned long len_of_file) {

    assert(data);
    assert(buf);

    size_t index = 0;
    if (index < len_of_file && buf[index++] == '(') {
        ReadNodeFileBuf(data ,data->tree.root, buf, len_of_file, &index);
        return SUCCESS;
    }

    return ERROR;
}

int ReadNodeFileBuf(Akinator *data, TreeNode *node, char *buf, const unsigned long len_of_file, size_t *index) {

    assert(node);
    assert(buf);
    assert(index);
    assert(data);

    SkipSpacesBuf(buf, index, len_of_file);

    char *str = ReadStr(buf, index, len_of_file);
    if (!str)
        return ERROR;

    node->value = AkinatorPushStr(data, str);

    if (buf[*index++] == '(') {
        node->left = TreeNodeNew(&data->tree, NULL);
        ReadNodeFileBuf(data, node->left, buf, len_of_file, index);
    }
    else {
        node->left = NULL;
        *index += 2;
    }

    if (buf[*index++] == '(') {
        node->right = TreeNodeNew(&data->tree, NULL);
        ReadNodeFileBuf(data, node->right, buf, len_of_file, index);
    }
    else {
        node->right = NULL;
        *index += 2;
    }

    EndNodeBuf(buf, index, len_of_file);

    return SUCCESS;
}

void EndNodeBuf(char *buf, size_t *index, const unsigned long len_of_file) {

    assert(buf);
    assert(index);

    while (*index < len_of_file && buf[*index] != ')')
        (*index)++;
    (*index)++;
}

void SkipSpacesBuf(char *buf, size_t *index, const unsigned long len_of_file) {

    assert(buf);
    assert(index);

    while (*index < len_of_file && buf[*index] == ' ')
        (*index)++;
}

char *ReadStr(char *buf, size_t *index, const unsigned long len_of_file) {

    assert(buf);
    assert(index);

    size_t begin_str = *index;
    size_t num_of_spaces = 0;

    while (*index < len_of_file && buf[*index] != '(' && buf[*index] != 'n') {
        if (buf[*index] == ' ')
            num_of_spaces++;
        else
            num_of_spaces = 0;
        (*index)++;
    }
    char tmp = buf[*index - num_of_spaces];
    buf[*index - num_of_spaces] = '\0';
    char *str = (char *) calloc (*index - begin_str - num_of_spaces, sizeof (char));
    if (!str)
        return NULL;

    sscanf(buf + begin_str, "%s", str);
    buf[*index - num_of_spaces] = tmp;

    return str;

}

int CompareWithoutSpaces(const char *fst, const char *snd) {

    assert(fst);
    assert(snd);

    size_t ind_fst = 0;
    size_t ind_snd = 0;

    if (fst == snd)
        return 0;

    while (fst[ind_fst] != '\0' && snd[ind_snd] != '\0') {
        if (fst[ind_fst] == ' ') {
            ind_fst++;
            continue;
        }
        if (snd[ind_snd] == ' ') {
            ind_snd++;
            continue;
        }
        if (fst[ind_fst] != snd[ind_snd]) {
            return fst[ind_fst] - snd[ind_snd];
        }
        else {
            ind_fst++;
            ind_snd++;
        }
    }
    return 0;
}