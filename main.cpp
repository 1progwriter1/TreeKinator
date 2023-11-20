#include <stdio.h>
#include "../MyLibraries/headers/systemdata.h"
#include "bin_tree.h"
#include "../MyLibraries/headers/func.h"
#include "akinator.h"
#include <string.h>
#include <assert.h>

int main(int argc, const char *argv[]) {

    assert(argv);

    const char *filename = AKINATOR_DATA;
    bool read_from_file = 0;

    if (argc > 1) {
        if (strcmp(argv[1], "--file") == 0) {
            read_from_file = 1;
            if (argc > 2) {
                filename = argv[2];
            }
        }
        else {
            printf(RED "Incorrect argument" END_OF_COLOR "\n");
            return ERROR;
        }
    }

    Akinator data = {};
    if (AkinatorCtor(&data) != SUCCESS)
        return ERROR;

    if (read_from_file) {
        ReadFromFile(&data, filename);
    }
    else {
        SetStartWord(&data, START_WORD);
    }

    StartGame(&data);

    AkinatorDtor(&data);

    return SUCCESS;
}