#include <stdio.h>
#include "akinator.h"
#include "bin_tree_values.h"
#include "../MyLibraries/headers/func.h"
#include "../MyLibraries/headers/systemdata.h"
#include <assert.h>
#include "gen_graph_tree.h"
#include "bin_tree.h"
#include <string.h>
#include "../Stack_Functions/stack.h"
#include "akinator_func.h"
#include "../MyLibraries/headers/file_func.h"

static bool EndGame(Akinator *data, const bool is_changed);

int StartGame(Akinator *data) {

    assert(data);

    bool is_changed = 0;

    do {
        printf(BRIGHT_CYAN "Выберите режим:  1) Отгадывание \n\t\t 2) Определение \n\t\t 3) Различия \n\t\t 4) Вывести дерево" END_OF_COLOR "\n");
        #ifdef SAY
        system("say " VOICE " Выберите режим");
        #endif

        int mode = 0;
        GetMode(&mode);

        switch (mode) {
            case (GUESS): {
                AkinatorGuess(data, &is_changed);
                break;
            }
            case (DEFINITION): {
                AkinatorDefinition(data);
                break;
            }
            case (DIFFERENCE): {
                AkinatorDifference(data);
                break;
            }
            case (PRINT_TREE): {
                GenGraphAkinator(&data->tree);
                break;
            }
            default:
                printf(RED "Неверный режим" END_OF_COLOR "\n");
        }
    } while (!EndGame(data, is_changed));

    return SUCCESS;

}

static bool EndGame(Akinator *data, const bool is_changed) {

    assert(data);

    bool end_game= 0;

    printf(BRIGHT_CYAN "Сыграете ещё раз?" END_OF_COLOR "\n");
        #ifdef SAY
        system("say " VOICE " Сыграете еще раз?");
        #endif

    if (GetAnswer() ==  NO) {
        end_game = 1;
        if (is_changed) {
            printf(BRIGHT_CYAN "Сохранить данные?" END_OF_COLOR "\n");
            #ifdef SAY
            system("say " VOICE " Сохранить данные?");
            #endif

            if (GetAnswer() == YES) {
                FILE *fn = fileopen(AKINATOR_DATA, WRITE);
                if (!fn)
                    return FILE_OPEN_ERROR;
                PrintPreOrder(data->tree.root, fn);
                fileclose(fn);
            }
        }

        printf(BRIGHT_CYAN "До свидания!" END_OF_COLOR "\n");
        #ifdef SAY
        system("say " VOICE " До свидания!");
        #endif
    }

    return end_game;
}

int ReadFromFileBuf(Akinator *data, const char *filename) {

    assert(data);

    unsigned long len_of_file = filelen(filename);

    char *buf = readbuf(filename);
    if (!buf)
        return NO_MEMORY;

    fprintf(stderr, "Start Read\n");

    if (WorkWithBuf(data, buf, len_of_file) != SUCCESS) {
        free(buf);
        return ERROR;
    }

    free(buf);

    return SUCCESS;
}

int ReadFromFile(Akinator *data, const char *filename) {

    assert(data);
    assert(filename);

    FILE *fn = fileopen(filename, READ);
    if (!fn)
        return FILE_OPEN_ERROR;

    if (IsNewNode(fn)) {
        data->tree.root = TreeNodeNew(&data->tree, NULL);
        ReadNodeAkinator(data, data->tree.root, fn);
    }

    fileclose(fn);

    return SUCCESS;

}

int AkinatorCtor(Akinator *data) {

    assert(data);

    if (TreeRootCtor(&data->tree) != SUCCESS)
        return NO_MEMORY;

    if (StrBufCtor(&data->buf) != SUCCESS) {
        TreeRootDtor(&data->tree);
        return NO_MEMORY;
    }

    return SUCCESS;
}

int AkinatorDtor(Akinator *data) {

    assert(data);

    TreeRootDtor(&data->tree);
    StrBufDetor(&data->buf);

    return SUCCESS;
}

int SetStartWord(Akinator *data, const char *word) {

    assert(word);

    char *str = (char *) calloc (MAX_STRLEN, sizeof (char));
    if (!str)
        return NO_MEMORY;

    memcpy(str, word, strlen(word));

    data->tree.root->value = AkinatorPushStr(data, str);

    return SUCCESS;
}