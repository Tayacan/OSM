# Add your _userland_ program sources to this variable:
SOURCES  := game.c

OBJECTS  := $(patsubst %.c, %.o, $(SOURCES))
TARGETS  := $(patsubst %.o, %, $(OBJECTS))

# crt.o must be the first one and the $(SYSLIBS) must come first in
# the pre-requisites list (or object files list).
SYSLIBS := ../tests/crt.o ../tests/_syscall.o ../tests/lib.o display.o cursor.o

# Compiler configuration
CC      := mips-elf-gcc
AS      := mips-elf-as
LD      := mips-elf-ld
CFLAGS  += -G0 -I.. -Wall -W -Werror
LDFLAGS := --script=ld.userland.script --cref -s -G0
ASFLAGS := -I.. -Wa,-mips32

# -G0 is needed to avoid GP optimization (LD won't link if it is used)

# Compiling targets (Add explicit targets here if you need to do more
# complex linking (e.g., your source code is divided to multiple
# files))
all: $(TARGETS)

$(TARGETS): %: $(SYSLIBS) %.o
	$(LD) -o $@ $(LDFLAGS) -Map $@.map $^

%.o: %.S
	$(CC) -o $@ $(ASFLAGS) -c $<


%.o: %.c
	$(CC) -o $@  $(CFLAGS) -c $<

clean:
	rm -f $(SYSLIBS) $(OBJECTS) *.map $(TARGETS) *~

.PHONY: clean
