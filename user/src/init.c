#include "stdint.h"

struct __attribute__((packed)) VideoChar {char c; char color;};

struct VideoChar * video = (struct VideoChar*) 0xb8000;

void _start(void){
    int i;
    for (i = 0; i < 90; i++) {
        video[1 + 1 * 80].c = '0' + i;
        video[1 + 1 * 80].color = 0x0f;
    }

    while(1);
}