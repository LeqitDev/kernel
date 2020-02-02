#include "console.h"
#include "intr.h"
#include "tasks.h"
#include <pmm.h>

void init(struct mb_info* mbInfo)
{
    clearScreen();

    init_pmm(mbInfo);

    init_gdt();
    init_idt();

    init_multitasking(mbInfo);
    while (1);
}
