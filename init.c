#include "console.h"

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

void init(void)
{
	clearScreen();
    char buffer[64] = "12345";
    StringPutBuffer writer = { buffer, 0 };
    kprintf(stringPutc, &writer, "%L", 0xFFFFFFFFFFFFFFFFFFFFF);
    puts(buffer);
	/*int numbers[27] = {1, 1, 1, 1, 1, 1, -1, -1, -1, -1, -1, -1, 0, 0, 0, 10, 10, 10, 16, 16, 16, -31, -31, -31, 4294967295, 4294967295, 4294967295};
	int base[27] = {10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 2, 10, 16, 2, 10, 16, 2, 10, 16, 2, 10, 16, 2, 10, 16};
	int length[27] = {0, 1, 2, 3, 5, 100, 0, 1, 2, 3, 5, 100, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64};
    char * numbers_string[27] = {"1", "1", "1", "1", "1", "1", "-1", "-1", "-1", "-1", "-1", "-1", "0", "0", "0", "10", "10", "10", "16", "16", "16", "-31", "-31", "-31", "4294967295", "4294967295", "4294967295"};
    char * base_string[27] = {"10", "10", "10", "10", "10", "10", "10", "10", "10", "10", "10", "10", "2", "10", "16", "2", "10", "16", "2", "10", "16", "2", "10", "16", "2", "10", "16"};
    char * length_string[27] = {"0", "1", "2", "3", "5", "100", "0", "1", "2", "3", "5", "100", "64", "64", "64", "64", "64", "64", "64", "64", "64", "64", "64", "64", "64", "64", "64"};
    char * Digits[27] = {"0", "1", "2", "3", "4", "5", "6", "7", "8", "9", "10", "11", "12", "13", "14", "15", "16", "17", "18", "19", "20", "21", "22", "23", "24", "25", "26"};
    bool single = true;
    int i = 0;
    int end = 24;

    if (single == true)
    {
        char buff[64] = "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX";
        NumToString(buff, sizeof(buff), numbers[i], base[i]);
        newLine();
        puts(Digits[i]);
        puts(": ");
        puts(numbers_string[i]);
        putc('|');
        puts(base_string[i]);
        putc('|');
        puts(length_string[i]);
        putc('|');
        puts(buff);
        newLine();
    }

	for (; i < end && single == false; i++)
    {
	    char buff[64] = "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX";
	    NumToString(buff, sizeof(buff), numbers[i], base[i]);
	    puts(Digits[i]);
	    puts(": ");
	    puts(numbers_string[i]);
	    putc('|');
        puts(base_string[i]);
        putc('|');
        puts(length_string[i]);
        putc('|');
	    puts(buff);
	    newLine();
    }*/
}