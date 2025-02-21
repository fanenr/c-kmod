obj-m += hello.o hello-seq.o
hello-objs := main.o
hello-seq-objs := seq.o

KDIR := /lib/modules/$(shell uname -r)/build

all:
	make -C $(KDIR) M=$(PWD) modules

clean:
	make -C $(KDIR) M=$(PWD) clean
