SRCS = $(shell find -name '*.[cS]')
OBJS = $(addsuffix .o,$(basename $(SRCS)))

ZIGROOT?=/home/felix/software/zig-linux-x86_64-0.5.0
ZIG=$(ZIGROOT)/zig 

ZFLAGS=-target i386-freestanding-eabi

kernel: $(OBJS)
	$(ZIG) build-exe $(ZFLAGS) --name $@ --linker-script kernel.ld $(addprefix --object ,$^)

%.o: %.c
	$(ZIG) cc $(ZFLAGS) -I$(ZIGROOT)/lib/zig/include -Wall -Wextra -g -fno-stack-protector -ffreestanding -c -o $@ $^

%.o: %.S
	$(ZIG) cc $(ZFLAGS) -c -o $@ $^

clean:
	rm $(OBJS)

.PHONY: clean
