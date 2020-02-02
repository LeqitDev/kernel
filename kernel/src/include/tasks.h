//
// Created by marku on 17.01.2020.
//

#ifndef TASKS_H
#define TASKS_H

#include "intr.h"
#include "multiboot.h"

struct cpu_state* schedule(struct cpu_state* cpu);

void init_multitasking(struct mb_info* mb_info);

#endif //TASKS_H
