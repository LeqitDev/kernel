#include "console.h"
#include <stdint.h>
#include "intr.h"
#include "tasks.h"
#include "mm.h"
#include "multiboot.h"
#include "string.h"
#include "elf.h"

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

void init_elf(void* image) {
    struct elf_header* header = image;
    struct elf_program_header* ph;
    int i;

    if (header->magic != ELF_MAGIC) {
        println("ERR(ELF loading): Keine gültige ELF-MAGIC!");
        return;
    }

    ph = (struct elf_program_header*) (((char*) image) + header->ph_offset);
    for (i = 0; i < header->ph_entry_count; i++, ph++) {
        void* dest = (void*) ph->virt_addr;
        void* src = ((char*) image) + ph->offset;

        if (ph->type != 1) {
            continue;
        }

        memset(dest, 0, ph->mem_size);
        memcpy(dest, src, ph->file_size);
    }
    init_task((void*) header->entry);
}

void init_multitasking(struct mb_info* mb_info) {
    if (mb_info->mbs_mods_count == 0) {
        init_task(task_a);
        init_task(task_b);
    } else {
        struct mb_modules* modules = mb_info->mbs_mods_addr;
        for (int i = 0; i < mb_info->mbs_mods_count; i++) {
            init_elf((void*) modules[i].mod_start);
        }
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