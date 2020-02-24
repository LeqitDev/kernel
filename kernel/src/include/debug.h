//
// Created by marku on 21.01.2020.
//

#ifndef DEBUG_H
#define DEBUG_H

#include <stdint.h>

void panic_message(uint32_t ebp);
void init_seriell(void);
void print_seriell(const char * str);

#endif //DEBUG_H
