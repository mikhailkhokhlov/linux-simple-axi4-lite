Simple interface Linux driver to AXI4-Lite peripheral.

How to use:

	1. Patch device tree:

		cp zynq-zybo-z7-dts.patch <path to linux-xlnx>
		cd <path to linux-xlnx>
		patch -p1 < zynq-zybo-z7-dts.patch

	2. Build driver:

		cd driver
		./make_drv.sh <path to linux-xlnx>

	3. Build test application

		cd test
		make

	4. Copy simple_axi_lite_test to target device and run:
		./simple_axi_lite_test
