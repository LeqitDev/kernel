//
// Created by marku on 21.12.2019.
//

#ifndef INTERRUPTDESCRIPTORTABLE_H
#define INTERRUPTDESCRIPTORTABLE_H

#include <stdint.h>

extern void outb(uint16_t port, uint8_t data);

extern unsigned char inb(uint16_t port);

extern void init_idt(void);

#endif //INTERRUPTDESCRIPTORTABLE_H
