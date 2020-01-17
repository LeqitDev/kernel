#include "include/console.h"
#include <stdint.h>
#include "include/InterruptDescriptorTable.h"
#include "include/Tasks.h"


void task_a(void) {
    println("A");
}

void task_b(void) {
    println("B");
}

static uint8_t stack_a[4096];
static uint8_t stack_b[4096];

struct cpu_state* init_task(uint8_t* stack, void* entry) {
    struct cpu_state new_state = {
            .eax = 0,
            .ebx = 0,
            .ecx = 0,
            .edx = 0,
            .esi = 0,
            .edi = 0,
            .ebp = 0,
            .eip = (uint32_t) entry,

            .cs  = 0x08,

            .eflags = 0x202,
    };

    struct cpu_state* state = (void*) (state + 4096 - sizeof(new_state));
    *state = new_state;

    return state;
}

static int current_task = -1;
static int num_task = 2;
static struct cpu_state* task_states[2];

void init_multitasking(void) {
    task_states[0] = init_task(stack_a, task_a);
    task_states[1] = init_task(stack_b, task_b);
}

struct cpu_state* schedule(struct cpu_state* cpu) {
    if (current_task >= 0) {
        task_states[current_task] = cpu;
    }

    current_task++;
    current_task %= num_task;

    cpu = task_states[current_task];

    return cpu;
}