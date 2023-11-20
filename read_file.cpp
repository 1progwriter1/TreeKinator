#include <stdio.h>
#include "read_file.h"
#include "../MyLibraries/headers/file_func.h"
#include "akinator.h"
#include <assert.h>
#include "bin_tree.h"
#include "../MyLibraries/headers/systemdata.h"
#include <string.h>

static int ReadNodeFile(Akinator *data, TreeNode *node, char *buf, const unsigned long len_of_file);
static char *GetStringBuf(char *buf);

int ReadAkinatorFile(Akinator *data, const char *filename) {

    assert(data);
    assert(filename);

    char *buf = readbuf(filename);
    unsigned long len_of_file = filelen(filename);

    data->tree.root = TreeNodeNew(&data->tree, NULL);
    if (!data->tree.root)
        return NO_MEMORY;

    for (size_t i = 0; i < len_of_file; i++) {
        if (buf[i] == '(') {
            if (ReadNodeFile(data, data->tree.root, buf + i + 1, len_of_file) != SUCCESS) {
                free(buf);
                return ERROR;
            }
            else {
                break;
            }
        }
    }

    free(buf);

    return SUCCESS;
}

static int ReadNodeFile(Akinator *data, TreeNode *node, char *buf, const unsigned long len_of_file) {

    assert(node);
    assert(buf);

    char *str = GetStringBuf(buf);
    if (!str)
        return ERROR;
    node->value = AkinatorPushStr(data, str);

    for (size_t i = 0; i < len_of_file; i++)
        if (buf[i] == ' ')
            continue;
        else if (buf[i] == '(')
            if (ReadNodeFile(data, node->left, buf + i + 1, len_of_file) != SUCCESS)
                return ERROR;
            else
                break;
        else if (len_of_file - i >= 3 && strncmp("nil", buf + i, 3) == 0)
            node->left = NULL;
        else
            return ERROR;

    for (size_t i = 0; i < len_of_file; i++) {
        if (buf[i] == ' ') {
            continue;
        }
        else if (buf[i] == '(') {
            if (ReadNodeFile(data, node->right, buf + i + 1, len_of_file) != SUCCESS) {
                return ERROR;
            }
            else  {
                buf += i + 1;
                break;
            }
        }
        else if (len_of_file - i >= 3 && strncmp("nil", buf + i, 3) == 0) {
                node->right = NULL;
                buf += i + 1;
            }
        else {
            return ERROR;
        }
    }

    return SUCCESS;
}

/* static bool IfNodeExist(char *buf, int *index, const unsigned long len_of_file) {

    assert(buf);
    assert(index);

    for (size_t i = 0; i < len_of_file; i++) {

        *index = (int) i;

        if (buf[i] == ' ')
            continue;

        if (buf[i] == '(')
            return 1;

        if (len_of_file - i >= 3 && buf[i] == 'n' && buf[i + 1] == 'i' && buf[i + 2] == 'l')
            return 0;

        return 0;
    }

    return 0;
} */

char *EndNodeBuf(char *buf, const unsigned long len_of_file) {

    assert(buf);

    size_t i = 0;
    while (buf[i] != ')' && i++ < len_of_file)
        ;

    return buf + i;
}

static char *GetStringBuf(char *buf) {

    assert(buf);

    char *str = (char *) calloc (MAX_STRLEN, sizeof (char));
    if (!str)
        return NULL;

    int num_of_elem_read = 0;
    if (sscanf(buf, " \"%[^\"] %n", str, &num_of_elem_read) != 1) {
        free(str);
        return NULL;
    }

    buf += num_of_elem_read + 1;

    return str;
}