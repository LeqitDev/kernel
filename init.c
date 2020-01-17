#include "include/console.h"
#include "include/GlobalDescriptorTable.h"
#include "include/InterruptDescriptorTable.h"

void init(void)
{
    clearScreen();

    init_gdt();
    init_idt();

    while (1);
}
