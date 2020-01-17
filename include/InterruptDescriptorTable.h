//
// Created by marku on 21.12.2019.
//

#ifndef INTERRUPTDESCRIPTORTABLE_H
#define INTERRUPTDESCRIPTORTABLE_H

#include <stdint.h>

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

extern void outb(uint16_t port, uint8_t data);

extern unsigned char inb(uint16_t port);

extern void init_idt(void);

#endif //INTERRUPTDESCRIPTORTABLE_H
