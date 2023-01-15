#ifndef LOADER_H_
#define LOADER_H_

#include "types.h"

extern bool gLimit2Meg;

void snd_InitLoader();
void snd_ShutdownLoader();

/* 00015014 000152f8 */ void *snd_IOPMemAlloc(/* 0x0(sp) */ SInt32 size, /* 0x4(sp) */ SInt32 use, /* 0x8(sp) */ SInt32 *act_size);
/* 000152f8 00015360 */ void snd_IOPMemFree(/* 0x0(sp) */ void *mem);

SoundBankPtr snd_BankLoadEx(SInt8 *name, SInt32 offset, UInt32 spu_mem_loc, UInt32 spu_mem_size);
SoundBankPtr snd_BankLoadByLocEx(SInt32 sect_loc, SInt32 file_offset, UInt32 spu_mem_loc, UInt32 spu_mem_size);
SoundBankPtr snd_BankLoadFromEEEx(UInt32 ee_loc, UInt32 spu_mem_loc, UInt32 spu_mem_size);
SoundBankPtr snd_BankLoadFromIOPEx(void *iop_loc, UInt32 spu_mem_loc, UInt32 spu_mem_size);
MultiMIDIBlockHeaderPtr snd_MMDLoad(SInt8 *name, SInt32 offset);
MultiMIDIBlockHeaderPtr snd_MMDLoadByLoc(SInt32 sect_loc, SInt32 file_offset);

void snd_UnloadAllBanks();
void snd_UnloadAllMMD();

void snd_WaitDMADone();

#endif // LOADER_H_
