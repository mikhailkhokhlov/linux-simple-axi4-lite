#KERNEL_SRC_DIR=../../linux-xlnx

obj-m := simple_axi_lite.o

all:
	make -C $(KERNEL_SRC_DIR) M=$(PWD) modules

clean:
	make -C $(KERNEL_SRC_DIR) M=$(PWD) clean

