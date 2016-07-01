#!/bin/sh

export LD_LIBRARY_PATH=/home/staff/lucas/mc723/hw_tools/systemc-2.3.1/lib-linux64/:$LD_LIBRARY_PATH
export PATH=$PATH:/home/staff/lucas/mips-newlib-elf/bin/
# /home/staff/lucas/mc723/hw_tools/ArchC-2.2.0/installed/bin:/home/staff/lucas/mc723/hw_tools/mips-newlib-elf/bin/

Y4KPATH="/home/ec2014/ra033324/y4k/classes/mc723a/git_projeto3/mips_multi_lock_multiplyperipheral_example/"

echo
echo "mips-newlib-elf-gcc -specs=archc ${Y4KPATH}/projeto3.c -o ${Y4KPATH}/projeto3.mips"
mips-newlib-elf-gcc -specs=archc -O3 ${Y4KPATH}/projeto3.c -o ${Y4KPATH}/projeto3.mips -lm
