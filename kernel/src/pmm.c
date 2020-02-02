#include "pmm.h"
#include "console.h"
#include "stdint.h"
#include "multiboot.h"

#define BITMAP_SIZE 32768
static uint32_t bitmap[BITMAP_SIZE];

extern const void kernel_start;
extern const void kernel_end;

static void pmm_mark_used(void* page) {
    uintptr_t index = (uintptr_t) page / 4096;
    bitmap[index / 32] &= ~(1 << (index % 32));
}

void* pmm_alloc(void) {
    for (int i = 0; i < BITMAP_SIZE; i++) {
        if (bitmap[i] != 0) {
            for (int j = 0; j < 32; j++) {
                if (bitmap[i] & (1 << j)) {
                    bitmap[i] &= ~(1 << j);
                    return (void*) ((i * 32 + j) * 4096);
                }
            }
        }
    }
    return null;
}

void pmm_free(void* page) {
    uintptr_t index = (uintptr_t) page / 4096;
    bitmap[index / 32] |= (1 << (index % 32));
//    println("bitmap[%i] = %b (bit changed: %b)", (index / 32), bitmap[index / 32], (1 << (index % 32)));
}

void init_pmm(struct mb_info* mb_info) {
    struct mb_mem_map* mmap = mb_info->mbs_mmap_addr;
    struct mb_mem_map* mmap_end = (void*)
            ((uintptr_t) mb_info->mbs_mmap_addr + mb_info->mbs_mmap_length);

    /*println("mmap:\n \t %hu\n \t %i\n \t %i\n \t %i\n \t %i \n", mmap, mmap->type, mmap->length, mmap->baseaddr, mmap->size);
    println("mmap_end:\n \t %hu\n \t %i\n \t %i\n \t %i\n \t %i", mmap_end, mmap_end->type, mmap_end->length, mmap_end->baseaddr, mmap_end->size);*/

    while (mmap < mmap_end) {
        if (mmap->type == 1) {
            uintptr_t addr = mmap->baseaddr;
            uintptr_t end_addr = addr + mmap->length;

            while (addr < end_addr) {
                pmm_free((void*) addr);
                addr += 0x1000;
            }
        }
        mmap++;
    }

    uintptr_t addr = (uintptr_t) &kernel_start;
    while (addr < (uintptr_t) &kernel_end) {
        pmm_mark_used((void*) addr);
        addr += 0x1000;
    }

    struct mb_modules* modules = mb_info->mbs_mods_addr;

    pmm_mark_used(mb_info);
    pmm_mark_used(modules);

    int i;
    for (i = 0; i < mb_info->mbs_mods_count; i++) {
        addr =  modules[i].mod_start;
        while (addr < modules[i].mod_end) {
            pmm_mark_used((void*) addr);
            addr += 0x1000;
        }
    }
}