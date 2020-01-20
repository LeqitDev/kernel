#include <stdint.h>
#include "include/InterruptDescriptorTable.h"
#include "include/console.h"
#include "include/KBC.h"
#include "include/Tasks.h"

//TODO: Kommentare vervollständigen

//vereinfachung von flags
#define IDT_KERNEL_CODE_SEGMENT 0x80
#define IDT_RING_0 0x00
#define IDT_INTERRUPT_GATE 0x6

#define IDT_ENTRIES 256 //größe der idt tabelle (zahl der einträge)

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

void irq_handler() {
    uint8_t scancode;
    uint8_t keycode = 0;
    int break_code = 0;

    // Status-Variablen fuer das Behandeln von e0- und e1-Scancodes
    static int     e0_code = 0;
    // Wird auf 1 gesetzt, sobald e1 gelesen wurde, und auf 2, sobald das erste
    // Datenbyte gelesen wurde
    static int      e1_code = 0;
    static uint16_t  e1_prev = 0;

    scancode = inb(0x60);

    // Um einen Breakcode handelt es sich, wenn das oberste Bit gesetzt ist und
    // es kein e0 oder e1 fuer einen Extended-scancode ist
    if ((scancode & 0x80) &&
        (e1_code || (scancode != 0xE1)) &&
        (e0_code || (scancode != 0xE0)))
    {
        break_code = 1;
        scancode &= ~0x80;
    }

    if (e0_code) {
        // Fake shift abfangen und ignorieren
        if ((scancode == 0x2A) || (scancode == 0x36)) {
            e0_code = 0;
            return;
        }

        keycode = translate_scancode(1, scancode, break_code);
        e0_code = 0;
    } else if (e1_code == 2) {
        // Fertiger e1-Scancode
        // Zweiten Scancode in hoeherwertiges Byte packen
        e1_prev |= ((uint16_t) scancode << 8);
        keycode = translate_scancode(2, e1_prev, break_code);
        e1_code = 0;
    } else if (e1_code == 1) {
        // Erstes Byte fuer e1-Scancode
        e1_prev = scancode;
        e1_code++;
    } else if (scancode == 0xE0) {
        // Anfang eines e0-Codes
        e0_code = 1;
    } else if (scancode == 0xE1) {
        // Anfang eines e1-Codes
        e1_code = 1;
    } else {
        // Normaler Scancode
        keycode = translate_scancode(0, scancode, break_code);
    }
    // Zum Testen sollte folgendes verwendet werden:
    if (!break_code) {
        char buffer[64] = "XXX";
        char character = handle_keys(keycode);
        setBgColor(CONSOLE_COLOR_BLACK);
        setTextColor(CONSOLE_COLOR_LIGHT_CYAN);
        printf(buffer, "%c", character);
        puts(buffer);
    }
    //Nach erfolgreichen Tests, könnte eine send_key_event Funtkion wie bei Týndur verwendet werden
}

struct cpu_state* handle_interrupt(struct cpu_state* cpu)
{
    struct cpu_state* new_cpu = cpu;
    if (cpu->intr <= 0x1f) {
        char buffer[64] = "X";
        printf(buffer, "Exception %i, Kernel angehalten! [%i]\n", cpu->intr, cpu->error);
        puts(buffer);
        println("cs: %i,\n eax: %i,\n ebp: %i,\n ebx: %i,\n ecx: %i,\n edi: %i,\n edx: %i,\n eip: %i,\n esi: %i,\n esp: %i,\n ss: %i,\n eflags: %i", cpu->cs, cpu->eax, cpu->ebp, cpu->ebx, cpu->ecx, cpu->edi, cpu->edx, cpu->eip, cpu->esi, cpu->esp, cpu->ss, cpu->eflags);

        // Hier den CPU-Zustand ausgeben

        while(1) {
            // Prozessor anhalten
            asm volatile("cli; hlt");
        }
    } else {
        if (cpu->intr >= 0x20 && cpu->intr <= 0x2f) {
            if (cpu->intr == 0x20) {
                new_cpu = schedule(cpu);
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

void load_idt(void) {
    asm volatile("lidt %0" : : "m" (idtp));
    asm volatile("sti");
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


