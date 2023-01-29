#include "types.h"
#include "util.h"

/* data 1cbc */ StreamChannelRange gStreamChannelRange[16];
/* data 1cfc */ struct VAGStreamHandler *gVAGStreamHandler;
/* data 1d00 */ SInt32 gNumVAGStreams;
/* data 1d04 */ SInt32 gVAGStreamingInited;
/* data 1d08 */ UInt32 gCdSpinMode;
/* data 1d0c */ SInt32 gHalfBufferSize;
/* data 1d10 */ SInt32 gMasterReadBufferSize;
/* data 1d14 */ SInt32 gMasterReadBufferFirstSector;
/* data 1d18 */ SInt32 gMasterReadBufferLastSector;
/* data 1d1c */ void *gMasterReadBuffer;
/* data 1d20 */ void *gCurrentReadBuffer;
/* data 1d24 */ UInt32 gSSNeedSectors;
/* data 1d28 */ UInt32 gSSReadSectors;
/* data 1d2c */ UInt32 gSSReadLoc;
/* data 1d30 */ UInt32 gBreakDataRead;
/* data 1d34 */ SInt32 gLastVAGReadError;
/* data 1d38 */ SInt32 gLastDATAReadError;
/* data 1d3c */ SInt32 gReadWaitThread;
/* data 1d40 */ SInt32 gReadIsEE;
/* data 1d44 */ UInt32 gLastReadSector;
/* data 1d48 */ UInt32 gLastReadAmount;
/* data 1d4c */ UInt32 gLastReadIOPLoc;
/* data 1d50 */ struct VAGStreamHeader *VAGStreamList;
/* data 1d54 */ struct VAGBuffer *VAGStreamLoadList;
/* data 1d58 */ struct VAGBuffer *VAGStreamLoadListTail;
/* data 1d5c */ struct VAGBuffer *VAGStreamDMAList;
/* data 1d60 */ struct VAGBuffer *VAGStreamDMAListTail;
/* data 1d64 */ struct VAGBuffer *gVAGReadBusy;
/* data 1d68 */ SInt32 gDataReadBusy;
/* data 1d6c */ static struct VAGBuffer *gDMABusy[2];
/* data 1d74 */ static SInt32 gCDIdleWaiter;
/* data 1d78 */ static SInt32 gStartDataLoadSema;
/* data 1d7c */ SInt32 gStartDMASema;
/* data 1d80 */ char *gSearchFileName;
/* data 1d84 */ struct sceCdlFILE *gSearchFileParams;
/* data 1d88 */ SInt32 gSearchFileSema;
/* data 1d8c */ SInt32 gSearchFileResult;
/* data 1d90 */ SInt32 gDMAThread;
/* data 1d94 */ SInt32 gDataLoadThread;
/* data 1d98 */ SInt32 gDMADoneThread;
/* data 1d9c */ SInt32 gLoadDoneThread;
/* data 1da0 */ SInt32 gDeferredFileCloseThread;
/* data 1da4 */ SInt32 gDeferredFileCloseSema;
/* data 1da8 */ SInt32 gDeferredCloseHandles[16];
/* data 1de8 */ SInt32 gNumStreamsPlaying;
/* data 1dec */ SInt32 gShuttingDownStreaming;
/* bss 8dd0 */ struct VAGStreamQueEntry gVAGStreamQue[64];
/* bss 8d90 */ struct EEVagStreamMonitor vsm;

/* 00022264 00023004 */ bool snd_InitVAGStreamingEx(/* 0x0(sp) */ SInt32 num_channels, /* 0x4(sp) */ SInt32 buffer_size, /* 0x8(sp) */ UInt32 read_mode, /* 0xc(sp) */ bool enable_streamsafe_from_ee) {
    /* -0x50(sp) */ SInt32 x;
    /* -0x4c(sp) */ SInt32 y;
    /* -0x48(sp) */ struct VAGStreamHeader *header;
    /* -0x44(sp) */ VAGStreamHeaderPtr *list;
    /* -0x40(sp) */ ThreadParam thread;
    /* -0x2c(sp) */ SemaParam sema;
    /* -0x1c(sp) */ SInt8 *iop_buffer;
    /* -0x18(sp) */ UInt32 spu_buffer;
    /* -0x14(sp) */ SInt32 intr_state;
    /* -0x10(sp) */ SInt32 dis;
    UNIMPLEMENTED();
}

/* 00023004 0002309c */ void snd_SetVAGStreamChannelRange(/* 0x0(sp) */ SInt32 vol_group, /* 0x4(sp) */ SInt32 ch_min, /* 0x8(sp) */ SInt32 ch_max) {
    UNIMPLEMENTED();
}
/* 0002309c 000233d4 */ SInt32 snd_AllocateStreamResources(/* 0x0(sp) */ struct VAGStreamHeader **stream1, /* 0x4(sp) */ struct VAGStreamHeader **stream2, /* 0x8(sp) */ SInt32 stereo, /* 0xc(sp) */ SInt32 get_voices, /* 0x10(sp) */ SInt32 vol_group) {
    UNIMPLEMENTED();
}
/* 000233d4 00023578 */ UInt32 snd_PlayVAGStreamByNameEx(/* 0x0(sp) */ char *name1, /* 0x4(sp) */ char *name2, /* 0x8(sp) */ UInt32 offset1, /* 0xc(sp) */ UInt32 offset2, /* 0x10(sp) */ SInt32 vol, /* 0x14(sp) */ SInt32 pan, /* 0x18(sp) */ SInt32 vol_group, /* 0x1c(sp) */ UInt32 queue, /* 0x20(sp) */ SInt32 sub_group, /* 0x24(sp) */ UInt32 flags) {
    /* -0x10(sp) */ SInt32 fh1;
    /* -0xc(sp) */ SInt32 fh2;
    UNIMPLEMENTED();
}

/* 00023578 00024618 */ UInt32 snd_PlayVAGStreamByLocEx(/* 0x0(sp) */ SInt32 loc1, /* 0x4(sp) */ SInt32 loc2, /* 0x8(sp) */ SInt32 offset1, /* 0xc(sp) */ SInt32 offset2, /* 0x10(sp) */ SInt32 vol, /* 0x14(sp) */ SInt32 pan, /* 0x18(sp) */ SInt32 vol_group, /* 0x1c(sp) */ UInt32 queue, /* 0x20(sp) */ SInt32 sub_group, /* 0x24(sp) */ UInt32 flags) {
    /* -0x28(sp) */ VAGStreamHandlerPtr handler;
    /* -0x24(sp) */ struct VAGStreamHeader *thestream;
    /* -0x20(sp) */ struct VAGStreamHeader *thestreamR;
    /* -0x1c(sp) */ SInt32 core;
    /* -0x18(sp) */ SInt32 c_v;
    /* -0x14(sp) */ struct VAGStreamQueEntry *slot1;
    /* -0x10(sp) */ struct VAGStreamQueEntry *slot2;
    /* -0xc(sp) */ SInt32 x;
    UNIMPLEMENTED();
}

/* 00024618 0002478c */ SInt32 snd_GetVAGStreamQueueCount(/* 0x0(sp) */ UInt32 handle) {
    /* -0x18(sp) */ SInt32 count;
    /* -0x14(sp) */ VAGStreamHandlerPtr hand;
    /* -0x10(sp) */ struct VAGStreamQueEntry *walk;
    /* -0xc(sp) */ struct VAGStreamHeader *stream;
    UNIMPLEMENTED();
}

/* 0002478c 00024c28 */ void snd_TryPreBufferQueElement(/* 0x0(sp) */ VAGStreamHandlerPtr handler, /* 0x4(sp) */ UInt32 load_flags) {
    /* -0x20(sp) */ struct VAGStreamHeader *currstream;
    /* -0x1c(sp) */ struct VAGStreamHeader *thestream;
    /* -0x18(sp) */ struct VAGStreamHeader *thestreamR;
    /* -0x14(sp) */ SInt32 stereo;
    UNIMPLEMENTED();
}

/* 00024c28 000250b0 */ void snd_SetupVAGStream(/* 0x0(sp) */ VAGStreamHandlerPtr handler, /* 0x4(sp) */ struct VAGStreamHeader *stream, /* 0x8(sp) */ SInt32 loc, /* 0xc(sp) */ SInt32 offset, /* 0x10(sp) */ SInt32 part_of_queue, /* 0x14(sp) */ UInt32 flags) {
    /* -0x68(sp) */ struct EEVagStreamMonitor vsm;
    /* -0x28(sp) */ struct IOPVagStreamMonitor *vsmp;
    /* -0x24(sp) */ sceSifReceiveData data_track;
    UNIMPLEMENTED();
}

/* 000250b0 0002547c */ void snd_StopAllStreams() {
    /* -0x18(sp) */ SInt32 x;
    /* -0x14(sp) */ SInt32 c;
    /* -0x10(sp) */ SInt32 cc;
    /* -0xc(sp) */ bool done;
    UNIMPLEMENTED();
}

/* 0002547c 0002581c */ void snd_PauseVAGStream(/* 0x0(sp) */ UInt32 handle) {
    /* -0x28(sp) */ SInt32 core;
    /* -0x24(sp) */ SInt32 voice;
    /* -0x20(sp) */ struct VAGStreamHeader *stream;
    /* -0x1c(sp) */ VAGStreamHandlerPtr hand;
    /* -0x18(sp) */ SInt32 intr_state;
    /* -0x14(sp) */ SInt32 dis;
    /* -0x10(sp) */ SInt32 x;
    UNIMPLEMENTED();
}

/* 0002581c 00025904 */ void snd_PauseAllStreams() {
    /* -0x10(sp) */ SInt32 x;
    UNIMPLEMENTED();
}

/* 00025904 00026058 */ void snd_ContinueVAGStream(/* 0x0(sp) */ UInt32 handle) {
    /* -0x28(sp) */ SInt32 core;
    /* -0x24(sp) */ SInt32 voice;
    /* -0x20(sp) */ struct VAGStreamHeader *stream;
    /* -0x1c(sp) */ VAGStreamHandlerPtr hand;
    /* -0x18(sp) */ SInt32 state;
    /* -0x14(sp) */ SInt32 dis;
    /* -0x10(sp) */ SInt32 x;
    UNIMPLEMENTED();
}

/* 00026058 000260f4 */ void snd_StopVAGStream(/* 0x0(sp) */ UInt32 handle) {
    /* -0x10(sp) */ VAGStreamHandlerPtr hand;
    UNIMPLEMENTED();
}

/* 000260f4 000261c0 */ bool snd_StreamHandlerIsBusyWithLoad(/* 0x0(sp) */ VAGStreamHandlerPtr hand) {
    /* -0x10(sp) */ struct VAGStreamHeader *stream;
    UNIMPLEMENTED();
}

/* 000261c0 000262cc */ bool snd_StreamHandlerIsBusyWithDMA(/* 0x0(sp) */ VAGStreamHandlerPtr hand) {
    /* -0x10(sp) */ struct VAGStreamHeader *stream;
    UNIMPLEMENTED();
}

/* 000262cc 00026aac */ void snd_KillVAGStreamEx(/* 0x0(sp) */ UInt32 handle, /* 0x4(sp) */ SInt32 from_where) {
    /* -0x28(sp) */ SInt32 core;
    /* -0x24(sp) */ SInt32 voice;
    /* -0x20(sp) */ struct VAGStreamHeader *master_stream;
    /* -0x1c(sp) */ struct VAGStreamHeader *stream;
    /* -0x18(sp) */ VAGStreamHandlerPtr hand;
    /* -0x14(sp) */ SInt32 dis;
    /* -0x10(sp) */ SInt32 intr_state;
    /* data 1df0 */ static int in;
    UNIMPLEMENTED();
}

/* 00026aac 000274e8 */ void snd_ProcessVAGStreamTick(/* 0x0(sp) */ VAGStreamHandlerPtr hand) {
    /* -0x100(sp) */ SInt32 dma_needed;
    /* -0xfc(sp) */ struct VAGStreamHeader *stream;
    /* -0xf8(sp) */ struct VAGStreamHeader *walk;
    /* -0xf4(sp) */ SInt32 done;
    /* -0xf0(sp) */ SInt32 voice[48];
    /* -0x30(sp) */ SInt32 voice_count;
    /* -0x2c(sp) */ SInt32 start_with_voices;
    /* -0x28(sp) */ SInt32 sync_error;
    /* -0x24(sp) */ UInt32 load_flags;
    /* -0x20(sp) */ SInt32 dis;
    /* -0x1c(sp) */ SInt32 intr_state;
    /* -0x18(sp) */ SInt32 read_sectors;
    /* -0x14(sp) */ SInt32 played_sectors;
    /* -0x10(sp) */ UInt32 diff;
    /* -0xc(sp) */ UInt32 hold;
    /* -0xc(sp) */ bool part_of_interleave;
    UNIMPLEMENTED();
}

/* 000274e8 00027f94 */ SInt32 snd_CheckVAGStreamProgress(/* 0x0(sp) */ struct VAGStreamHeader *stream, /* 0x4(sp) */ SInt32 *dma_needed) {
    /* -0x38(sp) */ SInt32 core;
    /* -0x34(sp) */ SInt32 voice;
    /* -0x30(sp) */ UInt32 addr;
    /* -0x2c(sp) */ UInt32 addr2;
    /* -0x28(sp) */ UInt32 addr3;
    /* -0x24(sp) */ UInt32 ends;
    /* -0x20(sp) */ SInt32 in_buffer;
    /* -0x1c(sp) */ SInt32 done;
    /* -0x18(sp) */ SInt32 local_dma_needed;
    /* -0x14(sp) */ SInt32 state;
    /* -0x10(sp) */ SInt32 dis;
    UNIMPLEMENTED();
}

/* 00027f94 000281c8 */ void snd_FixVAGStreamSamplesPlayed(/* 0x0(sp) */ struct VAGStreamHeader *stream, /* 0x4(sp) */ UInt32 addr) {
    /* -0x10(sp) */ SInt32 last_buffer;
    UNIMPLEMENTED();
}

/* 000281c8 000283b4 */ UInt32 snd_GetVAGStreamLoc(/* 0x0(sp) */ UInt32 handle) {
    /* -0x18(sp) */ UInt32 loc;
    /* -0x14(sp) */ UInt32 seconds;
    /* -0x10(sp) */ struct VAGStreamHeader *stream;
    /* -0xc(sp) */ VAGStreamHandlerPtr hand;
    UNIMPLEMENTED();
}

/* 000283b4 000284a4 */ UInt32 snd_GetVAGStreamTimeRemaining(/* 0x0(sp) */ UInt32 handle) {
    /* -0x18(sp) */ struct VAGStreamHeader *stream;
    /* -0x14(sp) */ VAGStreamHandlerPtr hand;
    /* -0x10(sp) */ UInt32 current_pos;
    /* -0xc(sp) */ UInt32 total_time;
    UNIMPLEMENTED();
}

/* 000284a4 00028624 */ UInt32 snd_GetVAGStreamTotalTime(/* 0x0(sp) */ UInt32 handle) {
    /* -0x18(sp) */ struct VAGStreamHeader *stream;
    /* -0x14(sp) */ VAGStreamHandlerPtr hand;
    /* -0x10(sp) */ UInt32 seconds;
    /* -0xc(sp) */ UInt32 samples;
    UNIMPLEMENTED();
}

/* 00028624 000286b0 */ SInt32 snd_IsVAGStreamBuffered(/* 0x0(sp) */ UInt32 handle) {
    /* -0x10(sp) */ VAGStreamHandlerPtr hand;
    UNIMPLEMENTED();
}

/* 000286b0 00028af8 */ void snd_VAGStreamDataLoadThread() {
    /* -0x18(sp) */ SInt32 intr_state;
    /* -0x14(sp) */ SInt32 dis;
    /* -0x10(sp) */ SInt32 ws_ret;
    UNIMPLEMENTED();
}

/* 00028af8 00028d20 */ SInt32 snd_StreamSafeCdRead(/* 0x0(sp) */ UInt32 lbn, /* 0x4(sp) */ UInt32 sectors, /* 0x8(sp) */ void *buf) {
    /* -0x18(sp) */ SInt32 intr_state;
    /* -0x14(sp) */ SInt32 dis;
    /* -0x10(sp) */ sceCdRMode mode;
    /* data 1df4 */ static SInt32 in_func;
    UNIMPLEMENTED();
}

/* 00028d20 00028dac */ SInt32 snd_StreamSafeCdBreak() {}
/* 00028dac 00028f9c */ SInt32 snd_StreamSafeCdSearchFile(/* 0x0(sp) */ sceCdlFILE *fp, /* 0x4(sp) */ char *name) {
    /* -0x10(sp) */ SInt32 dis;
    /* -0xc(sp) */ SInt32 intr_state;
    UNIMPLEMENTED();
}

/* 00028f9c 000292cc */ SInt32 snd_StreamSafeCdReadEEm(/* 0x0(sp) */ UInt32 lbn, /* 0x4(sp) */ UInt32 sectors, /* 0x8(sp) */ void *buf) {
    /* -0x10(sp) */ SInt32 intr_state;
    /* -0xc(sp) */ SInt32 dis;
    /* data 1df8 */ static SInt32 in_func;
    UNIMPLEMENTED();
}

/* 000292cc 0002945c */ SInt32 snd_StreamSafeCdSync(/* 0x0(sp) */ SInt32 mode) {}
/* 0002945c 000294c8 */ SInt32 snd_StreamSafeCdGetError() {}
/* 000294c8 000298d8 */ void snd_KickDataRead() {
    /* -0x18(sp) */ sceCdRMode mode;
    /* -0x14(sp) */ SInt32 ret_code;
    /* -0x10(sp) */ SInt32 th_id;
    {
        /* -0x10(sp) */ SInt32 th_id;
    }
    UNIMPLEMENTED();
}

/* 000298d8 00029d84 */ void snd_KickVAGRead(/* 0x0(sp) */ SInt32 offset) {
    /* -0x10(sp) */ SInt32 readbytes;
    /* -0xc(sp) */ UInt32 readsectors;
    UNIMPLEMENTED();
}

/* 00029d84 0002a4ac */ SInt32 snd_GetStreamDataFromIOPMemory(/* 0x0(sp) */ SInt32 offset, /* 0x4(sp) */ SInt32 readbytes, /* 0x8(sp) */ UInt32 readsectors) {
    /* -0x28(sp) */ SInt32 ret_code;
    /* -0x24(sp) */ char *src;
    /* -0x24(sp) */ int ii;
    /* -0x20(sp) */ int back;
    /* -0x20(sp) */ struct VAGStreamHeader *walk;
    /* -0x24(sp) */ struct VAGStreamHeader *master;
    /* -0x1c(sp) */ int i;
    {
        /* -0x18(sp) */ int ii;
        /* -0x14(sp) */ int back;
        /* -0x10(sp) */ u_int c;
    }
    UNIMPLEMENTED();
}

/* 0002a4ac 0002a704 */ SInt32 snd_GetStreamDataFromEEMemory(/* 0x0(sp) */ SInt32 offset, /* 0x4(sp) */ SInt32 readbytes, /* 0x8(sp) */ UInt32 readsectors) {
    /* -0x30(sp) */ sceSifReceiveData data_track;
    /* -0x14(sp) */ char *ee_src;
    /* -0x10(sp) */ SInt32 ret_code;
    UNIMPLEMENTED();
}

/* 0002a704 0002ae1c */ SInt32 snd_GetStreamDataFromMemoryStream(/* 0x0(sp) */ SInt32 offset, /* 0x4(sp) */ SInt32 readbytes, /* 0x8(sp) */ UInt32 readsectors, /* 0xc(sp) */ UInt32 flags) {
    /* -0x60(sp) */ sceSifReceiveData data_track;
    /* -0x44(sp) */ SInt32 ret_code;
    /* -0x40(sp) */ SInt32 did;
    /* -0x3c(sp) */ SInt32 dis;
    /* -0x38(sp) */ SInt32 intr_state;
    /* -0x34(sp) */ UInt32 data_needed;
    /* -0x30(sp) */ UInt32 read_bytes;
    /* -0x2c(sp) */ sceSifDmaData transData;
    /* -0x1c(sp) */ char *iop_loc;
    /* -0x18(sp) */ char *read_loc;
    /* -0x14(sp) */ UInt32 bytes_consumed;
    /* -0x10(sp) */ struct EEVagStreamMonitor *vsmp;
    {
        /* -0xc(sp) */ struct IOPVagStreamMonitor *vsmp;
    }
    UNIMPLEMENTED();
}

/* 0002ae1c 0002b0f4 */ SInt32 snd_GetStreamDataFromStdio(/* 0x0(sp) */ SInt32 offset, /* 0x4(sp) */ SInt32 readbytes, /* 0x8(sp) */ UInt32 readsectors) {
    /* -0x10(sp) */ SInt32 ret_code;
    /* -0xc(sp) */ SInt32 stdio_error;
    UNIMPLEMENTED();
}

/* 0002b0f4 0002b350 */ SInt32 snd_GetStreamDataFromCD(/* 0x0(sp) */ SInt32 offset, /* 0x4(sp) */ SInt32 readbytes, /* 0x8(sp) */ UInt32 readsectors) {
    /* -0x10(sp) */ SInt32 ret_code;
    /* -0xc(sp) */ sceCdRMode mode;
    UNIMPLEMENTED();
}

/* 0002b350 0002d704 */ void snd_VAGStreamLoadDoneThread() {
    /* -0x68(sp) */ SInt32 do_dma;
    /* -0x64(sp) */ SInt32 intr_state;
    /* -0x60(sp) */ bool dead;
    /* -0x5c(sp) */ SInt32 i;
    /* -0x58(sp) */ struct VAGStreamHeader *walk;
    /* -0x54(sp) */ SInt32 dis;
    /* -0x50(sp) */ VAGStreamHandlerPtr hand;
    /* -0x4c(sp) */ struct VAGStreamHeader *stream;
    /* -0x48(sp) */ SInt32 ws_ret;
    /* -0x44(sp) */ SInt32 did;
    /* -0x40(sp) */ sceSifDmaData transData;
    /* -0x44(sp) */ UInt32 sr;
    /* -0x30(sp) */ UInt32 size;
    /* -0x2c(sp) */ SCEIVAGHeader *vfh;
    /* -0x28(sp) */ struct VAGStreamHeader *new_master;
    /* -0x24(sp) */ VPKFileHeadPtr vpk;
    /* -0x20(sp) */ UInt32 original_stream_flags;
    /* -0x1c(sp) */ bool queued_stream;
    /* -0x18(sp) */ bool free_orig_stream;
    /* -0x14(sp) */ struct VAGStreamHeader *list_hold;
    /* -0x10(sp) */ UInt32 overage;
    /* -0x10(sp) */ SInt32 core;
    /* -0xc(sp) */ SInt32 c_v;
    /* -0xc(sp) */ char *start_addr;
    /* -0x10(sp) */ SInt32 space_left;
    {
        /* -0x14(sp) */ UInt32 overage;
        /* -0x14(sp) */ struct VAGBuffer *next_buff;
    }
    UNIMPLEMENTED();
}

/* 0002d704 0002dbdc */ void snd_RestartInterleavedStream(/* 0x0(sp) */ struct VAGStreamHandler *handler) {
    /* -0x18(sp) */ struct VAGStreamHeader *walk;
    /* -0x14(sp) */ SInt32 i;
    /* -0x10(sp) */ UInt32 overage;
    UNIMPLEMENTED();
}

/* 0002dbdc 0002dc98 */ void snd_SignalDataReadDone() {
    /* -0x10(sp) */ SInt32 th_id;
    UNIMPLEMENTED();
}

/* 0002dc98 0002e110 */ void snd_VAGStreamDMAThread() {
    /* -0x18(sp) */ SInt32 intr_state;
    /* -0x14(sp) */ SInt32 dis;
    /* -0x10(sp) */ SInt32 ws_ret;
    UNIMPLEMENTED();
}

/* 0002e110 0002e2f4 */ void snd_KickVAGDMA(/* 0x0(sp) */ SInt32 ch) {
    /* -0x10(sp) */ SInt32 size;
    UNIMPLEMENTED();
}

/* 0002e2f4 0002e414 */ void snd_VAGStreamDMADoneThread() {
    /* -0x10(sp) */ SInt32 ws_ret;
    UNIMPLEMENTED();
}

/* 0002e414 0002e87c */ void snd_RepairIOPMemoryBuffer(/* 0x0(sp) */ struct VAGBuffer *buff) {
    /* -0x20(sp) */ SInt32 back;
    /* -0x1c(sp) */ SInt32 ii;
    /* -0x18(sp) */ struct VAGStreamHeader *walk;
    /* -0x14(sp) */ struct VAGStreamHeader *master;
    /* -0x10(sp) */ int i;
    /* -0xc(sp) */ u_int c;
    UNIMPLEMENTED();
}

/* 0002e87c 0002f418 */ void snd_VAGDMADone(/* 0x0(sp) */ SInt32 ch) {
    /* -0x38(sp) */ SInt32 start;
    /* -0x34(sp) */ SInt32 start_chain;
    /* -0x30(sp) */ SInt32 done;
    /* -0x2c(sp) */ SInt32 nothing;
    /* -0x28(sp) */ SInt32 start_after_next_load;
    /* -0x24(sp) */ VAGStreamHeaderPtr walk;
    /* -0x20(sp) */ SInt32 which;
    /* -0x1c(sp) */ SInt32 dv;
    /* -0x18(sp) */ SInt32 intr_state;
    /* -0x14(sp) */ SInt32 dis;
    /* -0x10(sp) */ SInt32 core;
    /* -0xc(sp) */ SInt32 voice;
    UNIMPLEMENTED();
}

/* 0002f418 00030474 */ void snd_StartVAGStreamSounding(/* 0x0(sp) */ struct VAGStreamHeader *stream, /* 0x4(sp) */ SInt32 do_sync_list) {
    /* -0x30(sp) */ SInt32 paused;
    /* -0x2c(sp) */ UInt32 core;
    /* -0x28(sp) */ UInt32 c_v;
    /* -0x24(sp) */ SInt32 x;
    /* -0x20(sp) */ bool first;
    /* -0x1c(sp) */ SInt32 dv;
    /* -0x18(sp) */ UInt32 dv_core;
    /* -0x14(sp) */ UInt32 dv_v;
    /* -0x10(sp) */ SInt32 state;
    /* -0xc(sp) */ SInt32 dis;
    UNIMPLEMENTED();
}

/* 00030474 00030678 */ SInt32 snd_MakeVAGLoop(/* 0x0(sp) */ SInt8 *vag, /* 0x4(sp) */ SInt32 size, /* 0x8(sp) */ SInt32 final, /* 0xc(sp) */ SInt32 sl) {}
/* 00030678 0003085c */ struct VAGStreamHeader *snd_GetFreeVAGStream(/* 0x0(sp) */ SInt32 vol_group) {
    /* -0x18(sp) */ SInt32 intr_state;
    /* -0x14(sp) */ struct VAGStreamHeader *walk;
    /* -0x10(sp) */ SInt32 dis;
    /* -0xc(sp) */ SInt32 x;
    UNIMPLEMENTED();
}

/* 0003085c 00030958 */ SInt32 snd_AddVAGStreamDMABuffer(/* 0x0(sp) */ struct VAGBuffer *buffer) {
    /* -0x10(sp) */ SInt32 intr_state;
    /* -0xc(sp) */ SInt32 dis;
    UNIMPLEMENTED();
}

/* 00030958 00030ab8 */ bool snd_AddVAGStreamLoadBuffer(/* 0x0(sp) */ struct VAGBuffer *buffer) {
    /* -0x18(sp) */ SInt32 intr_state;
    /* -0x14(sp) */ struct VAGBuffer *walk;
    /* -0x10(sp) */ SInt32 dis;
    UNIMPLEMENTED();
}

/* 00030ab8 00030c18 */ void snd_RemoveVAGStreamLoadBuffer(/* 0x0(sp) */ struct VAGBuffer *buffer) {
    /* -0x18(sp) */ struct VAGBuffer *walk;
    /* -0x14(sp) */ SInt32 intr_state;
    /* -0x10(sp) */ SInt32 dis;
    UNIMPLEMENTED();
}

/* 00030c18 00030d78 */ void snd_RemoveVAGStreamDMABuffer(/* 0x0(sp) */ struct VAGBuffer *buffer) {
    /* -0x18(sp) */ struct VAGBuffer *walk;
    /* -0x14(sp) */ SInt32 intr_state;
    /* -0x10(sp) */ SInt32 dis;
    UNIMPLEMENTED();
}

/* 00030d78 00030e68 */ void snd_AddQueEntryToHandler(/* 0x0(sp) */ VAGStreamHandlerPtr handler, /* 0x4(sp) */ struct VAGStreamQueEntry *que) {
    /* -0x18(sp) */ struct VAGStreamQueEntry *walk;
    /* -0x14(sp) */ SInt32 intr_state;
    /* -0x10(sp) */ SInt32 dis;
    UNIMPLEMENTED();
}

/* 00030e68 00030fd8 */ struct VAGStreamQueEntry *snd_GetFreeQueSlot() {
    /* -0x18(sp) */ SInt32 x;
    /* -0x14(sp) */ SInt32 intr_state;
    /* -0x10(sp) */ SInt32 dis;
    UNIMPLEMENTED();
}

/* 00030fd8 0003107c */ void snd_FreeQueChain(/* 0x0(sp) */ struct VAGStreamQueEntry *que, /* 0x4(sp) */ bool is_stdio) {
    UNIMPLEMENTED();
}
/* 0003107c 00031128 */ void snd_CleanUpThread(/* 0x0(sp) */ SInt32 *tid) {
    UNIMPLEMENTED();
}
/* 00031128 000311a0 */ void snd_CleanUpSema(/* 0x0(sp) */ SInt32 *sema) {
    UNIMPLEMENTED();
}
/* 000311a0 0003148c */ void snd_FreeVAGStreamResources() {
    /* -0x60(sp) */ struct VAGStreamHeader *hold;
    /* -0x5c(sp) */ struct VAGStreamHeader *walk;
    /* -0x58(sp) */ SInt32 dis;
    /* -0x54(sp) */ SInt32 intr_state;
    /* -0x50(sp) */ ThreadInfo info;
    /* -0xc(sp) */ SInt32 err;
    UNIMPLEMENTED();
}

/* 0003148c 00031574 */ void snd_CloseVAGStreaming() {
    UNIMPLEMENTED();
}
/* 00031574 0003167c */ SInt32 snd_StreamSafeCheckCDIdle(/* 0x0(sp) */ SInt32 block) {
    UNIMPLEMENTED();
}
/* 0003167c 0003198c */ VAGStreamHandlerPtr snd_GetFreeVAGStreamHandler() {
    /* -0x18(sp) */ SInt32 x;
    /* -0x14(sp) */ SInt32 owner_num;
    /* -0x10(sp) */ SInt32 intr_state;
    /* -0xc(sp) */ SInt32 dis;
    UNIMPLEMENTED();
}

/* 0003198c 00031aa0 */ void snd_AddStreamToHandler(/* 0x0(sp) */ VAGStreamHandlerPtr handler, /* 0x4(sp) */ struct VAGStreamHeader *stream) {
    /* -0x10(sp) */ struct VAGStreamHeader *walk;
    UNIMPLEMENTED();
}

/* 00031aa0 00031bb4 */ void snd_AddStreamToHandlerQueue(/* 0x0(sp) */ VAGStreamHandlerPtr handler, /* 0x4(sp) */ struct VAGStreamHeader *stream) {
    /* -0x10(sp) */ struct VAGStreamHeader *walk;
    UNIMPLEMENTED();
}

/* 00031bb4 00031c34 */ SInt32 snd_GetNumStreamChannels(/* 0x0(sp) */ struct VAGStreamHeader *stream) {
    /* -0x10(sp) */ SInt32 count;
    UNIMPLEMENTED();
}

/* 00031c34 00031cfc */ SInt32 snd_FixVol(/* 0x0(sp) */ SInt32 orig_vol, /* 0x4(sp) */ SInt32 new_vol) {
    UNIMPLEMENTED();
}
/* 00031cfc 00031dd4 */ SInt32 snd_FixPan(/* 0x0(sp) */ SInt32 orig_pan, /* 0x4(sp) */ SInt32 new_pan) {
    UNIMPLEMENTED();
}
/* 00031dd4 00032160 */ void snd_SetVAGStreamSubGroupVolPan(/* 0x0(sp) */ UInt32 handle, /* 0x4(sp) */ SInt32 group, /* 0x8(sp) */ SInt32 vol, /* 0xc(sp) */ SInt32 pan) {
    /* -0x18(sp) */ VAGStreamHandlerPtr hand;
    /* -0x14(sp) */ struct VAGStreamHeader *stream;
    /* -0x10(sp) */ struct VAGStreamQueEntry *walk;
    UNIMPLEMENTED();
}

/* 00032160 0003286c */ void snd_SetVAGStreamVolPan(/* 0x0(sp) */ UInt32 handle, /* 0x4(sp) */ SInt32 vol, /* 0x8(sp) */ SInt32 pan) {
    /* -0x28(sp) */ VAGStreamHandlerPtr hand;
    /* -0x24(sp) */ struct VAGStreamHeader *stream;
    /* -0x20(sp) */ SInt32 own_the_allocator;
    /* -0x1c(sp) */ SpuVolume spu_vol;
    /* -0x18(sp) */ SInt32 x;
    /* -0x14(sp) */ SInt32 intr_state;
    /* -0x10(sp) */ SInt32 dis;
    UNIMPLEMENTED();
}

/* 0003286c 00032a48 */ VAGStreamHeaderPtr snd_GetFirstOfAdjacentStreamHeaders(/* 0x0(sp) */ VAGStreamHeaderPtr consider, /* 0x4(sp) */ SInt32 num_channels) {
    /* -0x28(sp) */ VAGStreamHeaderPtr start;
    /* -0x24(sp) */ VAGStreamHeaderPtr last_tested;
    /* -0x20(sp) */ SInt32 result;
    /* -0x1c(sp) */ SInt32 intr_state;
    /* -0x18(sp) */ SInt32 i;
    /* -0x14(sp) */ SInt32 dis;
    /* -0x10(sp) */ SInt32 vol_group;
    UNIMPLEMENTED();
}

/* 00032a48 00032b4c */ SInt32 snd_GetFreeStreamLoop(/* 0x0(sp) */ VAGStreamHeaderPtr start, /* 0x4(sp) */ VAGStreamHeaderPtr consider, /* 0x8(sp) */ SInt32 num_channels, /* 0xc(sp) */ VAGStreamHeaderPtr *last_tested, /* 0x10(sp) */ SInt32 vol_group) {}
/* 00032b4c 00032cc0 */ bool snd_StreamIsFreeForVolGroup(/* 0x0(sp) */ VAGStreamHeaderPtr stream, /* 0x4(sp) */ SInt32 vol_group) {
    /* -0x10(sp) */ SInt32 index;
    /* -0xc(sp) */ struct VAGStreamHeader *walk;
    UNIMPLEMENTED();
}

/* 00032cc0 00032fc4 */ void snd_SetVagStreamPitchModifier(/* 0x0(sp) */ UInt32 handle, /* -0x28(sp) */ SInt16 mod) {
    /* -0x24(sp) */ SInt32 core;
    /* -0x20(sp) */ SInt32 voice;
    /* -0x1c(sp) */ struct VAGStreamHeader *stream;
    /* -0x18(sp) */ VAGStreamHandlerPtr hand;
    /* -0x14(sp) */ SInt32 x;
    /* -0x10(sp) */ SInt32 intr_state;
    /* -0xc(sp) */ SInt32 dis;
    UNIMPLEMENTED();
}

/* 00032fc4 00033224 */ UInt16 snd_ModifyRawPitch(/* 0x0(sp) */ SInt32 pm, /* 0x4(sp) */ UInt32 pitch) {
    /* -0x18(sp) */ SInt32 next_shift;
    /* -0x14(sp) */ SInt32 shift_notes;
    /* -0x10(sp) */ SInt32 x;
    UNIMPLEMENTED();
}

/* 00033224 00033280 */ void snd_DeferredFileCloseWatcher() {}
/* 00033280 0003336c */ void snd_CloseAllDeferredFiles() {
    /* -0x10(sp) */ int i;
    UNIMPLEMENTED();
}

/* 0003336c 0003340c */ bool snd_AllStdioFilesClosed() {
    /* -0x10(sp) */ SInt32 i;
    UNIMPLEMENTED();
}

/* 0003340c 00033514 */ void snd_AddDeferredCloseFile(/* 0x0(sp) */ SInt32 handle, /* 0x4(sp) */ int from) {
    /* -0x10(sp) */ SInt32 i;
    UNIMPLEMENTED();
}
