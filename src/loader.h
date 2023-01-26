#ifndef LOADER_H_
#define LOADER_H_

#include "types.h"

extern SFXBlock2Ptr gBlockListHead;
extern bool gLimit2Meg;

enum {
    SBLK_ID = 0x6B6C4253,
};

void snd_InitLoader();
void snd_ShutdownLoader();

void *snd_IOPMemAlloc(SInt32 size, SInt32 use, SInt32 *act_size);
void snd_IOPMemFree(void *mem);

SoundBankPtr snd_BankLoadEx(SInt8 *name, SInt32 offset, UInt32 spu_mem_loc, UInt32 spu_mem_size);
SoundBankPtr snd_BankLoadByLocEx(SInt32 sect_loc, SInt32 file_offset, UInt32 spu_mem_loc, UInt32 spu_mem_size);
SoundBankPtr snd_BankLoadFromEEEx(UInt32 ee_loc, UInt32 spu_mem_loc, UInt32 spu_mem_size);
SoundBankPtr snd_BankLoadFromIOPEx(void *iop_loc, UInt32 spu_mem_loc, UInt32 spu_mem_size);
MultiMIDIBlockHeaderPtr snd_MMDLoad(SInt8 *name, SInt32 offset);
MultiMIDIBlockHeaderPtr snd_MMDLoadByLoc(SInt32 sect_loc, SInt32 file_offset);

SoundBankPtr snd_FindBankByNum(SInt8 num);

void snd_UnloadAllBanks();
void snd_UnloadAllMMD();

void snd_WaitDMADone();

SInt32 snd_GetFreeSPUDMA();
void snd_FreeSPUDMA(SInt32 ch);

#endif // LOADER_H_
