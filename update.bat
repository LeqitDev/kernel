@echo off

bash -c "make"

qemu-system-x86_64.exe -kernel kernel/kernel

bash -c "make clean"
