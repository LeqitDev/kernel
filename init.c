#include "console.h"
#include "DescriptorTable.h"

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

	set_entry(0, 0, 0, 0);
	set_entry(1, 0, 0x000FFFFF, (GDT_CODE_PL0));
	set_entry(2, 0, 0x000FFFFF, (GDT_DATA_PL0));
	set_entry(3, 0, 0x000FFFFF, (GDT_CODE_PL3));
	set_entry(4, 0, 0x000FFFFF, (GDT_DATA_PL3));

	set_gdt();

    reload_segments();

    char buffer[64] = "12345";
    StringPutBuffer writer = { buffer, 0 };
    kprintf(stringPutc, &writer, "%Lu", 0xFFFFFFFFFFFFFFFF);
    puts(buffer);

}
