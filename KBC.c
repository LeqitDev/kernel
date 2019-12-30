#include "include/InterruptDescriptorTable.h"
#include <stdint.h>

static void send_command(uint8_t command);
void init_keyboard(void) {
    while (inb(0x64) & 0x1) {
        inb(0x60);
    }

    send_command(0xF4);
}

static void send_command(uint8_t command) {
    while ((inb(0x64) & 0x2)) {}
    outb(0x60, command);
}