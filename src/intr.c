//TODO: Kommentare vervollständigen

//Imports
#include "console.h"
#include "kbc.h"
#include "intr.h"
#include "debug.h"
#include "tasks.h"

//Class variables

static uint32_t tss[32] = {0, 0, 0x10};


/*
 * GDT
 * */


//Variables

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

#define GDT_ENTRIES 6 //anzahl der einträge die in der gdt sind

static uint64_t gdt[GDT_ENTRIES]; //gdt[-table] erstellen


//Functions

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

void init_gdt(void) {
    set_gdt_entry(0, 0, 0, 0); //erster eintrag immer 0
    set_gdt_entry(1, 0, 0xfffff,
            GDT_FLAG_SEGMENT | GDT_FLAG_32_BIT |
            GDT_FLAG_CODESEG | GDT_FLAG_4K_GRAN | GDT_FLAG_PRESENT); //zweiter eintrag, codesegment im kernel (ring 0)
    set_gdt_entry(2, 0, 0xfffff,
            GDT_FLAG_SEGMENT | GDT_FLAG_32_BIT |
            GDT_FLAG_DATASEG | GDT_FLAG_4K_GRAN | GDT_FLAG_PRESENT); //dritter eintrag, datasegment im kernel (ring 0)
    set_gdt_entry(3, 0, 0xfffff,
            GDT_FLAG_SEGMENT | GDT_FLAG_32_BIT |
            GDT_FLAG_CODESEG | GDT_FLAG_4K_GRAN | GDT_FLAG_PRESENT | GDT_FLAG_RING3); //vierter eintrag, codesegment im user-space (ring 3)
    set_gdt_entry(4, 0, 0xfffff,
            GDT_FLAG_SEGMENT | GDT_FLAG_32_BIT |
            GDT_FLAG_DATASEG | GDT_FLAG_4K_GRAN | GDT_FLAG_PRESENT | GDT_FLAG_RING3); //fünfter eintrag, datasegment im user-space (ring 3)
    set_gdt_entry(5, (uint32_t) tss, sizeof(tss),
            GDT_FLAG_TSS | GDT_FLAG_PRESENT | GDT_FLAG_RING3);

    asm volatile("lgdt %0" : : "m" (gdtp)); //gdt setzten bzw. zu aktivieren

    //segmente neu laden um die neue gdt zu beachten
    asm volatile(
    "mov $0x10, %ax\n"
    "mov %ax, %ds\n"
    "mov %ax, %es\n"
    "mov %ax, %fs\n"
    "mov %ax, %gs\n"
    "mov %ax, %ss"
    );

    asm volatile("ltr %%ax" : : "a" (5 << 3));
}


/*
 * IDT
 * */


//Variables

//verbindung der handler zu assembler file
extern void intr_stub_0(void);
extern void intr_stub_1(void);
extern void intr_stub_2(void);
extern void intr_stub_3(void);
extern void intr_stub_4(void);
extern void intr_stub_5(void);
extern void intr_stub_6(void);
extern void intr_stub_7(void);
extern void intr_stub_8(void);
extern void intr_stub_9(void);
extern void intr_stub_10(void);
extern void intr_stub_11(void);
extern void intr_stub_12(void);
extern void intr_stub_13(void);
extern void intr_stub_14(void);
extern void intr_stub_15(void);
extern void intr_stub_16(void);
extern void intr_stub_17(void);
extern void intr_stub_18(void);

extern void intr_stub_32(void);
extern void intr_stub_33(void);

extern void intr_stub_48(void);


//vereinfachung von flags
#define IDT_KERNEL_CODE_SEGMENT 0x80
#define IDT_RING_0 0x00
#define IDT_INTERRUPT_GATE 0x6

#define IDT_ENTRIES 256 //größe der idt tabelle (zahl der einträge)

struct {
    unsigned int offset0 : 16;
    unsigned int selector : 16;
    unsigned int ist : 3;
    unsigned int _0 : 5;
    unsigned int type : 3;
    unsigned int bit : 1;
    unsigned int _1 : 1;
    unsigned int privilege : 2;
    unsigned int enabled : 1;
    unsigned int offset1 : 16;
} __attribute__((packed)) idt[IDT_ENTRIES]; //idt tabelle

struct {
    uint16_t limit;
    void* pointer;
} __attribute__((packed)) idtp = {
        .limit = IDT_ENTRIES * 8 - 1,
        .pointer = idt,
};


//Functions

void set_idt_entry(int i, unsigned int offset, unsigned int selector, unsigned int type, unsigned int bit, unsigned int privilege, unsigned int enabled) {
    //eintrag zum idt hinzufügen
    idt[i].ist = 0;
    idt[i]._0 = 0;
    idt[i]._1 = 0;
    idt[i].offset0 = offset & 0xFFFF;
    idt[i].offset1 = (offset >> 16) & 0xFFFF;
    idt[i].selector = selector;
    idt[i].type = type;
    idt[i].bit = bit;
    idt[i].privilege = privilege;
    idt[i].enabled = enabled;
}

static void init_pic(void)
{
    // Master-PIC initialisieren
    outb(0x20, 0x11); // Initialisierungsbefehl fuer den PIC
    outb(0x21, 0x20); // Interruptnummer fuer IRQ 0
    outb(0x21, 0x04); // An IRQ 2 haengt der Slave
    outb(0x21, 0x01); // ICW 4

    // Slave-PIC initialisieren
    outb(0xa0, 0x11); // Initialisierungsbefehl fuer den PIC
    outb(0xa1, 0x28); // Interruptnummer fuer IRQ 8
    outb(0xa1, 0x02); // An IRQ 2 haengt der Slave
    outb(0xa1, 0x01); // ICW 4

    // Alle IRQs aktivieren (demaskieren)
    outb(0x20, 0x0);
    outb(0xa0, 0x0);
}

void init_idt(void) {
    init_pic();

    set_idt_entry(0, (unsigned int) intr_stub_0, 0x8, 0x6, 1, 0x00, 1);
    set_idt_entry(1, (unsigned int) intr_stub_1, 0x8, 0x6, 1, 0x00, 1);
    set_idt_entry(2, (unsigned int) intr_stub_2, 0x8, 0x6, 1, 0x00, 1);
    set_idt_entry(3, (unsigned int) intr_stub_3, 0x8, 0x6, 1, 0x00, 1);
    set_idt_entry(4, (unsigned int) intr_stub_4, 0x8, 0x6, 1, 0x00, 1);
    set_idt_entry(5, (unsigned int) intr_stub_5, 0x8, 0x6, 1, 0x00, 1);
    set_idt_entry(6, (unsigned int) intr_stub_6, 0x8, 0x6, 1, 0x00, 1);
    set_idt_entry(7, (unsigned int) intr_stub_7, 0x8, 0x6, 1, 0x00, 1);
    set_idt_entry(8, (unsigned int) intr_stub_8, 0x8, 0x6, 1, 0x00, 1);
    set_idt_entry(9, (unsigned int) intr_stub_9, 0x8, 0x6, 1, 0x00, 1);
    set_idt_entry(10, (unsigned int) intr_stub_10, 0x8, 0x6, 1, 0x00, 1);
    set_idt_entry(11, (unsigned int) intr_stub_11, 0x8, 0x6, 1, 0x00, 1);
    set_idt_entry(12, (unsigned int) intr_stub_12, 0x8, 0x6, 1, 0x00, 1);
    set_idt_entry(13, (unsigned int) intr_stub_13, 0x8, 0x6, 1, 0x00, 1);
    set_idt_entry(14, (unsigned int) intr_stub_14, 0x8, 0x6, 1, 0x00, 1);
    set_idt_entry(15, (unsigned int) intr_stub_15, 0x8, 0x6, 1, 0x00, 1);
    set_idt_entry(16, (unsigned int) intr_stub_16, 0x8, 0x6, 1, 0x00, 1);
    set_idt_entry(17, (unsigned int) intr_stub_17, 0x8, 0x6, 1, 0x00, 1);
    set_idt_entry(18, (unsigned int) intr_stub_18, 0x8, 0x6, 1, 0x00, 1);

    set_idt_entry(32, (unsigned int) intr_stub_32, 0x8, 0x6, 1, 0x00, 1);
    set_idt_entry(33, (unsigned int) intr_stub_33, 0x8, 0x6, 1, 0x00, 1);

    set_idt_entry(48, (unsigned int) intr_stub_48, 0x8, 0x6, 1, 0x00, 1);

    asm volatile("lidt %0" : : "m" (idtp));
    asm volatile("sti");
}


/*
 * Interrupt Handler
 * */

struct cpu_state* handle_interrupt(struct cpu_state* cpu)
{
    struct cpu_state* new_cpu = cpu;
    if (cpu->intr <= 0x1f) {
        panic_message(cpu->ebp);
        char buffer[64] = "X";
        printf(buffer, "Exception %i, Kernel angehalten! [%i]\n", cpu->intr, cpu->error);
        puts(buffer);

        println("cs: %i,\n eax: %i,\n ebp: %i,\n ebx: %i,\n ecx: %i,\n edi: %i,\n edx: %i,\n eip: %i,\n esi: %i,\n esp: %i,\n ss: %i,\n eflags: %i", cpu->cs, cpu->eax, cpu->ebp, cpu->ebx, cpu->ecx, cpu->edi, cpu->edx, cpu->eip, cpu->esi, cpu->esp, cpu->ss, cpu->eflags);

        // Hier den CPU-Zustand ausgeben

        println("End of Exception");

        while(1) {
            // Prozessor anhalten
            asm volatile("cli; hlt");
        }
    } else {
        if (cpu->intr >= 0x20 && cpu->intr <= 0x2f) {
            if (cpu->intr == 0x20) {
                print("schedule");
                new_cpu = schedule(cpu);
                tss[1] = (uint32_t) (new_cpu + 1);
            }
            if (cpu->intr == 0x21) {
                //IRQ
                irq_handler();
            }
            if (cpu->intr >= 0x28) {
                outb(0xa0, 0x20);
            }
            outb(0x20, 0x20);
        }
    }

    return new_cpu;
}


/*
 * Other
 * */

void outb(uint16_t port, uint8_t data)
{
    //PCI ansteuern
    asm volatile ("outb %0, %1" : : "a" (data), "Nd" (port));
}

unsigned char inb(uint16_t port) {
    //PCI input bekommen
    unsigned char result;
    asm volatile("inb %1, %0" : "=a" (result) : "Nd" (port));
    return result;
}