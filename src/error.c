#include "error.h"
#include "sram.h"
#include "stdio.h"
#include "sysmem.h"
#include "types.h"

// TODO this is its own module
// int start() {
//    if (!sceSifCheckInit())
//        sceSifInit();
//    snd_RegisterErrorDisplayFunc((void *)(__int16)DisplayError);
//    return 0;
//}

void snderr_ShowLoadError(unsigned int a1) {
    printf("    error code -> ");

    switch (a1) {
    case -1:
        printf("SND_LOAD_ERR_SCECdErFAIL\n");
        return;
    case 1:
        printf("SND_LOAD_ERR_SCECdErABRT\n");
        return;
    case 16:
        printf("SND_LOAD_ERR_SCECdErCMD\n");
        return;
    case 17:
        printf("SND_LOAD_ERR_SCECdErOPENS\n");
        return;
    case 18:
        printf("SND_LOAD_ERR_SCECdErNODISC\n");
        return;
    case 19:
        printf("SND_LOAD_ERR_SCECdErNORDY\n");
        return;
    case 20:
        printf("SND_LOAD_ERR_SCECdErCUD\n");
        return;
    case 32:
        printf("SND_LOAD_ERR_SCECdErIPI\n");
        return;
    case 33:
        printf("SND_LOAD_ERR_SCECdErILI\n");
        return;
    case 34:
        printf("SND_LOAD_ERR_SCECdErPRM\n");
        return;
    case 48:
        printf("SND_LOAD_ERR_SCECdErREAD\n");
        return;
    case 49:
        printf("SND_LOAD_ERR_SCECdErTRMOPN\n");
        return;
    case 50:
        printf("SND_LOAD_ERR_SCECdErEOM\n");
        return;
    case 256:
        printf("SND_LOAD_ERR_COUDNT_OPEN_FILE\n");
        return;
    case 257:
        printf("SND_LOAD_ERR_READING_FILE\n");
        return;
    case 258:
        printf("SND_LOAD_ERR_IOP_MEMORY\n");
        return;
    case 259:
        printf("SND_LOAD_ERR_SPU_MEMORY\n");
        return;
    case 260:
        printf("SND_LOAD_ERR_IN_PROGRESS\n");
        return;
    case 261:
        printf("SND_LOAD_ERR_DMA\n");
        return;
    case 262:
        printf("SND_LOAD_ERR_RPC\n");
        return;
    case 263:
        printf("SND_LOAD_ERR_INVALID_FORMAT\n");
        return;
    default:
        printf("UNKNOWN (0x%8.8x)\n", a1);
        return;
    }
}

void snderr_ShowCDError(int a1, int a2, int a3, int a4) {
    switch (a1) {
    case -1:
        printf("\tSCECdErFAIL\n");
        break;
    case 0:
        printf("\tSCECdErNO\n");
        break;
    case 1:
        printf("\tSCECdErABRT\n");
        break;
    case 16:
        printf("\tSCECdErCMD\n");
        break;
    case 17:
        printf("\tSCECdErOPENS\n");
        break;
    case 18:
        printf("\tSCECdErNODISC\n");
        break;
    case 19:
        printf("\tSCECdErNORDY\n");
        break;
    case 20:
        printf("\tSCECdErCUD\n");
        break;
    case 32:
        printf("\tSCECdErIPI\n");
        break;
    case 33:
        printf("\tSCECdErILI\n");
        break;
    case 34:
        printf("\tSCECdErPRM\n");
        break;
    case 48:
        printf("\tSCECdErREAD\n");
        break;
    case 49:
        printf("\tSCECdErTRMOPN\n");
        break;
    case 50:
        printf("\tSCECdErEOM\n");
        break;
    default:
        printf("\tunknown cause (%d)\n", a1);
        break;
    }
    printf("\tRead start sector -> %d\n", a2);
    printf("\tNumber of sectors to read -> %d\n", a3);
    printf("\tIOP destination -> 0x%x\n", a4);
}

void snderr_ShowMemStat() {
    UInt32 maxFree = QueryMaxFreeMemSize();
    UInt32 totalFree = QueryTotalFreeMemSize();
    printf("--------------------------------------------------------------------\n");
    printf("IOP MEM: bytes free         -> 0x%8.8x (%d)\n", totalFree, totalFree);
    printf("         largest free block -> 0x%8.8x (%d)\n", maxFree, maxFree);
    printf("--------------------------------------------------------------------\n");
}

void snderr_DisplayError(SInt32 num, UInt32 arg1, UInt32 arg2, UInt32 arg3, UInt32 arg4) {
    switch (num) {
    case 1:
        printf(
            "snd_RegisterExternProcHandler: Couldn't register extern proc handler... ID 0x%x (%d) exists\n",
            arg1,
            arg2);
        return;
    case 2:
        printf("snd_RegisterExternProcHandler: Couldn't find extern proc handler ID 0x%x (%d)\n", arg1, arg2);
        return;
    case 3:
        printf("snd_DoExternCall: Couldn't find extern proc 0x%x (%d)\n", arg1, arg2);
        return;
    case 4:
        printf("snd_DoExternCall: Attempt to call function out of range in extern proc id 0x%x\n", arg1);
        return;
    case 5:
    case 6:
        printf("989snd: Couldn't allocate memory for RPC communications!\n");
        snderr_ShowMemStat();
        return;
    case 7:
        printf("989snd: Unrecognized command line argument -> %s\n", arg1);
        return;
    case 8:
        printf("snd_DoGrain: encountered a \"Loop End\" grain but couldn't find a previous \"Loop Start\"!\n");
        return;
    case 9:
        printf("snd_DoGrain: encountered a \"Loop Continue\" grain but couldn't find the next \"Loop End\"!\n");
        return;
    case 10:
        printf("snd_DoGrain: Can't have Random Play control grains inside other Random/Cycle Play blocks!\n");
        printf(
            "snd_DoGrain: Block ID = %c%c%c%c, index = %d, grain = %d\n",
            (arg1 >> 0) & 0xFF,
            (arg1 >> 8) & 0xFF,
            (arg1 >> 16) & 0xFF,
            (arg1 >> 24) & 0xFF,
            arg2,
            arg3);
        return;
    case 11:
        printf("snd_DoGrain: encountered a \"Goto Marker\" grain but couldn't find the matching marker!\n");
        return;
    case 12:
        printf("snd_DoGrain: encountered a \"Goto Random Marker\" grain but couldn't find the matching marker for %d!\n", arg1);
        return;
    case 13:
        printf("snd_DoGrain: Can't have Cycle Play control grains inside other Random/Cycle Play blocks!\n");
        return;
    case 14:
    case 17:
        printf("snd_BankLoad: Done... ERROR reading file!\n");
        snderr_ShowLoadError(arg1);
        return;
    case 15:
        printf("snd_BankLoad: Done... ERROR couldn't allocate %d bytes of IOP RAM!\n");
        snderr_ShowMemStat();
        return;
    case 16:
        printf("snd_BankLoad: Done... ERROR couldn't allocate IOP SCRATCH RAM!\n");
        snderr_ShowMemStat();
        return;
    case 18:
        printf("snd_BankLoad: Done... ERROR transfering data to SPU RAM!\n");
        return;
    case 19:
        printf("snd_BankLoadByLoc: Done... ERROR loading imbedded MMD!\n");
        return;
    case 20:
        printf("snd_UnloadBank: Tried to unload a null or 0xFFFFFFFF bank!\n");
        return;
    case 21:
        printf("snd_UnloadBank: Tried to unload a bank that isn't loaded!!!\n");
        return;
    case 22:
        printf("snd_UnloadBlock: Tried to unload a bank that isn't loaded!!!\n");
        return;
    case 23:
        printf("snd_MMDLoadFromFile: Done with Error... couldn't allocate %d bytes of IOP memory.\n");
        snderr_ShowMemStat();
        return;
    case 24:
        printf("snd_BankTransfer: ERROR... Sound System isn't running!\n");
        return;
    case 25:
        printf("snd_BankTransfer: ERROR... There is already a transfer in progress!\n");
        return;
    case 26:
        printf("snd_BankTransfer: ERROR... couldn't allocate %d bytes of sound ram!\n", arg1);
        printf("                  Here is a dump of the current free SRAM blocks...\n");
        snd_SRAMDumpFreeBlocks();
        return;
    case 27:
        printf("snd_BankTransfer: ERROR... couldn't allocate %d bytes at 0x%x in sound ram!\n", arg1, arg2);
        printf("                  There must already be something loaded at that SRAM location.\n");
        printf("                  (This bank was setup to be loaded into a hard coded SRAM address.)\n");
        snd_SRAMDumpFreeBlocks();
        return;
    case 28:
        printf("snd_GetRealMIDIStream: MIDI stream not found!\n");
        return;
    case 29:
        printf("snd_GetRealMIDIStream: Requested MIDI stream does not exist in the MMD!\n");
        return;
    case 30:
        printf("snd_InitMovieSound: buff_size must be divisible by 4!\n");
        return;
    case 31:
        printf("snd_InitMovieSound: Couldn't allocate %d bytes of IOP memory!\n");
        snderr_ShowMemStat();
        return;
    case 32:
        printf("snd_SRAMMalloc: Couldn't allocate %d bytes of sound ram.\n", arg1);
        snd_SRAMDumpFreeBlocks();
        return;
    case 33:
        printf("snd_SRAMFree: Lost free sound RAM!!!! No FREELIST nodes available!\n");
        snd_SRAMDumpFreeBlocks();
        return;
    case 34:
        printf("snd_SRAMMarkUsed: Possible sound ram curruption!!!\n");
        snd_SRAMDumpFreeBlocks();
        return;
    case 35:
        printf("snd_SRAMMarkUsed: requested RAM is in the middle of an allocated block!\n");
        snd_SRAMDumpFreeBlocks();
        return;
    case 36:
        printf("snd_UnlockMasterTick: more unlocks then locks!\n");
        return;
    case 37:
        printf("snd_InitVAGStreaming: Already Inited!\n");
        return;
    case 38:
        printf("snd_InitVAGStreaming: couldn't create DMA thead.\n");
        return;
    case 39:
        printf("snd_InitVAGStreaming: couldn't create Data Load thead.\n");
        return;
    case 40:
        printf("snd_InitVAGStreaming: couldn't create DMA done thead.\n");
        return;
    case 41:
        printf("snd_InitVAGStreaming: couldn't create Data Load done thead.\n");
        return;
    case 42:
        printf("snd_InitVAGStreaming: Couldn't allocate IOP memory for stream handlers!\n");
        printf("                         channels = %d\n", arg1);
        printf("                     handler size = %d\n");
        snderr_ShowMemStat();
        return;
    case 43:
        printf("snd_InitVAGStreaming: Couldn't allocate IOP memory for stream buffers!\n");
        printf("                      buffer_size = %d\n", arg1);
        printf("                         channels = %d\n");
        snderr_ShowMemStat();
        return;
    case 44:
        printf("snd_InitVAGStreaming: Couldn't allocate SPU memory for stream buffers!\n");
        snd_SRAMDumpFreeBlocks();
    case 45:
        printf("snd_InitVAGStreaming: Couldn't allocate IOP memory for stream headers!\n");
        snderr_ShowMemStat();
        return;
    case 46:
        printf("snd_InitVAGStreaming: Couldn't allocate IOP memory for stream safe reads!\n");
        snderr_ShowMemStat();
        return;
    case 47:
        printf("snd_PlayVAGStreamByLoc: VAG Streaming not inited!\n");
        return;
    case 48:
        printf("snd_PlayVAGStreamByLoc: WHAT !?!? There was a stream available but no handler. THIS IS BAD!\n");
        return;
    case 49:
        printf("SCEI strikes again! fudging value for NAX!\n");
        return;
    case 50:
        printf("STREAM.C: Stream stopped... channel out of sync. (stream handle 0x%8.8x\n", arg1);
        return;
    case 51:
        printf("STREAM.C: stream read out of sync with play! Stopping stream handle 0x%8.8x!\n", arg1);
        return;
    case 52:
    case 54:
        printf("snd_CheckVAGStreamProgress: Stream 0x%8.8x stopped... too many errors!\n", arg1);
        return;
    case 53:
        printf("snd_CheckVAGStreamProgress: Not in any buffer (0x%8.8x)!!!! Stopping stream 0x%8.8x\n", arg2, arg1);
        return;
    case 55:
        printf("989SND Stream error: Stream 0x%8.8x relooped! Try increasing buffer size!\n", arg1);
        return;
    case 56:
        printf("snd_VAGStreamDataLoadThread: WaitSema returned something other then KE_OK -> %d\n", arg1);
        return;
    case 57:
        printf("snd_StreamSafeCdReadEE: Streaming not inited! should have been handled by the EE!\n");
        return;
    case 58:
        printf("snd_StreamSafeCdReadEE: VAG Streaming was NOT initialized to allow stream safe CD Reading to the EE!\n");
        return;
    case 59:
        printf("snd_StreamSafeCdReadEEm: Already a data load in progress!\n");
        return;
    case 60:
        printf("snd_StreamSafeCdSync: can't block until CD is finished because there is already a thread waiting!\n");
        return;
    case 61:
        printf("snd_KickDataRead: Error calling sceCdRead()!\n");
        snderr_ShowCDError(arg1, arg2, arg3, arg4);
        return;
    case 62:
        printf("snd_KickVAGRead: Error calling sceCdRead()!\n");
        snderr_ShowCDError(arg1, arg2, arg3, arg4);
        return;
    case 63:
        printf("snd_VAGStreamLoadDoneThread: WaitSema returned something other then KE_OK -> %d\n", arg1);
        return;
    case 64:
        printf("snd_VAGStreamLoadDoneThread: Error during VAG stream Read!\n");
        snderr_ShowCDError(arg1, arg2, arg3, arg4);
        return;
    case 65:
        printf("snd_VAGStreamLoadDoneThread: Error during Data Read!\n");
        snderr_ShowCDError(arg1, arg2, arg3, arg4);
        return;
    case 66:
        printf("snd_VAGStreamLoadDoneThread: Error DMAing data to EE memory!!!\n");
        return;
    case 67:
        printf("snd_VAGStreamLoadDoneThread: Interleaved stream's block sizes don't match the current buffer sizes!\n");
        printf("snd_VAGStreamLoadDoneThread: Reinterleave the stream with a block size of %d.\n", arg1);
        printf("snd_VAGStreamLoadDoneThread: Or initialize the stream lib with a buffer size of %d.\n", arg2);
        return;
    case 68:
        printf("snd_VAGStreamLoadDoneThread: Not enough sequential stream buffers to play interleaved stream!\n");
        return;
    case 69:
        printf("STREAM.C: Invalid VAG at sector %d offset %d! Bad Sector? Wrong CD location?\n", arg1, arg2);
        return;
    case 70:
    case 71:
        printf("snd_VAGStreamDMAThread: WaitSema returned something other then KE_OK -> %d\n", arg1);
        return;
    case 72:
        printf("STREAM.C: tried to DMA buffer that isn't ready (stream buffers too small?)!\n");
        return;
    case 73:
        printf("STREAM.C: Tried to add a buffer to the load list and it was already there (%d)!\n", arg1);
        return;
    case 74:
        printf("snd_StreamSafeCheckCDIdle: Streaming not inited!\n");
        return;
    case 75:
    case 76:
        printf("snd_AddStreamToHandler: Stream already in handler!\n");
        return;
    case 77:
        printf("snd_SetVAGStreamVolPan: Holy crap! No stream attached to sound handler!!!\n");
        return;
    case 78:
        printf("snd_CollectTones: can't because program is out of range!\n");
        return;
    case 79:
        printf("snd_LockVoiceAllocator: Caller asked me to block but interrupts are disabled!\n");
        return;
    case 80:
        printf("snd_LockVoiceAllocator: WaitSema returned something other then KE_OK -> %d\n", arg1);
        return;
    case 81:
        printf("snd_LockVoiceAllocator: Couldn't signal semaphore!\n");
        return;
    case 82:
        printf("snd_MarkVoicePlaying: voice already on play list (%d)!!\n", arg1);
        return;
    case 83:
        printf("snd_SetMasterVolume: Setting the master volume of the \"EXTERNAL\" group has no effect!\n");
        return;
    case 84:
        printf("snd_BankLoad: Error... invalid file format found at supplied location!\n");
        printf("              sector -> 0x%8.8x\n", arg2);
        printf("         sector buff -> 0x%8.8x\n", arg3);
        printf("              got    -> 0x%8.8x\n", *(UInt8 *)arg1);
        printf("              got    -> 0x%8.8x\n", *(UInt8 *)(arg1 + 4));
        printf("              got    -> 0x%8.8x\n", *(UInt8 *)(arg1 + 8));
        printf("              got    -> 0x%8.8x\n", *(UInt8 *)(arg1 + 12));
        printf("              got    -> 0x%8.8x\n", *(UInt8 *)(arg1 + 16));
        printf("              got    -> 0x%8.8x\n", *(UInt8 *)(arg1 + 20));
        return;
    case 85:
        printf("snd_PreAllocReverbWorkArea: Error... nont enough SRAM for specified reverb type!\n");
        snd_SRAMDumpFreeBlocks();
    case 86:
        printf("snd_SetReverbType_L: Error... nont enough SRAM for specified reverb type!\n");
        snd_SRAMDumpFreeBlocks();
    case 87:
        printf("snd_SetReverbType_L: Error... requested reverb type is bigger then SRAM pre-allocation!\n");
        snd_SRAMDumpFreeBlocks();
        return;
    case 88:
        printf("snd_SRAMMallocRev: There IS enough free memory for the reverb buffer...\n");
        printf("                   but it isn't aligned correctly for the SPU. You may\n");
        printf("                   be looking at a SPU memory fragmentation issue. Here\n");
        printf("                   is a dump of the free memory blocks...\n");
        snd_SRAMDumpFreeBlocks();
        printf("                   The requested reverb needs %d bytes with its END\n", arg1);
        printf("                   aligend on a 128k boundary.\n");
        return;
    case 89:
        printf("snd_PlayAMESound: Error... Tried to play MIDI sound that never did find it's MIDI data\n");
        return;
    case 90:
        printf("snd_ResolveBankXREFS: Warning... MIDI sound couldn't find its midi data\n");
        printf("                      MIDI sound wanted MIDI data tagged: %c%c%c%c\n",
               (arg1 >> 0) & 0xFF,
               (arg1 >> 8) & 0xFF,
               (arg1 >> 16) & 0xFF);
        return;
    case 91:
        printf("snd_RegisterMIDI: Warning... MIDI data couldn't find its bank of sounds.\n");
        printf("                  MIDI data wanted bank tagged: %c%c%c%c\n",
               (arg1 >> 0) & 0xFF,
               (arg1 >> 8) & 0xFF,
               (arg1 >> 16) & 0xFF);
        return;
    case 92:
        printf("snd_SetMIDIRegister: register %d out of range. Must be between 0 and 15.\n", arg1);
        return;
    case 93:
        printf("snd_SRAMFree: Error... tried to free 0 bytes from SRAM location 0x%8.8x\n", arg1);
        return;
    case 94:
        printf("snd_StreamSafeCdRead (from EE): asked to read 0 sectors!\n");
        return;
    case 95:
        printf("snd_DoGrain: Encountered unknown grain type. Update your sound libs!\n");
        return;
    case 96:
        printf("989snd... MIDI: Unrecognized meta event (0x%x). Stream will stop.\n", arg1);
        return;
    case 97:
        printf("989snd... MIDI: Unrecognized sysex event.\n");
        return;
    case 98:
        printf("989snd... MIDI: Unrecognized MIDI event (0x%x). Stream will stop.\n", arg1);
        return;
    case 99:
        printf("snd_GetSoundUserData: error DMAing user data to the EE.\n");
        return;
    case 100:
        printf("989snd: Attemp to use a handle with value 0xFFFFFFFF!\n");
        return;
    case 101:
        printf("989snd: Invalid sound handle passed (0x%8.8x)!\n", arg1);
        return;
    case 102:
        printf("989snd: VERY ODD! There was an attempt to read %d sectors into the sector buffer!\n", arg1);
        return;
    case 103:
        printf("snd_InitMovieSoundEx: couldn't allocate %d bytes of SPU memory for ADPCM buffer\n", arg1);
        return;
    case 104:
        printf("snd_ProcessMIDITick: Bank change message but bank not found! (%d)\n", arg1);
        return;
    case 105:
        printf("989snd: Override bank size (%d) is smaller than actual bank data (%d)! The bank may not load correctly!\n",
               arg1,
               arg2);
        return;
    case 106:
        printf("989snd: Warning... PVS_FLAG_DOUBLE_VOICE_ON_MONO_STREAM can't be used when playing an interleaved stream.\n");
        return;
    case 107:
        printf("989snd: Error reading streaming VAG data from stdio (read returned %d for file handle %d)\n", arg1, arg2);
        return;
    case 108:
        printf("989snd: Error... snd_PlayVAGStreamByName not available in release version!\n");
        return;
    case 109:
        printf("snd_InitVAGStreaming: couldn't create Deferred File Close thead.\n");
        return;
    case 110:
        printf("989snd: Error trying to DMA VAG stream data from EE\n");
        return;
    case 111:
        printf("989snd: Couldn't add file handle to deferred close list! File will remain open!\n");
        return;
    case 112:
        printf("989snd: Call to snd_InitMovieSoundEx when already initialized!\n");
        return;
    case 113:
        printf("snd_BankLoad: Done... ERROR OLD bank file format!\n");
        printf("              Try the bnkupdte.exe command line tool, or re-export\n");
        printf("              from the latest SCEASoundTool\n");
        return;
    case 114:
        printf("snd_PlayVAGStreamByLocEx: Error DMAing extra data from EE to IOP.\n");
        return;
    case 115:
        printf("989snd: ERROR! snd_DeactivateHandler called with a handler not on master active list!\n");
        return;
    case 116:
        printf("989snd: ERROR! snd_DeactivateHandler error removing handler from sibling chain!\n");
        return;
    case 117:
        printf("989 PROBLEM! Child list error 1! Sound may be left playing!\n");
        return;
    case 118:
        printf("989 PROBLEM! Child list error 2! Would have attached sound handle to a parent twice!\n");
        return;
    case 119:
        printf("989snd: ERROR! head of ActiveSoundList is a child!\n");
        return;
    case 120:
        printf("989snd ERROR! Deactivating master handler and not killing children!\n");
        printf("              Children will be killed!\n");
        return;
    case 121:
        printf("989snd: Bad MIDI file Version number!\n");
        return;
    default:
        printf("989err: Unrecognized error code... %d -> %d, %d, %d, %d\n", num, arg1, arg2, arg3, arg4);
        printf("        (is this an old version of 989err.irx?)\n");
        return;
    }
}
