//
// Created by marku on 02.01.2020.
//

#ifndef KBC_H
#define KBC_H

#include <stdint.h>

#define KEY_CODE_BACKSPACE 8
#define KEY_CODE_TABULATOR 9
#define KEY_CODE_ENTER     13

extern uint8_t translate_scancode(int set, uint16_t scancode, int breakcode);

extern char handle_keys(int keycode);

#endif //KBC_H
