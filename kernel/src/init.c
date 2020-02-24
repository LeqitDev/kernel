#include "console.h"
#include "intr.h"
#include "tasks.h"
#include <mm.h>

void init(struct mb_info* mbInfo)
{
    clearScreen();

    init_pmm(mbInfo);

    init_vmm();

    init_gdt();
    init_idt();

    init_multitasking(mbInfo);
    while (1);
}
