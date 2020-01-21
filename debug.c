#include "include/console.h"
#include "include/debug.h"


struct StackFrame {
    struct StackFrame* prev;
    long eip;
};

void panic_message() {
    println("An error occurred: ");

    struct StackFrame* frame;
    int i = 1;

    asm("mov %%ebp, %0" : "=r"(frame));

    while (frame) {
        println("%i 0x{0:16} : %Lu", i, frame->eip);
        i++;
        if (frame->prev != frame) frame = 0;
        else frame = frame->prev;
    }
}