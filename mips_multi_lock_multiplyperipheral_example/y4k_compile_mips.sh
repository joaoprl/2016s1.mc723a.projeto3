#!/bin/sh

export LD_LIBRARY_PATH=/home/staff/lucas/mc723/hw_tools/systemc-2.3.1/lib-linux64/:$LD_LIBRARY_PATH
export PATH=$PATH:/home/staff/lucas/mips-newlib-elf/bin/
# /home/staff/lucas/mc723/hw_tools/ArchC-2.2.0/installed/bin:/home/staff/lucas/mc723/hw_tools/mips-newlib-elf/bin/
# export PATH=$PATH:/home/staff/lucas/mc723/hw_tools/ArchC-2.2.0/installed/bin:/home/staff/lucas/mc723/hw_tools/mips-newlib-elf/bin/

MIPSTLMPATH='/home/ec2014/ra033324/y4k/classes/mc723a/mips_multi_lock_multiplyperipheral_example/'

echo
echo "cd ${MIPSTLMPATH}/"
cd ${MIPSTLMPATH}/

echo
echo "acsim ./mips.ac -abi"
acsim ./mips.ac -abi

echo
orig='SRCS := main.cpp $(ACSRCS) $(ACFILES)  $(MODULE)_syscall.cpp'
repl='SRCS := main.cpp $(ACSRCS) $(ACFILES) $(MODULE)_syscall.cpp ac_tlm_peripheral.cpp ac_tlm_mem.cpp ac_tlm_router.cpp'
echo "sed \"s|${orig}|${repl}|\" Makefile.archc > y4k_Makefile.archc"
sed "s|${orig}|${repl}|" Makefile.archc > y4k_Makefile.archc

echo
echo "make -C ${MIPSTLMPATH}/ -f ${MIPSTLMPATH}/y4k_Makefile.archc clean"
make -C ${MIPSTLMPATH}/ -f ${MIPSTLMPATH}/y4k_Makefile.archc clean

echo
echo "make -C ${MIPSTLMPATH}/ -f ${MIPSTLMPATH}/y4k_Makefile.archc"
make -C ${MIPSTLMPATH}/ -f ${MIPSTLMPATH}/y4k_Makefile.archc
