SRCS = $(shell find -name '*.[cS]')
OBJS = $(addsuffix .o,$(basename $(SRCS)))

CC = gcc
LD = ld

ASFLAGS = -m32
CFLAGS = -m32 -Wall -g -fno-stack-protector -I src/include
LDFLAGS = -melf_i386 -Tsrc/kernel.ld
LGCCFIX = /usr/lib/gcc/x86_64-linux-gnu/9/32/libgcc.a

kernel: $(OBJS)
	$(LD) $(LDFLAGS) -o $@ $^ $(LGCCFIX)

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $^

%.o: %.S
	$(CC) $(ASFLAGS) -c -o $@ $^

clean:
	rm $(OBJS)

.PHONY: clean