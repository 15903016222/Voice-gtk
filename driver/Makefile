ifneq (${KERNELRELEASE},)	
	obj-m := DmaDoppler.o 
else
	KDIR?=/home/jake/workspace/Phascan-linux
	PWD:=$(shell pwd)
default:
	${MAKE} -C $(KDIR) M=${PWD} modules
endif

.PHONY: clean
clean:
	rm -rf *.o *.depend .*.cmd *.ko *.mod.c .tmp_versions *.order *.symvers *.ko.unsigned
