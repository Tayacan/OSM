# Makefile for the kernel module

# Set the module name
MODULE := proc


FILES := exception.c elf.c process.c syscall.c semaphores.c

SRC += $(patsubst %, $(MODULE)/%, $(FILES))

