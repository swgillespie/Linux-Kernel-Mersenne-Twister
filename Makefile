obj-m += linux_twister.o

all:
	gcc -o mersenne_twister_test mersenne_twister_test.c 
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules

clean:
	rm mersenne_twister_test
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean