#include <stdio.h>
#include "str_buffer.h"
#include "../MyLibraries/headers/systemdata.h"
#include <assert.h>

const int START_SIZE = 8;
const int INCREASE = 2;

static int StrBufVerify(StringBuffer *buf);
static int StrBufResize(StringBuffer *buf);

int StrBufCtor(StringBuffer *buf) {

    assert(buf);

    buf->capacity = START_SIZE;
    buf->size = 0;
    buf->data = (char **) calloc (START_SIZE, sizeof (char *));
    if (!buf->data)
        return NO_MEMORY;

    return SUCCESS;
}

int StrBufDetor(StringBuffer *buf) {

    assert(buf);

    for (size_t i = 0; i < buf->size; i++)
        free(buf->data[i]);

    free(buf->data);
    buf->capacity = 0;
    buf->size = 0;

    return SUCCESS;
}

int PushStr(StringBuffer *buf, char *str) {

    assert(buf);
    assert(str);

    if (StrBufVerify(buf) != SUCCESS)
        return ERROR;

    if (buf->size + 1 >= buf->capacity) {
        if (StrBufResize(buf) != SUCCESS)
            return NO_MEMORY;
    }

    buf->data[buf->size++] = str;

    return SUCCESS;
}

static int StrBufVerify(StringBuffer *buf) {

    assert(buf);

    bool error = 0;

    if (!buf->data) {
        printf(RED "Null data pointer" END_OF_COLOR "\n");
        error = 1;
    }

    if (buf->size >= buf->capacity) {
        printf(RED "Incorrect size or capacity" END_OF_COLOR "\n");
        error = 0;
    }

    if (error)
        return ERROR;

    return SUCCESS;
}

int PrintStrBuf(StringBuffer *buf) {

    assert(buf);

    if (StrBufVerify(buf) != SUCCESS)
        return ERROR;

    printf("data: %p\ncapacity: %lu\nsize: %lu\n", buf->data, buf->capacity, buf->size);
    for (size_t i = 0; i < buf->size; i++) {
        printf("%s ", buf->data[i]);
    }

    return SUCCESS;
}

static int StrBufResize(StringBuffer *buf) {

    assert(buf);

    if (StrBufVerify(buf) != SUCCESS)
        return ERROR;

    buf->capacity *= INCREASE;
    buf->data = (char **) realloc (buf->data, buf->capacity * sizeof (char *));
    if (!buf->data)
        return NO_MEMORY;

    return SUCCESS;
}