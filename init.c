#include "include/console.h"
#include "include/GlobalDescriptorTable.h"
#include "include/InterruptDescriptorTable.h"
#include "include/Tasks.h"

void init(void)
{
    clearScreen();

    init_gdt();
    init_idt();
    init_multitasking();

    while (1);
}
