#include "stdint.h"

struct __attribute__((packed)) VideoChar {char c; char color;};

struct VideoChar * video = (struct VideoChar*) 0xb8000;

void _start(void){
    int i;
    for (i = 0; i < 3; i++) {
        asm("int $0x30" : : "a" (0), "b" ('0' + i));
    }

    while(1);
}