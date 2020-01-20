//
// Created by marku on 17.01.2020.
//

#ifndef TASKS_H
#define TASKS_H

#include "InterruptDescriptorTable.h"

struct cpu_state* schedule(struct cpu_state* cpu);

void init_multitasking(void);

#endif //TASKS_H
