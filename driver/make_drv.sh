#!/bin/sh

if [ ! -d "${1}" ]; then
  echo "Need to specify path to Linux kernel tree";
  exit
fi

echo "==> Kernel directory: "${1}
echo ""

if [ $# -eq 2 -a "${2}" == "clean" ]; then
  make KERNEL_SRC_DIR=${1} clean
else
  make KERNEL_SRC_DIR=${1} ARCH=arm CROSS_COMPILE=arm-linux-gnueabihf-
fi
