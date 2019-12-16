#ifndef CONSOLE_H
#define CONSOLE_H
#include <stddef.h>

typedef __builtin_va_list       va_list;
#define va_start(ap, X)         __builtin_va_start(ap, X)
#define va_arg(ap, type)        __builtin_va_arg(ap, type)
#define va_end(ap)              __builtin_va_end(ap)

typedef int bool;
#define true 1
#define false 0

extern void clearScreen(void);

extern void puts(const char * string);
extern void newLine();
extern void putc(char c);
extern void scroll();

extern int unsignedToString(char * buffer, size_t length, unsigned long long num, int base);
extern int signedToString(char * buffer, size_t length, signed long long num, int base);

extern void kprintf(void (*put)(void * ctx, char c), void * ctx, char const * fmt, ...);

#endif