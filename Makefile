# _____     ___ ____     ___ ____
#  ____|   |    ____|   |        | |____|
# |     ___|   |____ ___|    ____| |    \    PS2DEV Open Source Project.
#-----------------------------------------------------------------------
# Copyright 2005, ps2dev - http://www.ps2dev.org
# Licenced under Academic Free License version 2.0
# Review ps2sdk README & LICENSE files for further details.

IOP_BIN = 989snd.irx
IOP_OBJS = src/989snd.o src/ame.o src/autopan.o src/autopb.o src/autoptch.o \
			src/autoverb.o src/autovol.o src/blocksnd.o src/extern.o \
			src/init.o src/intr_code.o src/LFO.o src/loader.o src/midi.o \
			src/moviesnd.o src/my_rand.o src/pantable.o src/playsnd.o \
			src/reverb.o src/sndhand.o src/sram.o src/stick.o src/stream.o \
			src/valloc.o src/vol.o

IOP_LIBS =
IOP_TABS = stdio.tab libsd.tab ioman.tab thbase.tab sysmem.tab sysclib.tab \
			sifcmd.tab sifman.tab intrman.tab thsemap.tab cdvdman.tab timrman.tab

IOP_CFLAGS += -Wall -fno-common -Werror-implicit-function-declaration -std=c99 -O3

all: $(IOP_BIN)

clean:
	rm -f $(IOP_BIN) $(IOP_OBJS)

run: $(IOP_BIN)
	ps2client execiop host:$(IOP_BIN)

sim: $(IOP_BIN)
	PCSX2 --elf="hello.elf"

#PCSX2 --irx=$(IOP_BIN)

reset:
	ps2client reset

include $(PS2SDK)/Defs.make
include $(PS2SDK)/samples/Makefile.pref
include $(PS2SDK)/samples/Makefile.iopglobal
