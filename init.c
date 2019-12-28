#include "console.h"
#include "GlobalDescriptorTable.h"
#include "InterruptDescriptorTable.h"

void init(void)
{
    clearScreen();

    init_gdt();
    init_idt();

    asm volatile("int $0x0");

    char buffer[64] = "XXXX";
    printf(buffer, "%s", "Hello World!");
    puts(buffer);
}
