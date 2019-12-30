#include <stdint.h>
#include "include/InterruptDescriptorTable.h"
#include "include/console.h"

#define IDT_KERNEL_CODE_SEGMENT 0x80
#define IDT_RING_0 0x00
#define IDT_INTERRUPT_GATE 0x6

#define IDT_ENTRIES 256

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
} __attribute__((packed)) idt[IDT_ENTRIES];

struct {
    uint16_t limit;
    void* pointer;
} __attribute__((packed)) idtp = {
        .limit = IDT_ENTRIES * 8 - 1,
        .pointer = idt,
};

void set_idt_entry(int i, unsigned int offset, unsigned int selector, unsigned int type, unsigned int bit, unsigned int privilege, unsigned int enabled) {
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

void outb(uint16_t port, uint8_t data)
{
    asm volatile ("outb %0, %1" : : "a" (data), "Nd" (port));
}

unsigned char inb(uint16_t port) {
    unsigned char result;
    asm volatile("inb %1, %0" : "=a" (result) : "Nd" (port));
    return result;
}

void init_pic(void) {
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
    outb(0x21, 0x0);
    outb(0xa1, 0x0);
}

struct cpu_state {
    // Von Hand gesicherte Register
    uint32_t   eax;
    uint32_t   ebx;
    uint32_t   ecx;
    uint32_t   edx;
    uint32_t   esi;
    uint32_t   edi;
    uint32_t   ebp;

    uint32_t   intr;
    uint32_t   error;

    // Von der CPU gesichert
    uint32_t   eip;
    uint32_t   cs;
    uint32_t   eflags;
    uint32_t   esp;
    uint32_t   ss;
};

void handle_interrupt(struct cpu_state* cpu)
{
    if (cpu->intr <= 0x1f) {
        char buffer[64] = "XXX";
        printf(buffer, "Exception %i, Kernel angehalten!\n", cpu->intr);
        puts(buffer);

        // Hier den CPU-Zustand ausgeben

        while(1) {
            // Prozessor anhalten
            asm volatile("cli; hlt");
        }
    } else {
        if (cpu->intr >= 0x20 && cpu->intr <= 0x2f) {
            if (cpu->intr == 0x20) {
                //IRQ
                puts("\nKeyboardInterrupt");
            }
            if (cpu->intr >= 0x28) {
                outb(0xa0, 0x20);
            }
            outb(0x20, 0x20);
        }
    }
}

void load_idt(void) {

    asm volatile("lidt %0" : : "m" (idtp));
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

    load_idt();
}


