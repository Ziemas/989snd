# _____     ___ ____     ___ ____
#  ____|   |    ____|   |        | |____|
# |     ___|   |____ ___|    ____| |    \    PS2DEV Open Source Project.
#-----------------------------------------------------------------------
# Copyright 2005, ps2dev - http://www.ps2dev.org
# Licenced under Academic Free License version 2.0
# Review ps2sdk README & LICENSE files for further details.

EE_LIB = 989snd.a
EE_OBJS = ee/989snd.o
EE_INCS = -I./common/ -I$(PS2DEV)/gsKit/include
EE_LIBS = -ldebug
EE_CFLAGS += -Wall -fno-strict-aliasing -fno-common -Werror-implicit-function-declaration -std=c99 -O3

TEST_OBJS = $(EE_OBJS) ee/main.o
TEST_EXE = test.elf
TEST_LIBS = $(EE_LIBS) -lgskit -ldmakit -lgskit_toolkit -lpng -lz
TEST_CFLAGS = $(EE_CFLAGS)
TEST_LDFLAGS = $(EE_LDFLAGS) -L$(PS2DEV)/gsKit/lib -L$(PS2SDK)/ports/lib

IOP_BIN = 989snd.irx
IOP_OBJS = iop/989snd.o iop/ame.o iop/autopan.o iop/autopb.o iop/autoptch.o \
			iop/autoverb.o iop/autovol.o iop/blocksnd.o iop/extern.o \
			iop/init.o iop/intr_code.o iop/LFO.o iop/loader.o iop/midi.o \
			iop/moviesnd.o iop/my_rand.o iop/pantable.o iop/playsnd.o \
			iop/reverb.o iop/sndhand.o iop/sram.o iop/stick.o iop/stream.o \
			iop/valloc.o iop/vol.o iop/error.o imports.o
IOP_LIBS =
#IOP_TABS = stdio.tab libsd.tab ioman.tab thbase.tab sysmem.tab sysclib.tab \
#			sifcmd.tab sifman.tab intrman.tab thsemap.tab cdvdman.tab timrman.tab
IOP_CFLAGS += -Wall -fno-strict-aliasing -fno-common -Werror-implicit-function-declaration -std=c99 -Os
IOP_CFLAGS += -DINTERNAL_SNDERR
IOP_INCS = -I./common/

all: $(IOP_BIN) $(EE_LIB) $(TEST_EXE)

$(TEST_EXE): $(EE_OBJS) $(TEST_OBJS)
	$(DIR_GUARD)
	$(EE_CC) -T$(EE_LINKFILE) $(EE_OPTFLAGS) -o $(TEST_EXE) $(TEST_OBJS) $(TEST_LDFLAGS) $(EXTRA_LDFLAGS) $(TEST_LIBS)

clean:
	rm -f $(IOP_BIN) $(IOP_OBJS) $(EE_LIB) $(EE_OBJS)

include $(PS2SDK)/Defs.make
include $(PS2SDK)/samples/Makefile.pref
include $(PS2SDK)/samples/Makefile.iopglobal
include $(PS2SDK)/samples/Makefile.eeglobal
