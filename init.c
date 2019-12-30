#include "include/console.h"
#include "include/GlobalDescriptorTable.h"
#include "include/InterruptDescriptorTable.h"

void init(void)
{
    clearScreen();

    init_gdt();
    init_idt();

    char buffer[64] = "XXXX";
    printf(buffer, "%s", "Hello World!");
    puts(buffer);
    while (1);
}
