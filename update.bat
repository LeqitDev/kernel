@echo off

bash -c "make"

qemu-system-x86_64.exe -serial stdio -d int -no-reboot -no-shutdown -kernel kernel/kernel -initrd test.bin,test.bin

bash -c "make clean-all"
