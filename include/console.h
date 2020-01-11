#ifndef CONSOLE_H
#define CONSOLE_H
#include <stddef.h>
#include <stdarg.h>
#include <stdbool.h>

#define CONSOLE_COLOR_BLACK         0x0
#define CONSOLE_COLOR_BLUE          0x1
#define CONSOLE_COLOR_GREEN         0x2
#define CONSOLE_COLOR_CYAN          0x3
#define CONSOLE_COLOR_RED           0x4
#define CONSOLE_COLOR_MAGENTA       0x5
#define CONSOLE_COLOR_BROWN         0x6
#define CONSOLE_COLOR_LIGHT_GREY    0x7
#define CONSOLE_COLOR_DARK_GREY     0x8
#define CONSOLE_COLOR_LIGHT_BLUE    0x9
#define CONSOLE_COLOR_LIGHT_GREEN   0xA
#define CONSOLE_COLOR_LIGHT_CYAN    0xB
#define CONSOLE_COLOR_LIGHT_RED     0xC
#define CONSOLE_COLOR_LIGHT_MAGENTA 0xD
#define CONSOLE_COLOR_YELLOW        0xE
#define CONSOLE_COLOR_WHITE         0xF

extern int x;
extern int y;

extern void clearScreen(void);

extern void puts(const char * string);
extern void newLine();
extern void getLine(char * line, int row);
extern void putc(char c);
extern void scroll();

extern int strlen(char * string);
extern int contains(char * string, char * searchkey);
extern int charinstring(char * string, char character);

extern void setBgColor(int hexColorCode);
extern void setTextColor(int hexColorCode);
extern void displaycursor(int col, int row);

extern int unsignedToString(char * buffer, size_t length, unsigned long long num, int base);
extern int signedToString(char * buffer, size_t length, signed long long num, int base);

extern void kprintf(void (*put)(void * ctx, char c), void * ctx, char const * fmt, ...);
extern void printf(char * buffer, char const * fmt, ...);

#endif
