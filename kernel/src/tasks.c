#include "console.h"
#include <stdint.h>
#include "intr.h"
#include "tasks.h"
#include "pmm.h"
#include "multiboot.h"
#include "string.h"

struct task {
    struct cpu_state* cpu_state;
    struct task* next_task;
};

static struct task* first_task = null;
static struct task* current_task = null;

static void task_a(void) {
    while (1) {
        println("a");
    }
}

static void task_b(void) {
    while (1) {
        println("b");
    }
}

struct task* init_task(void* entry) {
    uint8_t * stack = pmm_alloc();
    uint8_t * user_stack = pmm_alloc();

    struct cpu_state new_state = {
            .eax = 0,
            .ebx = 0,
            .ecx = 0,
            .edx = 0,
            .esi = 0,
            .edi = 0,
            .ebp = 0,
            .esp = (uint32_t) user_stack + 4096,
            .eip = (uint32_t) entry,

            .cs = 0x18 | 0x03,
            .ss = 0x20 | 0x03,

            .eflags = 0x202,
    };

    struct cpu_state* state = (void*) (stack + 4096 - sizeof(new_state));
    *state = new_state;

    struct task* task = pmm_alloc();
    task->cpu_state = state;
    task->next_task = first_task;
    first_task = task;

    return task;
}

void init_multitasking(struct mb_info* mb_info) {
    if (mb_info->mbs_mods_count == 0) {
        init_task(task_a);
        init_task(task_b);
    } else {
        struct mb_modules* module = mb_info->mbs_mods_addr;
        size_t length = module[0].mod_end - module[0].mod_start;
        void* load_addr = (void*) 0x200000;

        memcpy(load_addr, (void*) module[0].mod_end, length);
        println("Task at %iu", load_addr);
        init_task(load_addr);
    }
}

struct cpu_state* schedule(struct cpu_state* cpu) {
    if (current_task != null) {
        current_task->cpu_state = cpu;
    }
    if (current_task == null) {
        current_task = first_task;
    } else {
        current_task = current_task->next_task;
        if (current_task == null) {
            current_task = first_task;
        }
    }

    cpu = current_task->cpu_state;

    return cpu;
}