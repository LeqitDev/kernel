//
// Created by marku on 24.01.2020.
//

#ifndef PMM_H
#define PMM_H

#include "multiboot.h"

#define null ((void*) 0)

void init_pmm(struct mb_info* mb_info);
void pmm_free(void* page);
void* pmm_alloc(void);

#define PTE_PRESENT 0x1
#define PTE_WRITE   0x2
#define PTE_USER    0x4

struct vmm_context {
    int* pagedir;
};
void init_vmm(void);

#endif //CCOS_PMM_H
