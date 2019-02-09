obj-m = hrtimer_api.o
KERN_SRC = /home/msimonov/kernel_procamp/repos/busybox/_install/lib/modules/4.19.18/build

all:
	make -C $(KERN_SRC) M=$(PWD) modules
clean:
	make -C $(KERN_SRC) M=$(PWD) clean
