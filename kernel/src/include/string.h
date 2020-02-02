//
// Created by marku on 02.02.2020.
//

#ifndef CCOS_STRING_H
#define CCOS_STRING_H

#include "stddef.h"

static inline void* memset(void* buf, int c, int n)
{
    unsigned char* p = buf;

    while (n--) {
        *p++ = c;
    }

    return buf;
}

static inline void* memcpy(void* dest, const void* src, size_t n)
{
    unsigned char* d = dest;
    const unsigned char* s = src;

    while (n--) {
        *d++ = *s++;
    }

    return dest;
}


#endif //CCOS_STRING_H
