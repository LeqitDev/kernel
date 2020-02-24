#include "console.h"
#include "debug.h"
#include <stdint.h>
#include "com.h"


struct StackFrame {
    struct StackFrame* next_ebp;
    uint32_t ebp;
};

void panic_message(uint32_t ebp) {
    println("An error occurred: ");

    struct StackFrame* frame = 0;
    int i = 0;

    if (!ebp) asm("mov %%ebp, %0" : "=r"(frame));
    else frame = (struct StackFrame*) ebp;

    while (frame) {
        println("%i: %h", i, frame->ebp);
        if (frame->next_ebp != frame) frame = frame->next_ebp;
        else frame = 0;
        i++;
    }
}

void init_seriell(void) {
    init_com(0x3F8, 9600, 0x3, 0x8);
}

void print_seriell(const char * str) {
    for (int i = 0; i < strlen(str); i++) {
        write_com(0x3F8, str[i]);
    }
    write_com(0x3F8, '\n');
}