obj-m += kernel/linux_twister.o

all: kernel-mod tests

kernel-mod:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules

clean-kernel:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean

tests:
	gcc -o mersenne_twister_test tests/mersenne_twister_test.c 

clean-tests:
	rm mersenne_twister_test