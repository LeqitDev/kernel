//
// Created by marku on 24.01.2020.
//

#ifndef PMM_H
#define PMM_H

#define null ((void*) 0)

struct mem_map {
    int size : 4;
    int baseaddr : 8;
    int length : 8;
    int type : 4;
};

struct mb_info {
    int mbs_flags : 4;
    int mbs_mem_lower : 4;
    int mbs_mem_upper : 4;
    int mbs_bootdevice : 4;
    int mbs_cmdline : 4;
    int mbs_mods_count : 4;
    void* mbs_mods_addr;
    int mbs_syms : 16;
    int mbs_mmap_length : 4;
    void* mbs_mmap_addr;
};

void init_pmm(struct mb_info* mb_info);

void pmm_free(void* page);

void* pmm_alloc(void);

#endif //CCOS_PMM_H
