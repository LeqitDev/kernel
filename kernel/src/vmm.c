#include "mm.h"
#include "stdint.h"

static struct vmm_context* kernelcontext;

int vmm_map_page(struct vmm_context* context, uintptr_t virt, uintptr_t phys) {
    uint32_t page_index = virt / 0x1000;
    uint32_t pd_index = page_index / 1024;
    uint32_t pt_index = page_index % 1024;

    uint32_t * page_table;
    int i;

    if ((virt & 0xFFF) || (phys & 0xFFF)) return -1;

    if (context->pagedir[pd_index] & PTE_PRESENT) {
        page_table = (uint32_t*) (context->pagedir[pd_index] & ~0xFFF);
    } else {
        page_table = pmm_alloc();
        for (i = 0; i < 1024; i++) {
            page_table[i] = 0;
        }
        context->pagedir[pd_index] = (uint32_t) page_table | PTE_PRESENT | PTE_WRITE | PTE_USER;
    }

    page_table[pt_index] = phys | PTE_PRESENT | PTE_WRITE | PTE_USER;
    asm volatile("invlpg %0" : : "m" (*(char*)virt));
}

void vmm_activate_context(struct vmm_context* context) {
    asm volatile("mov %0, %%cr3" : : "r" (context->pagedir));
}

struct vmm_context* vmm_create_context(void) {
    struct vmm_context* context = pmm_alloc();
    int i;

    context->pagedir = pmm_alloc();
    for (i = 0; i < 1024; i++) {
        context->pagedir[i] = 0;
    }

    return context;
}

void init_vmm(void) {
    uint32_t cr0;
    int i;

    kernelcontext = vmm_create_context();

    for (i = 0; i < 4096 * 1024; i += 0x1000) {
        vmm_map_page(kernelcontext, i, i);
    }

    vmm_activate_context(kernelcontext);

    asm("mov %%cr0, %0" : "=r" (cr0));
    cr0 |= (1 << 31);
    asm("mov %0, %%cr0" : : "r" (cr0));
}