#ifndef CONSOLE_H
#define CONSOLE_H
#include <stddef.h>

#include <stdarg.h>
#include <stdbool.h>

extern void clearScreen(void);

extern void puts(const char * string);
extern void newLine();
extern void putc(char c);
extern void scroll();

extern int unsignedToString(char * buffer, size_t length, unsigned long long num, int base);
extern int signedToString(char * buffer, size_t length, signed long long num, int base);

extern void kprintf(void (*put)(void * ctx, char c), void * ctx, char const * fmt, ...);

#endif
