#include "include/console.h"
#include "include/debug.h"


struct StackFrame {
    struct StackFrame* prev;
    unsigned int eip;
};

void panic_message() {
    println("An error occurred: ");

    struct StackFrame* frame = 0;
    int i = 0;

    asm("mov %%ebp, %0" : "=r"(frame));

    while (frame->eip != 0) {
        println("%i: %hu", i, frame->eip);
        frame = frame->prev;
        i++;
    }
}