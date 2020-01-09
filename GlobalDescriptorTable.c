#include <stdint.h>
#include "include/console.h"
#include "include/GlobalDescriptorTable.h"

#define GDT_ENTRIES 5 //anzahl der einträge die in der gdt sind


//vereinfachung der einzelnen flags
#define GDT_FLAG_DATASEG 0x02
#define GDT_FLAG_CODESEG 0x0a
#define GDT_FLAG_TSS     0x09

#define GDT_FLAG_SEGMENT 0x10
#define GDT_FLAG_RING0   0x00
#define GDT_FLAG_RING3   0x60
#define GDT_FLAG_PRESENT 0x80

#define GDT_FLAG_4K_GRAN 0x800
#define GDT_FLAG_32_BIT  0x400

static uint64_t gdt[GDT_ENTRIES]; //gdt[-table] erstellen

struct {
    uint16_t limit;
    void* pointer;
} __attribute__((packed)) gdtp = {
        .limit = GDT_ENTRIES * 8 - 1,
        .pointer = gdt,
};

void set_gdt_entry(int i, unsigned int base, unsigned int limit, int flags) {
    //einen neuen gdt eintrag erstellen
    gdt[i] = limit & 0xffffLL; //limit1 hinzufügen
    gdt[i] |= (base & 0xffffffLL) << 16; //base1 hinzufügen
    gdt[i] |= (flags & 0xffLL) << 40; //flags1 hinzufügen
    gdt[i] |= ((limit >> 16) & 0xfLL) << 48; //limit2 hinzufügen
    gdt[i] |= ((flags >> 8 )& 0xffLL) << 52; //flags2 hinzufügen
    gdt[i] |= ((base >> 24) & 0xffLL) << 56; //base2 hinzufügen
}

void reload_segments(void) {
    //segmente neu laden um die neue gdt zu beachten
    asm volatile(
            "mov $0x10, %ax\n"
            "mov %ax, %ds\n"
            "mov %ax, %es\n"
            "mov %ax, %fs\n"
            "mov %ax, %gs\n"
            "mov %ax, %ss"
            );
}

void load_gdt(void) {
    asm volatile("lgdt %0" : : "m" (gdtp)); //gdt setzten bzw. zu aktivieren
    reload_segments(); //segmente neu laden um die neue gdt zu beachten
}

void init_gdt(void) {
    set_gdt_entry(0, 0, 0, 0); //erster eintrag immer 0
    set_gdt_entry(1, 0, 0xfffff, GDT_FLAG_SEGMENT | GDT_FLAG_32_BIT |
                             GDT_FLAG_CODESEG | GDT_FLAG_4K_GRAN | GDT_FLAG_PRESENT); //zweiter eintrag, codesegment im kernel (ring 0)
    set_gdt_entry(2, 0, 0xfffff, GDT_FLAG_SEGMENT | GDT_FLAG_32_BIT |
                             GDT_FLAG_DATASEG | GDT_FLAG_4K_GRAN | GDT_FLAG_PRESENT); //dritter eintrag, datasegment im kernel (ring 0)
    set_gdt_entry(3, 0, 0xfffff, GDT_FLAG_SEGMENT | GDT_FLAG_32_BIT |
                             GDT_FLAG_CODESEG | GDT_FLAG_4K_GRAN | GDT_FLAG_PRESENT | GDT_FLAG_RING3); //vierter eintrag, codesegment im user-space (ring 3)
    set_gdt_entry(4, 0, 0xfffff, GDT_FLAG_SEGMENT | GDT_FLAG_32_BIT |
                             GDT_FLAG_DATASEG | GDT_FLAG_4K_GRAN | GDT_FLAG_PRESENT | GDT_FLAG_RING3); //fünfter eintrag, datasegment im user-space (ring 3)

    load_gdt(); //gdt laden und aktivieren
}