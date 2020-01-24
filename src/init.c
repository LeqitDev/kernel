#include "console.h"
#include "intr.h"
#include "tasks.h"
#include <pmm.h>

void init(struct mb_info* mbInfo)
{
    init_pmm(mbInfo);

    clearScreen();

    init_gdt();
    init_idt();

    init_multitasking();

    while (1);
}
