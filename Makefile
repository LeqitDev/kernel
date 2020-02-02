all: kernel user

kernel:
	$(MAKE) -C kernel

user:
	$(MAKE) -C user

clean-kernel:
	$(MAKE) -C kernel clean

clean-user:
	$(MAKE) -C user clean

clean-all:
	$(MAKE) -C kernel clean
	$(MAKE) -C user clean

.PHONY: all kernel user