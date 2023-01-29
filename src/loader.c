#include "loader.h"
#include "989snd.h"
#include "blocksnd.h"
#include "init.h"
#include "intr_code.h"
#include "intrman.h"
#include "ioman.h"
#include "libsd.h"
#include "midi.h"
#include "playsnd.h"
#include "sndhand.h"
#include "sram.h"
#include "stdio.h"
#include "stick.h"
#include "stream.h"
#include "sysclib.h"
#include "sysmem.h"
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
    memset(&sp, 0, sizeof(sp));
    sp.max = 1;
    gSPURAMTransSema = CreateSema(&sp);
    if (gSPURAMTransSema < 0) {
        snd_ShowError(115, 0, 0, 0, 0);
        while (true)
            ;
    }
    gTransfering = 0;
    gTransferDoneCallback = NULL;
    gBankListHead = NULL;
    gBlockListHead = NULL;
    gMIDIListHead = NULL;
    gAllocProc = snd_IOPMemAlloc;
    gFreeProc = snd_IOPMemFree;
}

/* 0001030c 00010350 */ void snd_ShutdownLoader() {
    DeleteSema(gSPURAMTransSema);
    gSPURAMTransSema = -1;
}

/* 00010350 000103e0 */ void snd_RegisterIOPMemAllocator(/* 0x0(sp) */ ExternSndIOPAlloc AllocProc, /* 0x4(sp) */ ExternSndIOPFree FreeProc) {
    if (AllocProc) {
        gAllocProc = AllocProc;
    } else {
        gAllocProc = snd_IOPMemAlloc;
    }

    if (FreeProc) {
        gFreeProc = FreeProc;
    } else {
        gFreeProc = snd_IOPMemFree;
    }
}

/* 000103e0 0001047c */ bool snd_OpenDataSourceByName(/* 0x0(sp) */ char *name, /* 0x4(sp) */ SInt32 offset) {
    gFileHandle = open(name, O_RDONLY);
    if (gFileHandle < 0) {
        return false;
    }
    gFileStartOffset = offset;
    lseek(gFileHandle, offset, SEEK_SET);
    return true;
}

/* 0001047c 00010504 */ bool snd_OpenDataSourceByLoc(/* 0x0(sp) */ SInt32 sector, /* 0x4(sp) */ SInt32 offset) {
    gFileStartSector = sector;
    gFileStartOffset = offset;
    gFileNextReadSector = sector;
    gFileNextReadOffset = offset;
    gFileMarkSector = sector;
    gFileMarkOffset = offset;
    gFileHandle = 1;
    return true;
}

/* 00010504 00010564 */ bool snd_OpenDataSourceFromEE(/* 0x0(sp) */ UInt32 ee_loc) {
    gEEDataLocNext = ee_loc;
    gEEDataLocNext = ee_loc;
    gEEDataMark = ee_loc;
    gFileHandle = 2;
    return true;
}

/* 00010564 00010624 */ void snd_CloseDataSource() {
    if (gFileHandle == -1) {
        return;
    }

    if (gFileHandle != 0) {
        gFileStartSector = 0;
        gFileStartOffset = 0;
        gFileNextReadSector = 0;
        gFileStartOffset = 0;
    } else if (gEEDataLocStart != 0) {
        gEEDataLocStart = 0;
    } else {
        close(gFileHandle);
    }

    gFileHandle = -1;
}

/* 00010624 000108b8 */ SInt32 snd_SeekDataSource(/* 0x0(sp) */ SInt32 where, /* 0x4(sp) */ SInt32 from_where) {
    if (gFileHandle == -1) {
        return 0;
    }

    if (gFileStartSector) {
        if (from_where) {
            gFileNextReadSector = gFileMarkSector;
            gFileNextReadOffset = gFileMarkOffset + where;
        } else {
            gFileNextReadSector = gFileStartSector;
            gFileNextReadOffset = gFileStartOffset + where;
        }
        if (gFileNextReadOffset >= 2048) {
            gFileNextReadSector += gFileNextReadOffset / 2048;
            gFileNextReadOffset %= 2048;
        }
        return where;
    }

    if (gEEDataLocStart) {
        if (from_where) {
            gEEDataLocNext = gEEDataMark + where;
        } else {
            gEEDataLocNext = gEEDataLocStart + where;
        }

        return gEEDataLocNext;
    }

    if (from_where) {
        return lseek(gFileHandle, where + gFileMarkOffset, 0);
    } else {
        return lseek(gFileHandle, where + gFileStartOffset, 0);
    }

    return 0;
}

/* 000108b8 00010984 */ void snd_SetDataSourceMark() {
    if (gFileHandle == -1) {
        return;
    }

    if (gFileStartSector) {
        gFileMarkSector = gFileNextReadSector;
        gFileMarkOffset = gFileNextReadOffset;

        return;
    }

    if (gEEDataLocStart) {
        gEEDataMark = gEEDataLocNext;

        return;
    }

    gFileMarkOffset = lseek(gFileHandle, 0, SEEK_CUR);
}

/* 00010984 000109bc */ SInt32 snd_GetLastLoadError() {
    return gLastLoadError;
}

/* 000109bc 00010c88 */ SInt32 snd_ReadBytesFromEE(/* 0x0(sp) */ UInt32 ee_loc, /* 0x4(sp) */ void *iop_loc, /* 0x8(sp) */ SInt32 bytes) {
    /* -0x38(sp) */ SInt32 ret;
    /* -0x34(sp) */ sceSifReceiveData data_track;
    /* -0x18(sp) */ SInt32 full_blocks;
    /* -0x14(sp) */ SInt32 get_blocks = 0;
    /* -0x10(sp) */ SInt32 read_bytes;
    /* -0xc(sp) */ SInt32 part_size;

    if ((ee_loc & 0xf) != 0) {
        part_size = 16 - (ee_loc & 0xF);
        ee_loc -= ee_loc & 0xF;

        if (sceSifGetOtherData(&data_track, (void *)ee_loc, &gEEAlignBuffer, 16, 0) < 0) {
            return 0;
        }

        memcpy(iop_loc, &gFileLoadBuffer - part_size, part_size);
        bytes -= part_size;
        iop_loc += part_size;
        ee_loc += 16;
    }

    for (full_blocks = bytes / 16; full_blocks != 0; full_blocks -= get_blocks) {
        get_blocks = full_blocks;
        if (full_blocks >= 0x4000) {
            get_blocks = 0x3FFF;
        }

        if (sceSifGetOtherData(&data_track, (void *)ee_loc, iop_loc, 16 * get_blocks, 0) < 0) {
            return 0;
        }

        bytes -= 16 * get_blocks;
        iop_loc += 16 * get_blocks;
        ee_loc += 16 * get_blocks;
    }

    if (bytes != 0) {
        if (sceSifGetOtherData(&data_track, (void *)ee_loc, &gEEAlignBuffer, 16, 0) < 0) {
            return 0;
        }

        memcpy(iop_loc, &gEEAlignBuffer, bytes);
    }

    return bytes;
}

/* 00010c88 00010ecc */ SInt32 snd_ReadBytes(/* 0x0(sp) */ void *buffer, /* 0x4(sp) */ SInt32 count) {
    /* -0x10(sp) */ SInt32 ret;

    if (gFileHandle == -1) {
        return 0;
    }

    if (gFileStartSector != 0) {
        if (snd_FileRead(gFileNextReadSector, gFileNextReadOffset, count, buffer)) {
            gFileNextReadOffset += count;
            if (gFileNextReadOffset >= 2048) {
                gFileNextReadSector += gFileNextReadOffset / 2048;
                gFileNextReadOffset %= 2048;
            }
            return count;
        }

        return 0;
    }

    if (gEEDataLocStart != 0) {
        ret = snd_ReadBytesFromEE(gEEDataLocNext, buffer, count);
        if (ret == count) {
            gEEDataLocNext += count;
            return ret;
        }

        gLastLoadError = 257;
        return 0;
    }

    ret = read(gFileHandle, buffer, count);
    if (ret != count) {
        gLastLoadError = 257;
    }

    return ret;
}

/* 00010ecc 00010fcc */ SoundBankPtr snd_BankLoadEx(/* 0x0(sp) */ SInt8 *name, /* 0x4(sp) */ SInt32 offset, /* 0x8(sp) */ UInt32 spu_mem_loc, /* 0xc(sp) */ UInt32 spu_mem_size) {
    /* -0x10(sp) */ SoundBankPtr ret;

    if (snd_OpenDataSourceByName(name, offset) == false) {
        gLastLoadError = 256;
        return 0;
    }

    gLastLoadError = 0;
    ret = snd_ReadBank(spu_mem_loc, spu_mem_size);
    snd_CloseDataSource();
    if (ret != NULL && g989Monitor != NULL) {
        if (g989Monitor->bnk_load != NULL) {
            g989Monitor->bnk_load(ret, 1);
        }
    }

    return ret;
}

/* 00010fcc 00011098 */ void snd_IterateBlockListToMonitor() {
    /* -0x10(sp) */ SFXBlock2Ptr block;

    if (g989Monitor != NULL && g989Monitor->bnk_load != NULL) {
        for (block = gBlockListHead; block != NULL; block = block->NextBlock) {
            g989Monitor->bnk_load(block, 1);
        }
    }
}

/* 00011098 000114c4 */ SoundBankPtr snd_ParseIOPBank(/* 0x0(sp) */ SInt8 *iop_loc, /* 0x4(sp) */ UInt32 spu_mem_loc, /* 0x8(sp) */ UInt32 spu_mem_size) {
    /* -0x28(sp) */ SInt32 result;
    /* -0x24(sp) */ SoundBankPtr bank_head;
    /* -0x20(sp) */ SFXBlock2Ptr block;
    /* -0x1c(sp) */ SInt8 *bank_head_ram;
    /* -0x18(sp) */ FileAttributesPtr fa;
    /* -0x14(sp) */ SInt32 size;
    /* -0x10(sp) */ SInt8 *data_loc;
    fa = iop_loc;

    if (fa->type != 1 && fa->type != 3) {
        snd_ShowError(84, 0, 0, 0, 0);
        gLastLoadError = 263;
        return NULL;
    }

    size = fa->where[0].size;
    if (fa->num_chunks >= 3) {
        // make room for putting the MMD ptr before the bank.
        size += 4;
    }

    bank_head_ram = gAllocProc(size, 1, 0);
    if (bank_head_ram == NULL) {
        snd_ShowError(15, size, 0, 0, 0);
        gLastLoadError = 258;
        return NULL;
    }

    if (fa->num_chunks == 3) {
        bank_head = (SoundBankPtr)(bank_head_ram + 4);
        memcpy(bank_head, iop_loc + fa->where[0].offset, fa->where[0].size);
    } else {
        bank_head = (SoundBankPtr)bank_head_ram;
        memcpy(bank_head, iop_loc + fa->where[0].offset, fa->where[0].size);
    }

    if (fa->where[1].size != 0) {
        snd_BankTransfer(bank_head,
                         iop_loc + fa->where[1].offset,
                         fa->where[1].size,
                         0,
                         0,
                         spu_mem_loc,
                         spu_mem_size,
                         NULL);
        snd_IsCurrentTransferComplete(true);
        snd_EndBankTransfer(bank_head);
    } else {
        if (bank_head->DataID == SBV2_ID) {
            bank_head->FirstSound = (SoundPtr)((SInt8 *)bank_head->FirstSound + (UInt32)bank_head);
            bank_head->FirstProg = (ProgPtr)((SInt8 *)bank_head->FirstProg + (UInt32)bank_head);
            bank_head->FirstTone = (TonePtr)((SInt8 *)bank_head->FirstTone + (UInt32)bank_head);
        } else {
            block = (SFXBlock2Ptr)bank_head_ram;
            block->FirstSound = (SFX2Ptr)((SInt8 *)block->FirstSound + (UInt32)bank_head);
            block->FirstGrain = (SFXGrain2Ptr)((SInt8 *)block->FirstGrain + (UInt32)bank_head);
        }
        bank_head->Flags |= 1;
        snd_EndBankTransfer(bank_head);
    }

    if (fa->num_chunks == 3) {
        // whyyyy
        *(UInt32 *)bank_head_ram = (UInt32)snd_MMDLoadFromIOPLoc(iop_loc + fa->where[2].offset);
        if (*(UInt32 *)bank_head_ram == 0) {
            snd_RemoveBank(bank_head);
            gFreeProc(bank_head_ram);
            snd_ShowError(19, 0, 0, 0, 0);
            return NULL;
        }
        bank_head->Flags |= 8;
    }

    return bank_head;
}

/* 000114c4 00011d1c */ SoundBankPtr snd_ReadBank(/* 0x0(sp) */ UInt32 spu_mem_loc, /* 0x4(sp) */ UInt32 spu_mem_size) {
    /* -0x50(sp) */ SInt32 result;
    /* -0x4c(sp) */ SInt32 max_length;
    /* -0x48(sp) */ SoundBankPtr bank_head;
    /* -0x44(sp) */ SFXBlock2Ptr block_head = NULL;
    /* -0x40(sp) */ SInt8 *bank_head_ram;
    /* -0x3c(sp) */ SInt8 *bank_body = NULL;
    /* -0x38(sp) */ FileAttributesPtr fa;
    /* -0x34(sp) */ SInt32 block;
    /* -0x30(sp) */ SInt32 size;
    /* -0x2c(sp) */ SInt32 sram_offset;
    /* -0x28(sp) */ SInt8 buff[32];

    if (snd_ReadBytes(&buff, 32) != 32) {
        gReadBufferHasSector = 0;
        snd_ShowError(14, gLastLoadError, 0, 0, 0);
        return NULL;
    }

    fa = buff;
    if (fa->type != 1 && fa->type != 3) {
        snd_ShowError(84, (UInt32)&buff, gFileStartSector, gReadBufferHasSector, 0);
        gReadBufferHasSector = -1;
        gLastLoadError = 263;
        return 0;
    }

    size = fa->where[0].size;
    if (fa->num_chunks >= 3) {
        // Make room for the MMD ptr
        size += 4;
    }

    bank_head_ram = gAllocProc(size, 1, 0);
    if (bank_head_ram == NULL) {
        snd_ShowError(15, size, 0, 0, 0);
        gLastLoadError = 258;
        return NULL;
    }

    if (fa->num_chunks == 3) {
        bank_head = (SoundBankPtr)(bank_head_ram + 4);
    } else {
        bank_head = (SoundBankPtr)bank_head_ram;
    }

    if (fa->where[1].size != 0) {
        bank_body = gAllocProc(fa->where[1].size, 8, &max_length);
        if (fa->where[1].size < max_length) {
            max_length = fa->where[1].size;
        }

        if (bank_body == NULL) {
            snd_ShowError(16, 0, 0, 0, 0);
            gFreeProc(bank_head_ram);
            gLastLoadError = 258;
            return NULL;
        }
    }

    snd_SeekDataSource(fa->where[0].offset, 0);
    if ((snd_ReadBytes(bank_head, fa->where[0].size)) != fa->where[0].size) {
        gReadBufferHasSector = 0;
        snd_ShowError(17, gLastLoadError, 0, 0, 0);
        gFreeProc(bank_head_ram);
        gFreeProc(bank_body);
        return 0;
    }

    if (bank_head->DataID == SBLK_ID && bank_head->Version < 2) {
        snd_ShowError(113, 0, 0, 0, 0);
        gFreeProc(bank_head_ram);
        gFreeProc(bank_body);
        return NULL;
    }

    if (fa->where[1].size) {
        block = 0;
        sram_offset = 0;
        while (fa->where[1].size) {
            if (snd_ReadBytes(bank_body, max_length) != max_length) {
                gReadBufferHasSector = 0;
                snd_ShowError(14, gLastLoadError, 0, 0, 0);
                gFreeProc(bank_head_ram);
                gFreeProc(bank_body);
                return NULL;
            }

            if (block == 0) {
                result = snd_BankTransfer(bank_head, bank_body, max_length, 0, 0, spu_mem_loc, spu_mem_size, NULL);
            } else {
                result = snd_BankTransfer(bank_head, bank_body, max_length, sram_offset, 1, spu_mem_loc, spu_mem_size, NULL);
            }

            if (result < 0) {
                snd_ShowError(18, 0, 0, 0, 0);
                gFreeProc(bank_head_ram);
                gFreeProc(bank_body);
                return 0;
            }

            snd_IsCurrentTransferComplete(true);
            sram_offset += max_length;
            block++;
            fa->where[1].size -= max_length;
            if (fa->where[1].size < max_length) {
                max_length = fa->where[1].size;
            }
        }

        snd_EndBankTransfer(bank_head);
        gFreeProc(bank_body);
    } else {
        if (bank_head->DataID == SBV2_ID) {
            bank_head->FirstSound = (SoundPtr)((SInt8 *)bank_head->FirstSound + (UInt32)bank_head);
            bank_head->FirstProg = (ProgPtr)((SInt8 *)bank_head->FirstProg + (UInt32)bank_head);
            bank_head->FirstTone = (TonePtr)((SInt8 *)bank_head->FirstTone + (UInt32)bank_head);
            bank_head->Flags |= 1u;
        } else {
            block_head = (SFXBlock2Ptr)bank_head;
            block_head->FirstSound = (SFX2Ptr)((SInt8 *)block_head->FirstSound + (UInt32)block_head);
            block_head->FirstGrain = (SFXGrain2Ptr)((SInt8 *)block_head->FirstGrain + (UInt32)block_head);
            block_head->Flags |= 1u;
        }

        snd_EndBankTransfer(bank_head);
    }

    if (fa->num_chunks == 3) {
        snd_SeekDataSource(fa->where[2].offset, 0);
        *(UInt32 *)bank_head_ram = (UInt32)snd_MMDLoadFromDataSource();
        if (*(UInt32 *)bank_head_ram == 0) {
            snd_RemoveBank(bank_head);
            gFreeProc(bank_head_ram);
            snd_ShowError(19, 0, 0, 0, 0);
            return NULL;
        }

        bank_head->Flags |= 8;
    }

    return bank_head;
}

/* 00011d1c 000122a0 */ SInt32 snd_FileRead(/* 0x0(sp) */ SInt32 sect_loc, /* 0x4(sp) */ SInt32 offset, /* 0x8(sp) */ SInt32 size, /* 0xc(sp) */ SInt8 *buffer) {
    /* -0x20(sp) */ sceCdRMode mode;
    /* -0x1c(sp) */ SInt32 bytes_needed;
    /* -0x18(sp) */ SInt32 move_bytes;
    /* -0x14(sp) */ SInt32 sector;
    /* -0x10(sp) */ SInt32 get_whole_sectors;
    /* -0xc(sp) */ SInt32 err;

    move_bytes = 0;
    WaitSema(gFileReadMutex);
    snd_StreamSafeCdSync(0);
    snd_StreamSafeCdSync(16);
    snd_SetCDSifReg(1, 0);
    bytes_needed = size;
    sector = sect_loc + offset / 2048;
    offset %= 2048;

    // start not sector aligned
    if (size < 2048 || offset != 0) {
        if (sector != gReadBufferHasSector) {
            err = snd_StreamSafeCdRead(sector, 1, &gFileLoadBuffer);
            if (err != 0 && !gPrefs_Silent) {
                printf("Error Kicking Off Read!\n");
            }
            snd_StreamSafeCdSync(0);

            if (!err || (err = snd_StreamSafeCdGetError()) != 0) {
                gReadBufferHasSector = 0;
                if (err == 0) {
                    err = 48;
                }
                snd_SetCDSifReg(0, err);
                gLastLoadError = err;
                SignalSema(gFileReadMutex);
                return 0;
            }

            gReadBufferHasSector = sector;
        }

        move_bytes = 2048 - offset % 2048;
        if (size < move_bytes) {
            move_bytes = size;
        }
        memcpy(buffer, &gFileLoadBuffer + offset % 2048, move_bytes);
        bytes_needed = size - move_bytes;
        if (bytes_needed == 0) {
            snd_SetCDSifReg(0, 0);
            SignalSema(gFileReadMutex);
            return 1;
        }
        sector++;
    }

    // read whole sectors aligned
    get_whole_sectors = bytes_needed / 2048;
    if (get_whole_sectors != 0) {
        err = snd_StreamSafeCdRead(sector, get_whole_sectors, &buffer[move_bytes]);
        snd_StreamSafeCdSync(0);

        if (!err || (err = snd_StreamSafeCdGetError()) != 0) {
            gReadBufferHasSector = 0;
            if (err == 0) {
                err = 48;
            }
            snd_SetCDSifReg(0, err);
            gLastLoadError = err;
            SignalSema(gFileReadMutex);
            return 0;
        }
    }

    // read remainder
    sector += get_whole_sectors;
    bytes_needed %= 2048;
    if (bytes_needed != 0) {
        if (sector != gReadBufferHasSector) {
            err = snd_StreamSafeCdRead(sector, 1, &gFileLoadBuffer);
            snd_StreamSafeCdSync(0);

            if (!err || (err = snd_StreamSafeCdGetError()) != 0) {
                gReadBufferHasSector = 0;
                if (err == 0) {
                    err = 48;
                }
                snd_SetCDSifReg(0, err);
                gLastLoadError = err;
                SignalSema(gFileReadMutex);
                return 0;
            }

            gReadBufferHasSector = 0;
        }

        memcpy(&buffer[size - bytes_needed], &gFileLoadBuffer, bytes_needed);
    }

    snd_SetCDSifReg(0, 0);
    SignalSema(gFileReadMutex);
    return 1;
}

/* 000122a0 00012394 */ SoundBankPtr snd_BankLoadByLocEx(/* 0x0(sp) */ SInt32 sect_loc, /* 0x4(sp) */ SInt32 file_offset, /* 0x8(sp) */ UInt32 spu_mem_loc, /* 0xc(sp) */ UInt32 spu_mem_size) {
    /* -0x10(sp) */ SoundBankPtr ret;

    if (!snd_OpenDataSourceByLoc(sect_loc, file_offset)) {
        return NULL;
    }

    gLastLoadError = 0;
    ret = snd_ReadBank(spu_mem_loc, spu_mem_size);
    snd_CloseDataSource();

    if (ret != NULL && g989Monitor != NULL && g989Monitor->bnk_load != NULL) {
        g989Monitor->bnk_load(ret, 1);
    }

    return ret;
}

/* 00012394 00012480 */ SoundBankPtr snd_BankLoadFromEEEx(/* 0x0(sp) */ UInt32 ee_loc, /* 0x4(sp) */ UInt32 spu_mem_loc, /* 0x8(sp) */ UInt32 spu_mem_size) {
    /* -0x10(sp) */ SoundBankPtr ret;

    if (!snd_OpenDataSourceFromEE(ee_loc)) {
        return NULL;
    }

    gLastLoadError = 0;
    ret = snd_ReadBank(spu_mem_loc, spu_mem_size);
    snd_CloseDataSource();

    if (ret != NULL && g989Monitor != NULL && g989Monitor->bnk_load != NULL) {
        g989Monitor->bnk_load(ret, 1);
    }

    return ret;
}

/* 00012480 00012504 */ UInt32 snd_GetSRAMUsedByBank(/* 0x0(sp) */ SoundBankPtr bank) {
    /* -0x10(sp) */ SFXBlock2Ptr block;
    if (bank->DataID == SBLK_ID) {
        block = bank;
        return block->SRAMAllocSize;
    }

    return bank->VagDataSize;
}

/* 00012504 000125cc */ SoundBankPtr snd_BankLoadFromIOPEx(/* 0x0(sp) */ void *iop_loc, /* 0x4(sp) */ UInt32 spu_mem_loc, /* 0x8(sp) */ UInt32 spu_mem_size) {
    /* -0x10(sp) */ SoundBankPtr ret;

    gLastLoadError = 0;
    ret = snd_ParseIOPBank(iop_loc, spu_mem_loc, spu_mem_size);
    if (ret != NULL && g989Monitor != NULL && g989Monitor->bnk_load != NULL) {
        g989Monitor->bnk_load(ret, 1);
    }

    return ret;
}

/* 000125cc 00012768 */ void snd_UnloadBank(/* 0x0(sp) */ SoundBankPtr bank) {
    /* -0x10(sp) */ SoundBankPtr mem_to_free = bank;

    if (bank == NULL || bank == (SoundBankPtr)-1) {
        snd_ShowError(20, 0, 0, 0, 0);
        return;
    }

    if (bank->DataID == SBLK_ID) {
        snd_UnloadBlock(bank);
        return;
    }

    if (!snd_BankLoaded(bank)) {
        snd_ShowError(21, 0, 0, 0, 0);
        return;
    }

    if (g989Monitor != NULL && g989Monitor->bnk_load != NULL) {
        g989Monitor->bnk_load(bank, 0);
    }

    if ((bank->Flags & 8) != 0) {
        mem_to_free = (SoundBankPtr)((SInt8 *)bank - 4);
        snd_UnloadMMD((MultiMIDIBlockHeaderPtr)(*(UInt32 *)mem_to_free));
    }

    snd_RemoveBank(bank);
    gFreeProc(mem_to_free);
}

/* 00012768 00012830 */ bool snd_BankLoaded(/* 0x0(sp) */ SoundBankPtr bank) {
    /* -0x10(sp) */ SoundBankPtr walker = gBankListHead;
    if (bank == NULL || bank == (SoundBankPtr)-1) {
        return false;
    }

    while (walker != NULL) {
        if (bank == walker) {
            return true;
        }

        walker = walker->NextBank;
    }

    return false;
}

/* 00012830 00012904 */ void snd_UnloadBlock(/* 0x0(sp) */ SFXBlock2Ptr block) {
    if (!snd_BlockLoaded(block)) {
        snd_ShowError(22, 0, 0, 0, 0);
        return;
    }

    if (g989Monitor != NULL && g989Monitor->bnk_load != NULL) {
        g989Monitor->bnk_load(block, 0);
    }

    snd_RemoveBlock(block);
    gFreeProc(block);
}

/* 00012904 000129cc */ bool snd_BlockLoaded(/* 0x0(sp) */ SFXBlock2Ptr block) {
    /* -0x10(sp) */ SFXBlock2Ptr walker = block;
    if (block == NULL || block == (SFXBlock2Ptr)-1) {
        return false;
    }

    while (walker != NULL) {
        if (block == walker) {
            return true;
        }

        walker = walker->NextBlock;
    }

    return false;
}

/* 000129cc 00012a48 */ MultiMIDIBlockHeaderPtr snd_MMDLoad(/* 0x0(sp) */ SInt8 *name, /* 0x4(sp) */ SInt32 offset) {
    /* -0x10(sp) */ MultiMIDIBlockHeaderPtr ret;
    if (!snd_OpenDataSourceByName(name, offset)) {
        return NULL;
    }

    ret = snd_MMDLoadFromDataSource();
    snd_CloseDataSource();
    return ret;
}

/* 00012a48 00012b90 */ MultiMIDIBlockHeaderPtr snd_MMDLoadFromIOPLoc(/* 0x0(sp) */ SInt8 *iop_loc) {
    /* -0x18(sp) */ SInt32 length;
    /* -0x14(sp) */ MultiMIDIBlockHeaderPtr mmd;
    /* -0x10(sp) */ FileAttributesPtr fa;
    fa = (FileAttributesPtr)iop_loc;
    length = fa->where[0].size;

    mmd = gAllocProc(length, 3, 0);
    if (mmd == NULL) {
        snd_ShowError(23, length, 0, 0, 0);
        gLastLoadError = 258;
        return NULL;
    }

    memcpy(mmd, iop_loc + fa->where[0].offset, length);

    if (!snd_RegisterMIDI(mmd)) {
        gFreeProc(mmd);
        snd_ShowError(121, 0, 0, 0, 0);
        gLastLoadError = 263;
        return NULL;
    }

    return mmd;
}

/* 00012b90 00012d44 */ MultiMIDIBlockHeaderPtr snd_MMDLoadFromDataSource() {
    /* -0x28(sp) */ SInt32 result;
    /* -0x24(sp) */ SInt32 length;
    /* -0x20(sp) */ MultiMIDIBlockHeaderPtr mmd;
    /* -0x1c(sp) */ FileAttributesPtr fa;
    /* -0x18(sp) */ SInt8 buff[16];

    snd_SetDataSourceMark();
    if (snd_ReadBytes(buff, 16) != 16) {
        return NULL;
    }

    fa = buff;
    length = fa->where[0].size;
    snd_SeekDataSource(fa->where[0].offset, 1);

    mmd = gAllocProc(length, 3, 0);
    if (mmd == NULL) {
        snd_ShowError(23, length, 0, 0, 0);
        gLastLoadError = 258;
        return NULL;
    }

    if (snd_ReadBytes(mmd, length) != length) {
        gFreeProc(mmd);
        return NULL;
    }

    if (!snd_RegisterMIDI(mmd)) {
        gFreeProc(mmd);
        snd_ShowError(121, 0, 0, 0, 0);
        gLastLoadError = 263;
        return NULL;
    }

    return mmd;
}

/* 00012d44 00012dc0 */ MultiMIDIBlockHeaderPtr snd_MMDLoadByLoc(/* 0x0(sp) */ SInt32 sect_loc, /* 0x4(sp) */ SInt32 file_offset) {
    /* -0x10(sp) */ MultiMIDIBlockHeaderPtr ret;

    if (!snd_OpenDataSourceByLoc(sect_loc, file_offset)) {
        return NULL;
    }

    ret = snd_MMDLoadFromDataSource();
    snd_CloseDataSource();
    return ret;
}

/* 00012dc0 00012e0c */ void snd_UnloadMMD(/* 0x0(sp) */ MultiMIDIBlockHeaderPtr mmd) {
    snd_UnregisterMIDI(mmd);
    gFreeProc(mmd);
}

/* 00012e0c 00012ed4 */ void snd_UnloadAllMMD() {
    /* -0x10(sp) */ MIDIBlockHeaderPtr walk;
    for (walk = gMIDIListHead; walk != NULL; walk = walk->NextMIDIBlock) {
        if (walk->DataID == MMID_ID) {
            snd_UnloadMMD(walk);
        }
    }
    while (gMIDIListHead != NULL) {
        snd_UnloadMMD(gMIDIListHead);
    }
}

/* 00012ed4 0001354c */ SInt32 snd_BankTransfer(/* 0x0(sp) */ SoundBankPtr bank, /* 0x4(sp) */ SInt8 *data, /* 0x8(sp) */ UInt32 data_size, /* 0xc(sp) */ SInt32 offset, /* 0x10(sp) */ SInt32 state, /* 0x14(sp) */ UInt32 spu_mem_loc, /* 0x18(sp) */ UInt32 spu_mem_size, /* 0x1c(sp) */ SpuTransferCallbackProc callback) {
    /* -0x28(sp) */ UInt32 size;
    /* -0x24(sp) */ SInt32 ch;
    /* -0x20(sp) */ SInt32 msg = 0;
    /* -0x1c(sp) */ void *sram_loc;
    /* -0x18(sp) */ UInt32 sram_size;
    /* -0x14(sp) */ SFXBlock2Ptr block = bank;
    /* -0x10(sp) */ SInt32 dis;
    /* -0xc(sp) */ SInt32 oldstat;

    // BUG originally comparing address of function
    if (!snd_SystemRunning()) {
        snd_ShowError(24, 0, 0, 0, 0);
        return -2;
    }

    if (gTransfering) {
        snd_ShowError(25, 0, 0, 0, 0);
        gLastLoadError = 260;
        return -10;
    }

    if (bank->DataID == SBV2_ID) {
        if (spu_mem_size != 0) {
            if (spu_mem_size < bank->VagDataSize) {
                snd_ShowError(105, spu_mem_size, bank->VagDataSize, 0, 0);
            }
            bank->VagDataSize = spu_mem_size;
        }

        if (spu_mem_loc != 0) {
            bank->Flags &= ~4;
            bank->VagsInSR = (void *)spu_mem_loc;
        }

        sram_size = bank->VagDataSize;
        sram_loc = bank->VagsInSR;
    } else {
        if (spu_mem_size != 0) {
            if (spu_mem_size < block->VagDataSize) {
                snd_ShowError(105, spu_mem_size, block->VagDataSize, 0, 0);
            }
            block->SRAMAllocSize = spu_mem_size;
        }

        if (spu_mem_loc != 0) {
            block->Flags &= ~4;
            block->VagsInSR = (void *)spu_mem_loc;
        }

        sram_size = block->SRAMAllocSize;
        sram_loc = block->VagsInSR;
    }

    if (state == 0) {
        if ((bank->Flags & 4) != 0) {
            dis = CpuSuspendIntr(&oldstat);
            sram_loc = (void *)snd_SRAMMalloc(sram_size);
            if (!dis) {
                CpuResumeIntr(oldstat);
            }

            if (sram_loc == 0) {
                snd_ShowError(26, sram_size, 0, 0, 0);
                gLastLoadError = 259;
                return -11;
            }

            if (bank->DataID == SBV2_ID) {
                bank->VagsInSR = sram_loc;
            } else {
                block->VagsInSR = sram_loc;
            }
        } else {
            dis = CpuSuspendIntr(&oldstat);
            if (snd_SRAMMarkUsed((UInt32)sram_loc, sram_size) == 0) {
                if (!dis) {
                    CpuResumeIntr(oldstat);
                }
                snd_ShowError(27, sram_size, (UInt32)sram_loc, 0, 0);
                gLastLoadError = 259;
                return -11;
            }

            if (!dis) {
                CpuResumeIntr(oldstat);
            }
        }

        if ((bank->Flags & 1) == 0) {
            if (bank->DataID == SBV2_ID) {
                bank->FirstSound = (SoundPtr)((SInt8 *)bank->FirstSound + (UInt32)bank);
                bank->FirstProg = (ProgPtr)((SInt8 *)bank->FirstProg + (UInt32)bank);
                bank->FirstTone = (TonePtr)((SInt8 *)bank->FirstTone + (UInt32)bank);
                bank->Flags |= 1u;
            } else {
                block->FirstSound = (SFX2Ptr)((SInt8 *)block->FirstSound + (UInt32)block);
                block->FirstGrain = (SFXGrain2Ptr)((SInt8 *)block->FirstGrain + (UInt32)block);
                block->Flags |= 1u;
            }
        }
    }

    gTransferDoneCallback = callback;

    while (true) {
        ch = snd_GetFreeSPUDMA();
        if (ch != -1) {
            break;
        }

        if (!msg) {
            msg = 1;
        }

        while (ch < 1000) {
            ch++;
        }
    }

    gTransfering = ch + 1;
    snd_ClearTransSema();
    sceSdSetTransIntrHandler(ch, snd_TransCallback, NULL);
    if (sceSdVoiceTrans(ch, 0, data, sram_loc + offset, data_size) < data_size) {
        snd_FreeSPUDMA(gTransfering - 1);
        gTransfering = 0;
        gLastLoadError = 261;
        return -12;
    }

    return 0;
}

/* 0001354c 000135a4 */ void snd_ClearTransSema() {
    while (PollSema(gSPURAMTransSema) != -419)
        ;
}

/* 000135a4 0001361c */ SInt32 snd_IsCurrentTransferComplete(/* 0x0(sp) */ bool wait) {
    if (wait) {
        WaitSema(gSPURAMTransSema);
    }

    return gTransfering == 0;
}

/* 0001361c 00013c00 */ SInt32 snd_EndBankTransfer(/* 0x0(sp) */ SoundBankPtr bank) {
    /* -0x18(sp) */ SInt32 x;
    /* -0x14(sp) */ SFXBlock2Ptr block;
    /* -0x10(sp) */ TonePtr tp;
    if (bank->DataID == SBV2_ID) {
        /* -0x10(sp) */ SoundBankPtr walker;
        for (x = 0; x < bank->NumTones; x++) {
            bank->FirstTone[x].VAGInSR = (void *)((UInt32)bank->FirstTone[x].VAGInSR + (UInt32)bank->VagsInSR);
        }
        for (x = 0; x < bank->NumProgs; x++) {
            bank->FirstProg[x].FirstTone = (TonePtr)((UInt32)bank->FirstProg[x].FirstTone + (UInt32)bank);
        }
        if (gBankListHead != NULL) {
            for (walker = gBankListHead; walker->NextBank != NULL; walker = walker->NextBank)
                ;

            walker->NextBank = bank;
        } else {
            gBankListHead = bank;
        }
    } else {
        /* -0x10(sp) */ SFXBlock2Ptr walker;
        block = (SFXBlock2Ptr)bank;

        block->GrainData = (void *)((UInt32)block->GrainData + (UInt32)block);
        for (x = 0; x < block->NumGrains; x++) {
            if (block->FirstGrain[x].OpcodeData.MicroOp.Type == GRAIN_TYPE_TONE) {
                tp = (TonePtr)((SInt8 *)block->GrainData + (block->FirstGrain[x].OpcodeData.Opcode & 0xFFFFFF));
                tp->VAGInSR = (void *)((UInt32)tp->VAGInSR + (UInt32)block->VagsInSR);
            }
        }

        for (x = 0; x < block->NumSounds; x++) {
            block->FirstSound[x].FirstGrain = (SFXGrain2Ptr)((UInt32)block->FirstSound[x].FirstGrain + (UInt32)block->FirstGrain);
            block->FirstSound[x].Flags &= ~0x4000;
        }

        if ((block->Flags & BLOCK_HAS_NAMES) != 0) {
            block->BlockNames = (SFXBlockNamesPtr)((UInt32)block->BlockNames + (UInt32)bank);
            block->BlockNames->SFXNameTableOffset += (UInt32)block->BlockNames->BlockName;
            block->BlockNames->VAGNameTableOffset += (UInt32)block->BlockNames->BlockName;
            block->BlockNames->VAGImportsTableOffset += (UInt32)block->BlockNames->BlockName;
            block->BlockNames->VAGExportsTableOffset += (UInt32)block->BlockNames->BlockName;
        }

        if ((block->Flags & BLOCK_HAS_USERDATA) != 0) {
            block->SFXUD = (SFXUserDatPtr)((UInt32)block->SFXUD + (UInt32)block);
        }

        if (gBlockListHead != NULL) {
            for (walker = gBlockListHead; walker->NextBlock != NULL; walker = walker->NextBlock)
                ;
            walker->NextBlock = block;
        } else {
            gBlockListHead = block;
        }
    }

    return 0;
}

/* 00013c00 00013dac */ void snd_RemoveBank(/* 0x0(sp) */ SoundBankPtr bank) {
    /* -0x18(sp) */ SoundBankPtr walker = gBankListHead;
    /* -0x14(sp) */ SoundBankPtr prev = NULL;
    /* -0x10(sp) */ SInt32 dis;
    /* -0xc(sp) */ SInt32 oldstat;

    if (bank->DataID == SBLK_ID) {
        snd_RemoveBlock(bank);
        return;
    }

    if (bank == NULL) {
        return;
    }

    while (walker && walker != bank) {
        prev = walker;
        walker = walker->NextBank;
    }

    if (walker == bank) {
        snd_StopAllSoundsInBank(bank, 1);
        if (prev != NULL) {
            prev->NextBank = walker->NextBank;
        } else {
            gBankListHead = walker->NextBank;
        }

        dis = CpuSuspendIntr(&oldstat);

        if (bank->VagDataSize != 0) {
            snd_SRAMFree((UInt32)bank->VagsInSR, bank->VagDataSize);
        }

        if (!dis) {
            CpuResumeIntr(oldstat);
        }
    }
}

/* 00013dac 00013f28 */ void snd_RemoveBlock(/* 0x0(sp) */ SFXBlock2Ptr block) {
    /* -0x18(sp) */ SFXBlock2Ptr walker = gBlockListHead;
    /* -0x14(sp) */ SFXBlock2Ptr prev = NULL;
    /* -0x10(sp) */ SInt32 dis;
    /* -0xc(sp) */ SInt32 oldstat;

    if (block == NULL) {
        return;
    }

    while (walker && walker != block) {
        prev = walker;
        walker = walker->NextBlock;
    }

    if (walker == block) {
        snd_StopAllSoundsInBank(block, 1);
        if (prev != NULL) {
            prev->NextBlock = walker->NextBlock;
        } else {
            gBlockListHead = walker->NextBlock;
        }

        dis = CpuSuspendIntr(&oldstat);

        if (block->SRAMAllocSize != 0) {
            snd_SRAMFree((UInt32)block->VagsInSR, block->SRAMAllocSize);
        }

        if (!dis) {
            CpuResumeIntr(oldstat);
        }
    }
}

/* 00013f28 00013fbc */ void snd_UnloadAllBanks() {
    while (gBankListHead != NULL) {
        snd_UnloadBank(gBankListHead);
    }

    while (gBlockListHead != NULL) {
        snd_UnloadBlock(gBlockListHead);
    }
}

/* 00013fbc 00014218 */ void snd_ResolveBankXREFS() {
    /* -0x18(sp) */ SoundBankPtr bank;
    /* -0x14(sp) */ SFXBlock2Ptr block;
    /* -0x10(sp) */ SInt32 x;
    /* -0xc(sp) */ MIDISoundPtr midisound;

    for (bank = gBankListHead; bank != NULL; bank = bank->NextBank) {
        for (x = 0; x < bank->NumSounds; x++) {
            bank->FirstSound[x].Bank = bank;
        }

        if (bank->FirstSound[x].Type == SND_TYPE_MIDI || bank->FirstSound[x].Type == SND_TYPE_AME) {
            midisound = &bank->FirstSound[x];
            midisound->MIDIBlock = snd_FindMIDIBlock(midisound->MIDIID);
            if (midisound->MIDIBlock == NULL) {
                snd_ShowError(90, midisound->MIDIID, 0, 0, 0);
            }
        }

        bank->Flags |= BFLAG_XREF_RESOLVED;
    }

    for (block = gBlockListHead; block != NULL; block = block->NextBlock) {
        block->Flags |= BFLAG_XREF_RESOLVED;
    }
}

/* 00014218 000142f8 */ SoundBankPtr snd_FindBankByID(/* 0x0(sp) */ UInt32 id) {
    /* -0x10(sp) */ SoundBankPtr bank;
    for (bank = gBankListHead; bank != NULL; bank = bank->NextBank) {
        if (bank->BankID == id) {
            return bank;
        }
    }

    return snd_FindBlockByID(id);
}

/* 000142f8 00014404 */ SFXBlock2Ptr snd_FindBlockByName(/* 0x0(sp) */ UInt32 *name) {
    /* -0x18(sp) */ SFXBlock2Ptr block;
    /* -0x14(sp) */ struct SFXBlockNames *names_header;
    /* -0x10(sp) */ UInt32 *buffer = (UInt32 *)name;

    for (block = gBlockListHead; block != NULL; block = block->NextBlock) {
        if ((block->Flags & BLOCK_HAS_NAMES) != 0) {
            names_header = block->BlockNames;
            if (names_header->BlockName[0] == buffer[0] && names_header->BlockName[1] == buffer[1]) {
                return block;
            }
        }
    }

    return NULL;
}

/* 00014404 000144b0 */ SFXBlock2Ptr snd_FindBlockByID(/* 0x0(sp) */ UInt32 id) {
    /* -0x10(sp) */ SFXBlock2Ptr block;
    for (block = gBlockListHead; block != NULL; block = block->NextBlock) {
        if (block->BlockID == id) {
            return block;
        }
    }

    return NULL;
}

/* 000144b0 0001459c */ SoundBankPtr snd_FindBankByNum(/* -0x10(sp) */ SInt8 num) {
    /* -0xc(sp) */ SoundBankPtr bank;
    for (bank = gBankListHead; bank != NULL; bank = bank->NextBank) {
        if (bank->BankNum == num) {
            return bank;
        }
    }

    return snd_FindBlockByNum(num);
}

/* 0001459c 0001464c */ SFXBlock2Ptr snd_FindBlockByNum(/* -0x10(sp) */ SInt8 num) {
    /* -0xc(sp) */ SFXBlock2Ptr block;

    for (block = gBlockListHead; block != NULL; block = block->NextBlock) {
        if (block->BlockNum == num) {
            return block;
        }
    }

    return NULL;
}

/* 0001464c 00014884 */ SInt32 snd_RegisterMIDI(/* 0x0(sp) */ MIDIBlockHeaderPtr midi) {
    /* -0x18(sp) */ MultiMIDIBlockHeaderPtr mmid;
    /* -0x14(sp) */ MIDIBlockHeaderPtr hold;
    /* -0x10(sp) */ SInt32 x;

    if (midi->DataID == MID_ID) {
        midi->BankPtr = snd_FindBankByID(midi->BankID);
        if (midi->BankPtr == NULL) {
            snd_ShowError(91, midi->BankID, 0, 0, 0);
        }
        midi->DataStart = midi->DataStart + (UInt32)midi;
        midi->NextMIDIBlock = NULL;
        midi->MultiMIDIParent = NULL;
        snd_InsertMIDIBlockInList(midi);
    } else if (midi->DataID == MMID_ID) {
        mmid = midi;
        mmid->NextMIDIBlock = NULL;
        if (mmid->Version < 2) {
            return 0;
        }
        snd_InsertMIDIBlockInList(mmid);
        for (x = 0; x < mmid->NumMIDIBlocks; x++) {
            mmid->BlockPtr[x] += (UInt32)midi;
            hold = mmid->BlockPtr[x];
            snd_RegisterMIDI(hold);
            hold->MultiMIDIParent = mmid;
        }
    }

    return 1;
}

/* 00014884 00014a60 */ void snd_UnregisterMIDI(/* 0x0(sp) */ MIDIBlockHeaderPtr midi) {
    /* -0x18(sp) */ MultiMIDIBlockHeaderPtr mmid;
    /* -0x14(sp) */ MIDIBlockHeaderPtr hold;
    /* -0x10(sp) */ SInt32 x;
    /* -0xc(sp) */ MIDIBlockHeaderPtr walk;
    /* data 1394 */ static SInt32 working_mmd = 0;
    if (!working_mmd) {
        snd_MIDIStreamGone(midi);
    }

    if (midi->DataID == MMID_ID) {
        mmid = midi;
        working_mmd = 1;
        for (x = 0; x < mmid->NumMIDIBlocks; x++) {
            snd_UnregisterMIDI(mmid->BlockPtr[x]);
        }
        working_mmd = 0;
    }

    if (gMIDIListHead == NULL) {
        return;
    }

    if (gMIDIListHead == midi) {
        gMIDIListHead = midi->NextMIDIBlock;
    } else {
        for (walk = gMIDIListHead; walk != NULL; walk = walk->NextMIDIBlock)
            ;
        walk->NextMIDIBlock = midi->NextMIDIBlock;
    }
}

/* 00014a60 00014b00 */ MIDIBlockHeaderPtr snd_FindMIDIBlock(/* 0x0(sp) */ UInt32 id) {
    /* -0x10(sp) */ MIDIBlockHeaderPtr walk;
    for (walk = gMIDIListHead; walk != NULL; walk = walk->NextMIDIBlock) {
        if (walk->ID == id) {
            break;
        }
    }

    return walk;
}

/* 00014b00 00014ba8 */ void snd_InsertMIDIBlockInList(/* 0x0(sp) */ MIDIBlockHeaderPtr midi) {
    /* -0x10(sp) */ MIDIBlockHeaderPtr walk;
    if (gMIDIListHead != NULL) {
        for (walk = gMIDIListHead; walk != NULL; walk = walk->NextMIDIBlock)
            ;
        walk->NextMIDIBlock = midi;
    } else {
        gMIDIListHead = midi;
    }
}

/* 00014ba8 00014d78 */ void snd_MIDIStreamGone(/* 0x0(sp) */ MIDIBlockHeaderPtr midi) {
    /* -0x18(sp) */ SoundBankPtr bank;
    /* -0x14(sp) */ SInt32 x;
    /* -0x10(sp) */ MIDISoundPtr midisound;

    for (bank = gBankListHead; bank != NULL; bank = bank->NextBank) {
        for (x = 0; x < bank->NumSounds; x++) {
            if (bank->FirstSound[x].Type == SND_TYPE_MIDI || bank->FirstSound[x].Type == SND_TYPE_AME) {
                midisound = &bank->FirstSound[x];
                if (midisound->MIDIBlock == midi) {
                    snd_LockMasterTick(512);
                    snd_StopAllHandlersForSound(midisound, 0, 0);
                    snd_UnlockMasterTick();
                    midisound->MIDIBlock = NULL;
                }
            }
        }
    }
}

/* 00014d78 00015014 */ void snd_SetCDSifReg(/* 0x0(sp) */ SInt32 reg8, /* 0x4(sp) */ SInt32 reg9) {
    /* -0x28(sp) */ SInt32 did;
    /* -0x24(sp) */ SInt32 dis;
    /* -0x20(sp) */ SInt32 intr_state;
    /* -0x1c(sp) */ sceSifDmaData transData;
    gStats.cd_busy = reg8;
    gStats.cd_error = reg9;

    if (gEEStatusAddr == 0) {
        return;
    }

    transData.size = 16;
    transData.attr = 0;

    if (gStats.cd_busy) {
        transData.src = &gStats;
        transData.dest = gEEStatusAddr;
        dis = CpuSuspendIntr(&intr_state);
        did = sceSifSetDmaIntr(&transData, 1, snd_EEDMADone, &gEEDMADoneSema);
        if (!dis) {
            CpuResumeIntr(intr_state);
        }

        while (sceSifDmaStat(did) >= 0) {
            WaitSema(gEEDMADoneSema);
        }
    }

    transData.src = &gStats.cd_error;
    transData.dest = gEEStatusAddr + 16;
    dis = CpuSuspendIntr(&intr_state);
    did = sceSifSetDmaIntr(&transData, 1, snd_EEDMADone, &gEEDMADoneSema);
    if (!dis) {
        CpuResumeIntr(intr_state);
    }

    while (sceSifDmaStat(did) >= 0) {
        WaitSema(gEEDMADoneSema);
    }

    if (reg8 == 0) {
        transData.src = &gStats;
        transData.dest = gEEStatusAddr;
        dis = CpuSuspendIntr(&intr_state);
        did = sceSifSetDmaIntr(&transData, 1, snd_EEDMADone, &gEEDMADoneSema);
        if (!dis) {
            CpuResumeIntr(intr_state);
        }

        while (sceSifDmaStat(did) >= 0) {
            WaitSema(gEEDMADoneSema);
        }
    }
}

/* 00015014 000152f8 */ void *snd_IOPMemAlloc(/* 0x0(sp) */ SInt32 size, /* 0x4(sp) */ SInt32 use, /* 0x8(sp) */ SInt32 *act_size) {
    /* -0x20(sp) */ void *mem;
    /* -0x1c(sp) */ SInt32 max_avail;
    /* -0x18(sp) */ SInt32 from_where;
    /* -0x14(sp) */ SInt32 dis;
    /* -0x10(sp) */ SInt32 oldstat;

    dis = CpuSuspendIntr(&oldstat);
    if (use == 8) {
        max_avail = QueryMaxFreeMemSize();
        if (gLimit2Meg != 0 && max_avail > 0x200000) {
            if (!gPrefs_Silent) {
                printf("989snd: detected more then 2 meg free (%d free).\n", max_avail);
            }
            max_avail -= 0x600000;
            if (max_avail < 0) {
                max_avail = 0;
            }
            if (!gPrefs_Silent) {
                printf("        subtracting 6 meg (%d)\n", 0x600000);
                printf("        max avail then is %d\n", max_avail);
            }
            if (!max_avail) {
                max_avail = 10240;
                if (!gPrefs_Silent) {
                    printf("        allowing a 10k buffer!\n");
                }
            }
        }

        if ((max_avail & 0xf) != 0) {
            max_avail = 16 * (max_avail / 16);
        }
        if (max_avail < size) {
            size = max_avail;
        }
        if (size == 0) {
            if (!dis) {
                CpuResumeIntr(oldstat);
            }
            return NULL;
        }
    }

    // TODO simplify, compiler simplified switch case?
    if ((use == 7 || use < 8) && use < 6 && use >= 1) {
        mem = AllocSysMemory(ALLOC_LAST, size, 0);
    } else {
        mem = AllocSysMemory(ALLOC_FIRST, size, 0);
    }

    if (mem != NULL && act_size != NULL) {
        *act_size = size;
    }

    if (!dis) {
        CpuResumeIntr(oldstat);
    }

    return mem;
}

/* 000152f8 00015360 */ void snd_IOPMemFree(/* 0x0(sp) */ void *mem) {
    /* -0x10(sp) */ SInt32 dis;
    /* -0xc(sp) */ SInt32 oldstat;
    dis = CpuSuspendIntr(&oldstat);
    FreeSysMemory(mem);
    if (!dis) {
        CpuResumeIntr(oldstat);
    }
}

/* 00015360 00015478 */ SInt32 snd_GetFreeSPUDMA() {
    /* -0x10(sp) */ SInt32 intr_state;
    /* -0xc(sp) */ SInt32 dis;
    dis = CpuSuspendIntr(&intr_state);
    if (!gDMAInUse[0]) {
        gDMAInUse[0] = 1;
        if (!dis) {
            CpuResumeIntr(intr_state);
        }
        sceSdSetTransIntrHandler(0, NULL, NULL);
        return 0;
    }

    if (!gDMAInUse[1]) {
        gDMAInUse[1] = 1;
        if (!dis) {
            CpuResumeIntr(intr_state);
        }
        sceSdSetTransIntrHandler(1, NULL, NULL);
        return 1;
    }

    if (!dis) {
        CpuResumeIntr(intr_state);
    }

    return -1;
}

/* 00015478 000154e8 */ void snd_FreeSPUDMA(/* 0x0(sp) */ SInt32 ch) {
    gDMAInUse[ch] = 0;
    if (VAGStreamDMAList != NULL) {
        SignalSema(gStartDMASema);
    }
}

/* 000154e8 0001556c */ void snd_WaitDMADone() {
    while (gDMAInUse[0] || gDMAInUse[1]) {
        gWaitingDMAComplete = GetThreadId();
        SleepThread();
    }

    gWaitingDMAComplete = 0;
}
