//
// Created by marku on 02.02.2020.
//

#ifndef CCOS_MULTIBOOT_H
#define CCOS_MULTIBOOT_H

struct mb_mem_map {
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

struct mb_modules {
    int mod_start;
    int mod_end;
    char* string;
    int reserved;
};

#endif //CCOS_MULTIBOOT_H
