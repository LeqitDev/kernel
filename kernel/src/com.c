#include "stdint.h"
#include "intr.h"
#include "com.h"

#define IER 1
#define IIR 2
#define FCR 2
#define LCR 3
#define MCR 4
#define LSR 5
#define MSR 6

void init_com (uint16_t base, uint32_t baud, uint8_t parity, uint8_t bits) {
    union {
        uint8_t b[2];
        uint16_t w;
    } divisor;
    divisor.w = 115200/baud;

    // Interrupt ausschalten
    outb(base+IER,0x00);

    // DLAB-Bit setzen
    outb(base+LCR,0x80);

    // Teiler (low) setzen
    outb(base+0,divisor.b[0]);

    // Teiler (high) setzen
    outb(base+1,divisor.b[1]);

    // Anzahl Bits, Parität, usw setzen (DLAB zurücksetzen)
    outb(base+LCR,((parity&0x7)<<3)|((bits-5)&0x3));

    // Initialisierung abschließen
    outb(base+FCR,0xC7);
    outb(base+MCR,0x0B);
}

// Prüft, ob man bereits schreiben kann
uint8_t is_transmit_empty(uint16_t base) {
    return inb(base+LSR)&0x20;
}

// Byte senden
void write_com(uint16_t base, uint8_t chr) {
    while (is_transmit_empty(base)==0);
    outb(base,chr);
}

// Prüft, ob man bereits lesen kann
uint8_t serial_received(uint16_t base) {
    return inb(base+LSR)&1;
}

// Byte empfangen
uint8_t read_serial(uint16_t base) {
    while (!serial_received(base));
    return inb(base);
}