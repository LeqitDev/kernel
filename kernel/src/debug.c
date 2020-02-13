#include "console.h"
#include "debug.h"
#include <stdint.h>


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