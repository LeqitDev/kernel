//
// Created by marku on 24.01.2020.
//

#ifndef PMM_H
#define PMM_H

#include "stdint.h"

#define null ((void*) 0)

struct mem_map {
    int size;
    long long baseaddr;
    long long length;
    int type;
};

struct mb_info {
    int mbs_flags;
    int mbs_mem_lower;
    int mbs_mem_upper;
    int mbs_bootdevice;
    int mbs_cmdline;
    int mbs_mods_count ;
    void* mbs_mods_addr;
    int mbs_syms[4];
    int mbs_mmap_length;
    void* mbs_mmap_addr;
};

void init_pmm(struct mb_info* mb_info);

void pmm_free(void* page);

void* pmm_alloc(void);

#endif //CCOS_PMM_H
