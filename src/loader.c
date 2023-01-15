#include "loader.h"
#include "types.h"

/* data 1344 */ bool gLimit2Meg = false;
/* data 1348 */ SInt32 gFileHandle = -1;
/* data 134c */ SInt32 gFileStartSector = 0;
/* data 1350 */ SInt32 gFileStartOffset = 0;
/* data 1354 */ SInt32 gFileNextReadSector = 0;
/* data 1358 */ SInt32 gFileNextReadOffset = 0;
/* data 135c */ SInt32 gFileMarkSector = 0;
/* data 1360 */ SInt32 gFileMarkOffset = 0;
/* data 1364 */ SInt32 gLastLoadError = 0;
/* data 1368 */ UInt32 gEEDataLocStart = 0;
/* data 136c */ UInt32 gEEDataLocNext = 0;
/* data 1370 */ UInt32 gEEDataMark = 0;
/* data 1374 */ void *gIOPDataLocStart = NULL;
/* data 1378 */ ExternSndIOPAlloc gAllocProc = snd_IOPMemAlloc;
/* data 137c */ ExternSndIOPFree gFreeProc = snd_IOPMemFree;
/* data 1380 */ char *gEEStatusAddr = NULL;
/* data 1384 */ SoundBankPtr gBankListHead = NULL;
/* data 1388 */ SFXBlock2Ptr gBlockListHead = NULL;
/* data 138c */ MIDIBlockHeaderPtr gMIDIListHead = NULL;
/* data 1390 */ SInt32 gReadBufferHasSector = 0;
/* bss 1d0 */ SInt8 gFileLoadBuffer[2048];
/* bss 180 */ struct SndSystemStatus gStats;
/* bss 1c0 */ char gEEAlignBuffer[16];

/* 0001022c 0001030c */ void snd_InitLoader() {
    /* -0x18(sp) */ SemaParam sp;
}

/* 0001030c 00010350 */ void snd_ShutdownLoader() {}
/* 00010350 000103e0 */ void snd_RegisterIOPMemAllocator(/* 0x0(sp) */ ExternSndIOPAlloc AllocProc, /* 0x4(sp) */ ExternSndIOPFree FreeProc) {}
/* 000103e0 0001047c */ bool snd_OpenDataSourceByName(/* 0x0(sp) */ char *name, /* 0x4(sp) */ SInt32 offset) {}
/* 0001047c 00010504 */ bool snd_OpenDataSourceByLoc(/* 0x0(sp) */ SInt32 sector, /* 0x4(sp) */ SInt32 offset) {}
/* 00010504 00010564 */ bool snd_OpenDataSourceFromEE(/* 0x0(sp) */ UInt32 ee_loc) {}
/* 00010564 00010624 */ void snd_CloseDataSource() {}
/* 00010624 000108b8 */ SInt32 snd_SeekDataSource(/* 0x0(sp) */ SInt32 where, /* 0x4(sp) */ SInt32 from_where) {}
/* 000108b8 00010984 */ void snd_SetDataSourceMark() {}
/* 00010984 000109bc */ SInt32 snd_GetLastLoadError() {}
/* 000109bc 00010c88 */ SInt32 snd_ReadBytesFromEE(/* 0x0(sp) */ UInt32 ee_loc, /* 0x4(sp) */ void *iop_loc, /* 0x8(sp) */ SInt32 bytes) {
    /* -0x38(sp) */ SInt32 ret;
    /* -0x34(sp) */ sceSifReceiveData data_track;
    /* -0x18(sp) */ SInt32 full_blocks;
    /* -0x14(sp) */ SInt32 get_blocks;
    /* -0x10(sp) */ SInt32 read_bytes;
    /* -0xc(sp) */ SInt32 part_size;
}

/* 00010c88 00010ecc */ SInt32 snd_ReadBytes(/* 0x0(sp) */ void *buffer, /* 0x4(sp) */ SInt32 count) {
    /* -0x10(sp) */ SInt32 ret;
}

/* 00010ecc 00010fcc */ SoundBankPtr snd_BankLoadEx(/* 0x0(sp) */ SInt8 *name, /* 0x4(sp) */ SInt32 offset, /* 0x8(sp) */ UInt32 spu_mem_loc, /* 0xc(sp) */ UInt32 spu_mem_size) {
    /* -0x10(sp) */ SoundBankPtr ret;
}

/* 00010fcc 00011098 */ void snd_IterateBlockListToMonitor() {
    /* -0x10(sp) */ SFXBlock2Ptr block;
}

/* 00011098 000114c4 */ SoundBankPtr snd_ParseIOPBank(/* 0x0(sp) */ SInt8 *iop_loc, /* 0x4(sp) */ UInt32 spu_mem_loc, /* 0x8(sp) */ UInt32 spu_mem_size) {
    /* -0x28(sp) */ SInt32 result;
    /* -0x24(sp) */ SoundBankPtr bank_head;
    /* -0x20(sp) */ SFXBlock2Ptr block;
    /* -0x1c(sp) */ SInt8 *bank_head_ram;
    /* -0x18(sp) */ FileAttributesPtr fa;
    /* -0x14(sp) */ SInt32 size;
    /* -0x10(sp) */ SInt8 *data_loc;
}

/* 000114c4 00011d1c */ SoundBankPtr snd_ReadBank(/* 0x0(sp) */ UInt32 spu_mem_loc, /* 0x4(sp) */ UInt32 spu_mem_size) {
    /* -0x50(sp) */ SInt32 result;
    /* -0x4c(sp) */ SInt32 max_length;
    /* -0x48(sp) */ SoundBankPtr bank_head;
    /* -0x44(sp) */ SFXBlock2Ptr block_head;
    /* -0x40(sp) */ SInt8 *bank_head_ram;
    /* -0x3c(sp) */ SInt8 *bank_body;
    /* -0x38(sp) */ FileAttributesPtr fa;
    /* -0x34(sp) */ SInt32 block;
    /* -0x30(sp) */ SInt32 size;
    /* -0x2c(sp) */ SInt32 sram_offset;
    /* -0x28(sp) */ SInt8 buff[32];
}

/* 00011d1c 000122a0 */ SInt32 snd_FileRead(/* 0x0(sp) */ SInt32 sect_loc, /* 0x4(sp) */ SInt32 offset, /* 0x8(sp) */ SInt32 size, /* 0xc(sp) */ SInt8 *buffer) {
    /* -0x20(sp) */ sceCdRMode mode;
    /* -0x1c(sp) */ SInt32 bytes_needed;
    /* -0x18(sp) */ SInt32 move_bytes;
    /* -0x14(sp) */ SInt32 sector;
    /* -0x10(sp) */ SInt32 get_whole_sectors;
    /* -0xc(sp) */ SInt32 err;
}

/* 000122a0 00012394 */ SoundBankPtr snd_BankLoadByLocEx(/* 0x0(sp) */ SInt32 sect_loc, /* 0x4(sp) */ SInt32 file_offset, /* 0x8(sp) */ UInt32 spu_mem_loc, /* 0xc(sp) */ UInt32 spu_mem_size) {
    /* -0x10(sp) */ SoundBankPtr ret;
}

/* 00012394 00012480 */ SoundBankPtr snd_BankLoadFromEEEx(/* 0x0(sp) */ UInt32 ee_loc, /* 0x4(sp) */ UInt32 spu_mem_loc, /* 0x8(sp) */ UInt32 spu_mem_size) {
    /* -0x10(sp) */ SoundBankPtr ret;
}

/* 00012480 00012504 */ UInt32 snd_GetSRAMUsedByBank(/* 0x0(sp) */ SoundBankPtr bank) {
    /* -0x10(sp) */ SFXBlock2Ptr block;
}

/* 00012504 000125cc */ SoundBankPtr snd_BankLoadFromIOPEx(/* 0x0(sp) */ void *iop_loc, /* 0x4(sp) */ UInt32 spu_mem_loc, /* 0x8(sp) */ UInt32 spu_mem_size) {
    /* -0x10(sp) */ SoundBankPtr ret;
}

/* 000125cc 00012768 */ void snd_UnloadBank(/* 0x0(sp) */ SoundBankPtr bank) {
    /* -0x10(sp) */ SoundBankPtr mem_to_free;
}

/* 00012768 00012830 */ bool snd_BankLoaded(/* 0x0(sp) */ SoundBankPtr bank) {
    /* -0x10(sp) */ SoundBankPtr walker;
}

/* 00012830 00012904 */ void snd_UnloadBlock(/* 0x0(sp) */ SFXBlock2Ptr block) {}
/* 00012904 000129cc */ bool snd_BlockLoaded(/* 0x0(sp) */ SFXBlock2Ptr block) {
    /* -0x10(sp) */ SFXBlock2Ptr walker;
}

/* 000129cc 00012a48 */ MultiMIDIBlockHeaderPtr snd_MMDLoad(/* 0x0(sp) */ SInt8 *name, /* 0x4(sp) */ SInt32 offset) {
    /* -0x10(sp) */ MultiMIDIBlockHeaderPtr ret;
}

/* 00012a48 00012b90 */ MultiMIDIBlockHeaderPtr snd_MMDLoadFromIOPLoc(/* 0x0(sp) */ SInt8 *iop_loc) {
    /* -0x18(sp) */ SInt32 length;
    /* -0x14(sp) */ MultiMIDIBlockHeaderPtr mmd;
    /* -0x10(sp) */ FileAttributesPtr fa;
}

/* 00012b90 00012d44 */ MultiMIDIBlockHeaderPtr snd_MMDLoadFromDataSource() {
    /* -0x28(sp) */ SInt32 result;
    /* -0x24(sp) */ SInt32 length;
    /* -0x20(sp) */ MultiMIDIBlockHeaderPtr mmd;
    /* -0x1c(sp) */ FileAttributesPtr fa;
    /* -0x18(sp) */ SInt8 buff[16];
}

/* 00012d44 00012dc0 */ MultiMIDIBlockHeaderPtr snd_MMDLoadByLoc(/* 0x0(sp) */ SInt32 sect_loc, /* 0x4(sp) */ SInt32 file_offset) {
    /* -0x10(sp) */ MultiMIDIBlockHeaderPtr ret;
}

/* 00012dc0 00012e0c */ void snd_UnloadMMD(/* 0x0(sp) */ MultiMIDIBlockHeaderPtr mmd) {}
/* 00012e0c 00012ed4 */ void snd_UnloadAllMMD() {
    /* -0x10(sp) */ MIDIBlockHeaderPtr walk;
}

/* 00012ed4 0001354c */ SInt32 snd_BankTransfer(/* 0x0(sp) */ SoundBankPtr bank, /* 0x4(sp) */ SInt8 *data, /* 0x8(sp) */ UInt32 data_size, /* 0xc(sp) */ SInt32 offset, /* 0x10(sp) */ SInt32 state, /* 0x14(sp) */ UInt32 spu_mem_loc, /* 0x18(sp) */ UInt32 spu_mem_size, /* 0x1c(sp) */ SpuTransferCallbackProc callback) {
    /* -0x28(sp) */ UInt32 size;
    /* -0x24(sp) */ SInt32 ch;
    /* -0x20(sp) */ SInt32 msg;
    /* -0x1c(sp) */ void *sram_loc;
    /* -0x18(sp) */ UInt32 sram_size;
    /* -0x14(sp) */ SFXBlock2Ptr block;
    /* -0x10(sp) */ SInt32 dis;
    /* -0xc(sp) */ SInt32 oldstat;
}

/* 0001354c 000135a4 */ void snd_ClearTransSema() {}
/* 000135a4 0001361c */ SInt32 snd_IsCurrentTransferComplete(/* 0x0(sp) */ bool wait) {}
/* 0001361c 00013c00 */ SInt32 snd_EndBankTransfer(/* 0x0(sp) */ SoundBankPtr bank) {
    /* -0x18(sp) */ SInt32 x;
    /* -0x14(sp) */ SFXBlock2Ptr block;
    /* -0x10(sp) */ SoundBankPtr walker;
    /* -0x10(sp) */ TonePtr tp;
    {
        /* -0x10(sp) */ SFXBlock2Ptr walker;
    }
}

/* 00013c00 00013dac */ void snd_RemoveBank(/* 0x0(sp) */ SoundBankPtr bank) {
    /* -0x18(sp) */ SoundBankPtr walker;
    /* -0x14(sp) */ SoundBankPtr prev;
    /* -0x10(sp) */ SInt32 dis;
    /* -0xc(sp) */ SInt32 oldstat;
}

/* 00013dac 00013f28 */ void snd_RemoveBlock(/* 0x0(sp) */ SFXBlock2Ptr block) {
    /* -0x18(sp) */ SFXBlock2Ptr walker;
    /* -0x14(sp) */ SFXBlock2Ptr prev;
    /* -0x10(sp) */ SInt32 dis;
    /* -0xc(sp) */ SInt32 oldstat;
}

/* 00013f28 00013fbc */ void snd_UnloadAllBanks() {}
/* 00013fbc 00014218 */ void snd_ResolveBankXREFS() {
    /* -0x18(sp) */ SoundBankPtr bank;
    /* -0x14(sp) */ SFXBlock2Ptr block;
    /* -0x10(sp) */ SInt32 x;
    /* -0xc(sp) */ MIDISoundPtr midisound;
}

/* 00014218 000142f8 */ SoundBankPtr snd_FindBankByID(/* 0x0(sp) */ UInt32 id) {
    /* -0x10(sp) */ SoundBankPtr bank;
}

/* 000142f8 00014404 */ SFXBlock2Ptr snd_FindBlockByName(/* 0x0(sp) */ UInt32 *name) {
    /* -0x18(sp) */ SFXBlock2Ptr block;
    /* -0x14(sp) */ struct SFXBlockNames *names_header;
    /* -0x10(sp) */ UInt32 *buffer;
}

/* 00014404 000144b0 */ SFXBlock2Ptr snd_FindBlockByID(/* 0x0(sp) */ UInt32 id) {
    /* -0x10(sp) */ SFXBlock2Ptr block;
}

/* 000144b0 0001459c */ SoundBankPtr snd_FindBankByNum(/* -0x10(sp) */ SInt8 num) {
    /* -0xc(sp) */ SoundBankPtr bank;
}

/* 0001459c 0001464c */ SFXBlock2Ptr snd_FindBlockByNum(/* -0x10(sp) */ SInt8 num) {
    /* -0xc(sp) */ SFXBlock2Ptr block;
}

/* 0001464c 00014884 */ SInt32 snd_RegisterMIDI(/* 0x0(sp) */ MIDIBlockHeaderPtr midi) {
    /* -0x18(sp) */ MultiMIDIBlockHeaderPtr mmid;
    /* -0x14(sp) */ MIDIBlockHeaderPtr hold;
    /* -0x10(sp) */ SInt32 x;
}

/* 00014884 00014a60 */ void snd_UnregisterMIDI(/* 0x0(sp) */ MIDIBlockHeaderPtr midi) {
    /* -0x18(sp) */ MultiMIDIBlockHeaderPtr mmid;
    /* -0x14(sp) */ MIDIBlockHeaderPtr hold;
    /* -0x10(sp) */ SInt32 x;
    /* -0xc(sp) */ MIDIBlockHeaderPtr walk;
    /* data 1394 */ static SInt32 working_mmd;
}

/* 00014a60 00014b00 */ MIDIBlockHeaderPtr snd_FindMIDIBlock(/* 0x0(sp) */ UInt32 id) {
    /* -0x10(sp) */ MIDIBlockHeaderPtr walk;
}

/* 00014b00 00014ba8 */ void snd_InsertMIDIBlockInList(/* 0x0(sp) */ MIDIBlockHeaderPtr midi) {
    /* -0x10(sp) */ MIDIBlockHeaderPtr walk;
}

/* 00014ba8 00014d78 */ void snd_MIDIStreamGone(/* 0x0(sp) */ MIDIBlockHeaderPtr midi) {
    /* -0x18(sp) */ SoundBankPtr bank;
    /* -0x14(sp) */ SInt32 x;
    /* -0x10(sp) */ MIDISoundPtr midisound;
}

/* 00014d78 00015014 */ void snd_SetCDSifReg(/* 0x0(sp) */ SInt32 reg8, /* 0x4(sp) */ SInt32 reg9) {
    /* -0x28(sp) */ SInt32 did;
    /* -0x24(sp) */ SInt32 dis;
    /* -0x20(sp) */ SInt32 intr_state;
    /* -0x1c(sp) */ sceSifDmaData transData;
}

/* 00015014 000152f8 */ void *snd_IOPMemAlloc(/* 0x0(sp) */ SInt32 size, /* 0x4(sp) */ SInt32 use, /* 0x8(sp) */ SInt32 *act_size) {
    /* -0x20(sp) */ void *mem;
    /* -0x1c(sp) */ SInt32 max_avail;
    /* -0x18(sp) */ SInt32 from_where;
    /* -0x14(sp) */ SInt32 dis;
    /* -0x10(sp) */ SInt32 oldstat;
}

/* 000152f8 00015360 */ void snd_IOPMemFree(/* 0x0(sp) */ void *mem) {
    /* -0x10(sp) */ SInt32 dis;
    /* -0xc(sp) */ SInt32 oldstat;
}

/* 00015360 00015478 */ SInt32 snd_GetFreeSPUDMA() {
    /* -0x10(sp) */ SInt32 intr_state;
    /* -0xc(sp) */ SInt32 dis;
}

/* 00015478 000154e8 */ void snd_FreeSPUDMA(/* 0x0(sp) */ SInt32 ch) {}
/* 000154e8 0001556c */ void snd_WaitDMADone() {}
