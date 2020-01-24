@echo off

bash -c "/mnt/e/Downloads/zig-linux/zig-linux-x86_64-0.5.0+2c7a2aefb/zig build"

cd zig-cache/bin/
qemu-system-x86_64.exe -d in_asm -kernel kernel
cd ../..
