#include <stdint.h>
#include "console.h"
#include "DescriptorTable.h"

#define GDT_ENTRIES 4

static uint64_t gdt[GDT_ENTRIES];

struct {
    uint16_t limit;
    void* pointer;
} __attribute__((packed)) gdtp = {
        .limit = GDT_ENTRIES * 8 - 1,
        .pointer = gdt,
};

void set_entry(int i, unsigned int base, unsigned int limit, int flags) {
    gdt[i] = limit & 0xffffLL;
    gdt[i] |= (base & 0xffffffLL) << 16;
    gdt[i] |= (flags & 0xffLL) << 40;
    gdt[i] |= ((limit >> 16) & 0xfLL) << 48;
    gdt[i] |= ((flags >> 8 )& 0xffLL) << 52;
    gdt[i] |= ((base >> 24) & 0xffLL) << 56;
}

void set_gdt(void) {
    asm volatile("lgdt %0" : : "m" (gdtp));
}

void reload_segments(void) {
    asm("mov $0x10, %ax\n"
        "mov %ax, %ds\n"
        "mov %ax, %es\n"
        "mov %ax, %fs\n"
        "mov %ax, %gs\n"
        "mov %ax, %ss\n"
        "ljmp $0x8, _wait\n"
        "_wait:");
}