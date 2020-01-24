#include "pmm.h"
#include "stdint.h"

#define BITMAP_SIZE 32768
static uint32_t bitmap[BITMAP_SIZE];

void pmm_mark_used(void* page) {
    uintptr_t index = (uintptr_t) page / 4096;
    bitmap[index / 32] &= ~(1 << (index % 32));
}

void* pmm_alloc(void) {
    for (int i = 0; i < BITMAP_SIZE; i++) {
        if (bitmap[i] != 0) {
            for (int j = 0; j < 32; j++) {
                if (bitmap[i] & (1 << j)) {
                    bitmap[i] &= (1 << j);
                    return (void*) ((i * 32 + j) * 4096);
                }
            }
        }
    }
    return 0;
}

void pmm_free(void* page) {
    uintptr_t index = (uintptr_t) page / 4096;
    bitmap[index / 32] |= (1 << (index % 32));
}

void init_pmm(struct mb_info* mb_info) {
    struct mem_map* mmap = mb_info->mbs_mmap_addr;
    struct mem_map* mmap_end = (void*) ((uintptr_t) mb_info->mbs_mmap_addr + mb_info->mbs_mmap_length);

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

    extern const void kernel_start;
    extern const void kernel_end;

    uintptr_t addr = (uintptr_t) &kernel_start;
    while (addr < (uintptr_t) &kernel_end) {
        pmm_mark_used((void*) addr);
        addr += 0x1000;
    }
}