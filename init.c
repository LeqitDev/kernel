#include "console.h"
#include "GlobalDescriptorTable.h"
#include "InterruptDescriptorTable.h"

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
    init_gdt();
    init_idt();
	clearScreen();

    asm volatile("int $0x0");

    char buffer[64] = "12345";
    StringPutBuffer writer = { buffer, 0 };
    kprintf(stringPutc, &writer, "%s", "Hello World!1");
    puts(buffer);

}
