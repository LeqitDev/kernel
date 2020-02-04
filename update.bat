@echo off

bash -c "make"

qemu-system-x86_64.exe -d int -kernel kernel/kernel -initrd user/test.bin

bash -c "make clean-all"
