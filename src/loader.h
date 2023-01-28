#ifndef LOADER_H_
#define LOADER_H_

#include "types.h"

extern SFXBlock2Ptr gBlockListHead;
extern bool gLimit2Meg;

enum BANK_FLAGS {
    BFLAG_XREF_RESOLVED = 2,
};

enum DATA_ID {
    SBV2_ID = 0x32764253,
    SBLK_ID = 0x6B6C4253,
};

void snd_InitLoader();
void snd_ShutdownLoader();

void *snd_IOPMemAlloc(SInt32 size, SInt32 use, SInt32 *act_size);
void snd_IOPMemFree(void *mem);

SInt32 snd_FileRead(SInt32 sect_loc, SInt32 offset, SInt32 size, SInt8 *buffer);
SoundBankPtr snd_ReadBank(UInt32 spu_mem_loc, UInt32 spu_mem_size);

SoundBankPtr snd_BankLoadEx(SInt8 *name, SInt32 offset, UInt32 spu_mem_loc, UInt32 spu_mem_size);
SoundBankPtr snd_BankLoadByLocEx(SInt32 sect_loc, SInt32 file_offset, UInt32 spu_mem_loc, UInt32 spu_mem_size);
SoundBankPtr snd_BankLoadFromEEEx(UInt32 ee_loc, UInt32 spu_mem_loc, UInt32 spu_mem_size);
SoundBankPtr snd_BankLoadFromIOPEx(void *iop_loc, UInt32 spu_mem_loc, UInt32 spu_mem_size);
MultiMIDIBlockHeaderPtr snd_MMDLoad(SInt8 *name, SInt32 offset);
MultiMIDIBlockHeaderPtr snd_MMDLoadFromIOPLoc(SInt8 *iop_loc);
MultiMIDIBlockHeaderPtr snd_MMDLoadFromDataSource();
MultiMIDIBlockHeaderPtr snd_MMDLoadByLoc(SInt32 sect_loc, SInt32 file_offset);

bool snd_BankLoaded(SoundBankPtr bank);
void snd_UnloadBlock(SFXBlock2Ptr block);
bool snd_BlockLoaded(SFXBlock2Ptr block);

SoundBankPtr snd_FindBankByNum(SInt8 num);

SFXBlock2Ptr snd_FindBlockByID(UInt32 id);
SFXBlock2Ptr snd_FindBlockByNum(SInt8 num);

void snd_UnloadAllBanks();
void snd_UnloadMMD(MultiMIDIBlockHeaderPtr mmd);
void snd_UnloadAllMMD();

SInt32 snd_RegisterMIDI(MIDIBlockHeaderPtr midi);
void snd_UnregisterMIDI(MIDIBlockHeaderPtr midi);
MIDIBlockHeaderPtr snd_FindMIDIBlock(UInt32 id);
void snd_InsertMIDIBlockInList(MIDIBlockHeaderPtr midi);
void snd_MIDIStreamGone(MIDIBlockHeaderPtr midi);

SInt32 snd_BankTransfer(SoundBankPtr bank, SInt8 *data, UInt32 data_size, SInt32 offset, SInt32 state, UInt32 spu_mem_loc, UInt32 spu_mem_size, SpuTransferCallbackProc callback);

void snd_ClearTransSema();
SInt32 snd_IsCurrentTransferComplete(bool wait);
SInt32 snd_EndBankTransfer(SoundBankPtr bank);

void snd_RemoveBank(SoundBankPtr bank);
void snd_RemoveBlock(SFXBlock2Ptr block);

void snd_SetCDSifReg(SInt32 reg8, SInt32 reg9);

void snd_WaitDMADone();

SInt32 snd_GetFreeSPUDMA();
void snd_FreeSPUDMA(SInt32 ch);

#endif // LOADER_H_
