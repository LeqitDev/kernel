#include "console.h"
#include <stdarg.h>

struct __attribute__((packed)) VideoChar { char c; char color; };

struct VideoChar * video = (struct VideoChar*) 0xb8000;
int x = 0;
int y = 0;

typedef struct
{
    char * text;
    size_t len;
} StringPutBuffer;

static void stringPutc(void * ctx, char c)
{
    StringPutBuffer * buf = (StringPutBuffer *)ctx;
    buf->text[buf->len] = c;
    buf->len += 1;
    buf->text[buf->len] = 0;
}

void clearScreen(void)
{
	struct VideoChar vchar;
	vchar.c = ' ';
	vchar.color = 0x0f;
	for(int i = 0; i <= 2000; i++)
	{
		video[i] = vchar;
	}
}

void puts(const char * string)
{
	for (int i = 0; string[i] != 0; i++)
	{
	    if (string[i] == '\n') {
            i++;
            y++;
            x = 0;
	    } else {
            struct VideoChar vchar;
            vchar.c = string[i];
            vchar.color = 0x0f;

            video[x + 80 * y] = vchar;

            x++;
	    }
	}
	if (y == 24 || x / 80 >= 24)
		scroll();
}

void putc(char c)
{
    if (c == '\n') {
        y++;
        x = 0;
    } else {
        struct VideoChar vchar;
        vchar.c = c;
        vchar.color = 0x0f;

        video[x + 80 * y] = vchar;

        x++;
    }
}

void newLine()
{
    x = 0;
    y++;
}

void scroll(void)
{
	for (int y = 0; y < 24; y++)
	{
		for (int x = 0; x < 80; x++)
		{
			video[x + 80 * y] = video[x + 80 * (y + 1)];
		}
	}
	x = 0;
}

int unsignedToString(char * buffer, size_t length, unsigned long long num, int base)
{
    if (base < 2 && base > 16)
    {
        return -1;
    }

    int len = 0;
    const char * Digits = "0123456789ABCDEF";

    unsigned long long helper = num;

    for (; helper > 0; len++)
    {
        helper /= base;
    }

    if (len+1 > (int) length)
    {
        return -1;
    }

    if(num == 0)
    {
        buffer[0] = Digits[0];
    }

    for (len = 0; num > 0; len++)
    {
        buffer[len] = Digits[num % base];
        num /= base;
    }

    //reverse
    for (int x = 0; x < len/2; x++)
    {
        char temp = buffer[x];
        buffer[x] = buffer[len-x-1];
        buffer[len-x-1] = temp;
    }

    buffer[len] = '\0';
    return len;
}

int signedToString(char * buffer, size_t length, signed long long num, int base)
{
    int neg = 0;
    if (num < 0) {buffer[0] = '-'; num = -num; buffer++; neg++;}
    int len = unsignedToString(buffer, length, num, base);
    len += neg;
    return len;
}

int genprintf(void (*put)(void * ctx, char c), void * ctx, char const * fmt, va_list ArgList)
{
    int len = 0;
    while (*fmt)
    {
        len++;
        if (*fmt == '%')
        {
            fmt++;
            if (*fmt == 'i')
            {
                fmt++;
                char Buff[64] = "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX";
                int length;
                if (*fmt == 'u')
                {
                    fmt++;
                    length = unsignedToString(Buff, sizeof(Buff), va_arg(ArgList, unsigned int), 10);
                }
                else
                {
                    length = signedToString(Buff, sizeof(Buff), va_arg(ArgList, int), 10);
                }
                len += length - 1;
                for (int i = 0; i < length; i++) {
                    if (put != NULL) put(ctx, Buff[i]);
                }
            }
            else if (*fmt == 'l')
            {
                fmt++;
                char Buff[64] = "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX";
                int length;
                if (*fmt == 'u')
                {
                    fmt++;
                    length = unsignedToString(Buff, sizeof(Buff), va_arg(ArgList, long), 10);
                }
                else
                {
                    length = signedToString(Buff, sizeof(Buff), va_arg(ArgList, long), 10);
                }
                len += length - 1;
                for (int i = 0; i < length; i++) {
                    if (put != NULL) put(ctx, Buff[i]);
                }
            }
            else if (*fmt == 'L')
            {
                fmt++;
                char Buff[64] = "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX";
                int length;
                if (*fmt == 'u')
                {
                    fmt++;
                    length = unsignedToString(Buff, sizeof(Buff), va_arg(ArgList, unsigned long long), 10);
                }
                else
                {
                    length = signedToString(Buff, sizeof(Buff), va_arg(ArgList, long long), 10);
                }
                len += length - 1;
                for (int i = 0; i < length; i++) {
                    if (put != NULL) put(ctx, Buff[i]);
                }
            }
            else if (*fmt == 'b')
            {
                fmt++;
                char Buff[64] = "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX";
                int length;
                if (*fmt == 'u')
                {
                    fmt++;
                    length = unsignedToString(Buff, sizeof(Buff), va_arg(ArgList, unsigned int), 2);
                }
                else
                {
                    length = signedToString(Buff, sizeof(Buff), va_arg(ArgList, int), 2);
                }
                len += length - 1;
                for (int i = 0; i < length; i++) {
                    if (put != NULL) put(ctx, Buff[i]);
                }
            }
            else if (*fmt == 'b')
            {
                fmt++;
                char Buff[64] = "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX";
                int length;
                if (*fmt == 'u')
                {
                    fmt++;
                    length = unsignedToString(Buff, sizeof(Buff), va_arg(ArgList, unsigned int), 8);
                }
                else
                {
                    length = signedToString(Buff, sizeof(Buff), va_arg(ArgList, int), 8);
                }
                len += length - 1;
                for (int i = 0; i < length; i++) {
                    if (put != NULL) put(ctx, Buff[i]);
                }
            }
            else if (*fmt == 'h')
            {
                fmt++;
                char Buff[64] = "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX";
                int length;
                if (*fmt == 'u')
                {
                    fmt++;
                    length = unsignedToString(Buff, sizeof(Buff), va_arg(ArgList, unsigned int), 16);
                }
                else
                {
                    length = signedToString(Buff, sizeof(Buff), va_arg(ArgList, int), 16);
                }
                len += length - 1;
                if (put != NULL) put(ctx, '0');
                if (put != NULL) put(ctx, 'x');
                for (int i = 0; i < length; i++) {
                    if (put != NULL) put(ctx, Buff[i]);
                }
            }
            else if (*fmt == 'c')
            {
                fmt++;
                char c = va_arg(ArgList, int);
                if (put != NULL) put(ctx, c);
                len++;
            }
            else if (*fmt == 's')
            {
                fmt++;
                int i = 0;
                char const * Buff = va_arg(ArgList, char const *);
                for (; Buff[i] != 0; i++) {
                    if (put != NULL) put(ctx, Buff[i]);
                }
                len += i - 1;
            }
        }
        else
        {
            if (put != NULL) put(ctx, *fmt);
            fmt++;
        }
    }
    return len;
}

void kprintf(void (*put)(void * ctx, char c), void * ctx, char const * fmt, ...)
{
    va_list vaList;
    va_start(vaList, fmt);
    genprintf(put, ctx, fmt, vaList);
    va_end(vaList);
}

void printf(char * buffer, char const * fmt, ...) {
    va_list vaList;
    va_start(vaList, fmt);
    StringPutBuffer writer = { buffer, 0 };
    genprintf(stringPutc, &writer, fmt, vaList);
    va_end(vaList);
}
