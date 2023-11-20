#ifndef STR_BUFFER_TREE
#define STR_BUFFER_TREE

#include <stdio.h>

struct StringBuffer {
    char **data;
    size_t size;
    size_t capacity;
};

int PrintStrBuf(StringBuffer *buf);
int StrBufCtor(StringBuffer *buf);
int StrBufDetor(StringBuffer *buf);
int PushStr(StringBuffer *buf, char *str);

#endif