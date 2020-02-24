//
// Created by marku on 14.02.2020.
//

#ifndef CCOS_COM_H
#define CCOS_COM_H

#include "stdint.h"

void init_com(uint16_t base, uint32_t baud, uint8_t parity, uint8_t bits);
uint8_t is_transmit_empty(uint16_t base);
void write_com(uint16_t base, uint8_t chr);
uint8_t serial_received(uint16_t base);
uint8_t read_serial(uint16_t base);

#endif //CCOS_COM_H
