cmake_minimum_required(VERSION 3.15)
project(CCOS C)

set(CMAKE_C_STANDARD 11)

include_directories(kernel/src/include)
include_directories(kernel/src)

include_directories(user/src/include)
include_directories(user/src)

add_executable(
        CCOS
        kernel/src/include/mm.h kernel/src/include/intr.h kernel/src/include/commands.h kernel/src/include/console.h kernel/src/include/debug.h kernel/src/include/kbc.h kernel/src/include/stdint.h kernel/src/include/tasks.h kernel/src/include/multiboot.h kernel/src/include/string.h kernel/src/include/stddef.h
        kernel/src/pmm.c kernel/src/intr.c kernel/src/commands.c kernel/src/console.c kernel/src/debug.c kernel/src/init.c kernel/src/kbc.c kernel/src/tasks.c
        user/src/init.c
        user/src/include/stdint.h kernel/src/include/elf.h kernel/src/vmm.c kernel/src/com.c kernel/src/include/com.h)