#include "989snd.h"
#include "types.h"

/* data 0 */ UInt16 g989Version = 0x301;
/* data 4 */ // ModuleInfo Module = {"989snd_Library", g989Version};
/* data c */ struct Extern989MonitorInfo* g989Monitor = NULL;
/* data 10 */ SInt32 gThreadPriority_TICK = 9;
/* data 14 */ SInt32 gThreadPriority_RPC = 0x3a;
/* data 18 */ SInt32 gThreadPriority_STREAM = 0x3c;
/* data 1c */ SInt32 gMainRPCThreadId = 0;
/* data 20 */ SInt32 gEEDMADoneSema = 0;
/* data 24 */ SInt32 gFileReadMutex = 0;
/* data 28 */ bool gDoRPC = true;
/* data 2c */ SInt8* snd_MESSAGE_RECIEVE_BUFFER = NULL;
/* data 30 */ UInt32* snd_MESSAGE_RETURN_BUFFER = NULL;
/* data 34 */ UInt32* gWriteBackdataOffset = NULL;
/* data 38 */ Extern989HandlerPtr gExternHandlersList;
/* data 3c */ CommandHandler gCommandFunc[] = {
    snd_CMD_SL_CLOSE,
    snd_CMD_SL_LOADBANK,
    snd_CMD_SL_LOADBANKBYLOC,
    snd_CMD_SL_LOADMMD,
    snd_CMD_SL_LOADMMDBYLOC,
    snd_CMD_SL_UNLOADBANK_A,
    snd_CMD_SL_UNLOADMIDI_A,
    snd_CMD_SL_RESOLVEBANKS,
    snd_CMD_SL_SETMASTERVOL_A,
    snd_CMD_SL_GETMASTERVOL,
    snd_CMD_SL_SETPLAYBACKMODE_A,
    snd_CMD_SL_GETPLAYBACKMODE,
    snd_CMD_SL_SETMIXERMODE_A,
    snd_CMD_SL_SETREVERBTYPE,
    snd_CMD_SL_SETREVERBDEPTH,
    snd_CMD_SL_AUTOVERB,
    snd_CMD_SL_PLAYSOUND,
    snd_CMD_SL_PLAYSOUND_A,
    snd_CMD_SL_PAUSESOUND,
    snd_CMD_SL_CONTINUESOUND,
    snd_CMD_SL_STOPSOUND,
    snd_CMD_SL_PAUSEALLSOUNDS,
    snd_CMD_SL_CONTINUEALLSOUNDS,
    snd_CMD_SL_STOPALLSOUNDS,
    snd_CMD_SL_SOUNDISSTILLPLAYING,
    snd_CMD_SL_ISSOUNDALOOPER,
    snd_CMD_SL_SETSOUNDVOLPAN_A,
    snd_CMD_SL_GETSOUNDORIGPITCH,
    snd_CMD_SL_GETSOUNDCURRPITCH,
    snd_CMD_SL_SETSOUNDPITCH_A,
    snd_CMD_SL_SETSOUNDPITCHBEND_A,
    snd_CMD_SL_SETSOUNDPITCHMOD_A,
    snd_CMD_SL_SETSOUNDPARAMS,
    snd_CMD_SL_AUTOVOL_A,
    snd_CMD_SL_AUTOPAN_A,
    snd_CMD_SL_AUTOPITCH_A,
    snd_CMD_SL_AUTOPITCHBEND_A,
    snd_CMD_SL_GETGLOBALEXCITE,
    snd_CMD_SL_SETGLOBALEXCITE_A,
    snd_CMD_SL_GETMIDIREGISTER,
    snd_CMD_SL_SETMIDIREGISTER_A,
    snd_CMD_SL_INITVAGSTREAMINGEX,
    snd_CMD_SL_PLAYVAGSTREAMBYNAME,
    snd_CMD_SL_PLAYVAGSTREAMBYLOC,
    snd_CMD_SL_PAUSEVAGSTREAM,
    snd_CMD_SL_CONTINUEVAGSTREAM,
    snd_CMD_SL_STOPVAGSTREAM,
    snd_CMD_SL_GETVAGSTREAMQUEUECOUNT,
    snd_CMD_SL_GETVAGSTREAMLOC,
    snd_CMD_SL_GETVAGSTREAMTIMEREMAINING,
    snd_CMD_SL_GETVAGSTREAMTOTALTIME,
    snd_CMD_SL_STOPALLVAGSTREAMS,
    snd_CMD_SL_CLOSEVAGSTREAMING,
    snd_CMD_SL_STREAMSAFECHECKCDIDLE,
    snd_CMD_SL_STREAMSAFECDBREAK,
    snd_CMD_SL_STREAMSAFECDREAD,
    snd_CMD_SL_ALLOCIOPRAM,
    snd_CMD_SL_FREEIOPRAM,
    snd_CMD_SL_INITMOVIESOUND,
    snd_CMD_SL_CLOSEMOVIESOUND,
    snd_CMD_SL_RESETMOVIESOUND,
    snd_CMD_SL_STARTMOVIESOUND,
    snd_CMD_SL_PAUSEMOVIESOUND,
    snd_CMD_SL_GETTRANSSTATUS,
    snd_CMD_SL_GETSTICK,
    snd_CMD_SL_GETVOICEALLOC,
    snd_CMD_SL_LOCKVALLOC,
    snd_CMD_SL_EXTERNVALLOC,
    snd_CMD_SL_EXTERNVFREE,
    snd_CMD_SL_UNLOCKVALLOC,
    snd_CMD_SL_SRAMMALLOC,
    snd_CMD_SL_SRAMMARKUSED,
    snd_CMD_SL_SRAMFREE,
    snd_CMD_SL_SRAMGETFREE,
    snd_CMD_SL_SRAMMAXFREE,
    snd_CMD_SL_EXTERNCALL,
    snd_CMD_SL_COMMAND_BATCH,
    snd_CMD_SL_SETGROUPVOICERANGE,
    snd_CMD_SL_ISVAGSTREAMBUFFERED,
    snd_CMD_SL_SETREVERBEX,
    snd_CMD_SL_PREALLOCREVERBWORKAREA,
    snd_CMD_SL_GETLASTLOADERROR,
    snd_CMD_SL_SETPANMODE,
    snd_CMD_SL_PLAYSOUNDBYNAME,
    snd_CMD_SL_PLAYSOUNDBYNAME_A,
    snd_CMD_SL_GETSOUNDUD,
    snd_CMD_SL_BANKLOADFROMEE,
    snd_CMD_SL_SETMOVIESOUNDVOLPAN,
    snd_CMD_SL_BANKLOADFROMIOP,
    snd_CMD_SL_UPDATEMOVIEADPCM,
    snd_CMD_SL_GETMOVIENAX,
    snd_CMD_SL_PLAYSOUNDEX,
    snd_CMD_SL_GETSOUNDREG,
    snd_CMD_SL_SETSOUNDREG,
    snd_CMD_SL_SETALLSOUNDREG,
    snd_CMD_SL_SETMASTERVOLUMEDUCKER,
    snd_CMD_SL_STOPALLSOUNDSINGROUP,
    snd_CMD_SL_SETVAGSTREAMCHANNELRANGE,
    snd_CMD_SL_SETVAGSTREAMSUBGROUPVOLPAN,
    snd_CMD_SL_SETREVERBMODE,
    snd_CMD_SL_CDSTATUS,
    snd_CMD_SL_GETSFXGLOBALREG,
    snd_CMD_SL_SETSFXGLOBALREG,
    snd_CMD_SL_EXTERNCALLWITHDATA
};
/* data 1e4 */ SndErrorDisplayFunc gErrorDisplayFunc = NULL;
/* bss 0 */ SInt8 snd_LOADER_MESSAGE_RECIEVE_BUFFER[256];
/* bss 100 */ UInt32 snd_LOADER_MESSAGE_RETURN_BUFFER[4];

/* 00000000 00000120 */ bool snd_RegisterExternProcHandler(/* 0x0(sp) */ Extern989HandlerPtr hand)
{
    /* -0x10(sp) */ Extern989HandlerPtr work;
}

/* 00000120 00000260 */ bool snd_UnregisterExternProcHandler(/* 0x0(sp) */ Extern989HandlerPtr hand)
{
    /* -0x10(sp) */ Extern989HandlerPtr work;
}

/* 00000260 0000030c */ Extern989HandlerPtr snd_FindExternProcHandler(/* 0x0(sp) */ UInt32 id)
{
    /* -0x10(sp) */ Extern989HandlerPtr work;
}

/* 0000030c 00000424 */ SInt32 snd_DoExternCall(/* 0x0(sp) */ UInt32 proc_id, /* 0x4(sp) */ SInt32 func_index, /* 0x8(sp) */ SInt32 arg1, /* 0xc(sp) */ SInt32 arg2, /* 0x10(sp) */ SInt32 arg3, /* 0x14(sp) */ SInt32 arg4, /* 0x18(sp) */ SInt32 arg5)
{
    /* -0x10(sp) */ Extern989HandlerPtr work;
}

/* 00000424 00000534 */ SInt32 snd_DoExternCallWithData(/* 0x0(sp) */ UInt32 proc_id, /* 0x4(sp) */ SInt32 func_index, /* 0x8(sp) */ SInt32 data_size, /* 0xc(sp) */ void* data_ptr)
{
    /* -0x10(sp) */ Extern989HandlerPtr work;
}

/* 00000534 00000598 */ void snd_CMD_SL_INIT(/* 0x0(sp) */ SInt8* msg_data)
{
    /* -0x10(sp) */ UInt32* data;
}

/* 00000598 000005cc */ void snd_CMD_SL_CLOSE(/* 0x0(sp) */ SInt8* msg_data) {}
/* 000005cc 00000630 */ void snd_CMD_SL_LOADBANK(/* 0x0(sp) */ SInt8* msg_data) {}
/* 00000630 000006a0 */ void snd_CMD_SL_LOADBANKBYLOC(/* 0x0(sp) */ SInt8* msg_data)
{
    /* -0x10(sp) */ SInt32* data;
}

/* 000006a0 00000704 */ void snd_CMD_SL_BANKLOADFROMEE(/* 0x0(sp) */ SInt8* msg_data)
{
    /* -0x10(sp) */ UInt32* data;
}

/* 00000704 00000768 */ void snd_CMD_SL_BANKLOADFROMIOP(/* 0x0(sp) */ SInt8* msg_data)
{
    /* -0x10(sp) */ UInt32* data;
}

/* 00000768 000007c4 */ void snd_CMD_SL_LOADMMD(/* 0x0(sp) */ SInt8* msg_data) {}
/* 000007c4 0000082c */ void snd_CMD_SL_LOADMMDBYLOC(/* 0x0(sp) */ SInt8* msg_data)
{
    /* -0x10(sp) */ SInt32* data;
}

/* 0000082c 0000087c */ void snd_CMD_SL_UNLOADBANK_A(/* 0x0(sp) */ SInt8* msg_data) {}
/* 0000087c 000008bc */ void snd_CMD_SL_UNLOADMIDI_A(/* 0x0(sp) */ SInt8* msg_data) {}
/* 000008bc 000008f0 */ void snd_CMD_SL_RESOLVEBANKS(/* 0x0(sp) */ SInt8* msg_data) {}
/* 000008f0 00000948 */ void snd_CMD_SL_SETMASTERVOL_A(/* 0x0(sp) */ SInt8* msg_data)
{
    /* -0x10(sp) */ SInt32* data;
}

/* 00000948 00000998 */ void snd_CMD_SL_GETMASTERVOL(/* 0x0(sp) */ SInt8* msg_data) {}
/* 00000998 000009d8 */ void snd_CMD_SL_SETPLAYBACKMODE_A(/* 0x0(sp) */ SInt8* msg_data) {}
/* 000009d8 00000a1c */ void snd_CMD_SL_GETPLAYBACKMODE(/* 0x0(sp) */ SInt8* msg_data) {}
/* 00000a1c 00000a74 */ void snd_CMD_SL_SETMIXERMODE_A(/* 0x0(sp) */ SInt8* msg_data)
{
    /* -0x10(sp) */ SInt32* data;
}

/* 00000a74 00000ab4 */ void snd_CMD_SL_SETREVERBMODE(/* 0x0(sp) */ SInt8* msg_data) {}
/* 00000ab4 00000b1c */ void snd_CMD_SL_SETGROUPVOICERANGE(/* 0x0(sp) */ SInt8* msg_data)
{
    /* -0x10(sp) */ SInt32* data;
}

/* 00000b1c 00000b74 */ void snd_CMD_SL_SETREVERBTYPE(/* 0x0(sp) */ SInt8* msg_data)
{
    /* -0x10(sp) */ SInt32* data;
}

/* 00000b74 00000bdc */ void snd_CMD_SL_SETREVERBDEPTH(/* 0x0(sp) */ SInt8* msg_data)
{
    /* -0x10(sp) */ SInt32* data;
}

/* 00000bdc 00000c54 */ void snd_CMD_SL_AUTOVERB(/* 0x0(sp) */ SInt8* msg_data)
{
    /* -0x10(sp) */ SInt32* data;
}

/* 00000c54 00000d0c */ void snd_CMD_SL_PLAYSOUND(/* 0x0(sp) */ SInt8* msg_data)
{
    /* -0x10(sp) */ SInt32* data;
}

/* 00000d0c 00000db4 */ void snd_CMD_SL_PLAYSOUND_A(/* 0x0(sp) */ SInt8* msg_data)
{
    /* -0x10(sp) */ SInt32* data;
}

/* 00000db4 00000e08 */ void snd_CMD_SL_PLAYSOUNDEX(/* 0x0(sp) */ SInt8* msg_data)
{
    /* -0x10(sp) */ SndPlayParamsPtr params;
}

/* 00000e08 00000e48 */ void snd_CMD_SL_PAUSESOUND(/* 0x0(sp) */ SInt8* msg_data) {}
/* 00000e48 00000e88 */ void snd_CMD_SL_CONTINUESOUND(/* 0x0(sp) */ SInt8* msg_data) {}
/* 00000e88 00000ec8 */ void snd_CMD_SL_STOPSOUND(/* 0x0(sp) */ SInt8* msg_data) {}
/* 00000ec8 00000f08 */ void snd_CMD_SL_PAUSEALLSOUNDS(/* 0x0(sp) */ SInt8* msg_data) {}
/* 00000f08 00000f48 */ void snd_CMD_SL_CONTINUEALLSOUNDS(/* 0x0(sp) */ SInt8* msg_data) {}
/* 00000f48 00000f7c */ void snd_CMD_SL_STOPALLSOUNDS(/* 0x0(sp) */ SInt8* msg_data) {}
/* 00000f7c 00000fbc */ void snd_CMD_SL_STOPALLSOUNDSINGROUP(/* 0x0(sp) */ SInt8* msg_data) {}
/* 00000fbc 0000100c */ void snd_CMD_SL_SOUNDISSTILLPLAYING(/* 0x0(sp) */ SInt8* msg_data) {}
/* 0000100c 00001074 */ void snd_CMD_SL_ISSOUNDALOOPER(/* 0x0(sp) */ SInt8* msg_data)
{
    /* -0x10(sp) */ SInt32* data;
}

/* 00001074 000010dc */ void snd_CMD_SL_SETSOUNDVOLPAN_A(/* 0x0(sp) */ SInt8* msg_data)
{
    /* -0x10(sp) */ SInt32* data;
}

/* 000010dc 00001144 */ void snd_CMD_SL_GETSOUNDORIGPITCH(/* 0x0(sp) */ SInt8* msg_data)
{
    /* -0x10(sp) */ SInt32* data;
}

/* 00001144 00001194 */ void snd_CMD_SL_GETSOUNDCURRPITCH(/* 0x0(sp) */ SInt8* msg_data) {}
/* 00001194 000011ec */ void snd_CMD_SL_SETSOUNDPITCH_A(/* 0x0(sp) */ SInt8* msg_data)
{
    /* -0x10(sp) */ SInt32* data;
}

/* 000011ec 00001244 */ void snd_CMD_SL_SETSOUNDPITCHBEND_A(/* 0x0(sp) */ SInt8* msg_data)
{
    /* -0x10(sp) */ SInt32* data;
}

/* 00001244 0000129c */ void snd_CMD_SL_SETSOUNDPITCHMOD_A(/* 0x0(sp) */ SInt8* msg_data)
{
    /* -0x10(sp) */ SInt32* data;
}

/* 0000129c 00001354 */ void snd_CMD_SL_SETSOUNDPARAMS(/* 0x0(sp) */ SInt8* msg_data)
{
    /* -0x10(sp) */ SInt32* data;
}

/* 00001354 000013d0 */ void snd_CMD_SL_AUTOVOL_A(/* 0x0(sp) */ SInt8* msg_data)
{
    /* -0x10(sp) */ SInt32* data;
}

/* 000013d0 00001460 */ void snd_CMD_SL_AUTOPAN_A(/* 0x0(sp) */ SInt8* msg_data)
{
    /* -0x10(sp) */ SInt32* data;
}

/* 00001460 000014d8 */ void snd_CMD_SL_AUTOPITCH_A(/* 0x0(sp) */ SInt8* msg_data)
{
    /* -0x10(sp) */ SInt32* data;
}

/* 000014d8 00001550 */ void snd_CMD_SL_AUTOPITCHBEND_A(/* 0x0(sp) */ SInt8* msg_data)
{
    /* -0x10(sp) */ SInt32* data;
}

/* 00001550 00001594 */ void snd_CMD_SL_GETGLOBALEXCITE(/* 0x0(sp) */ SInt8* msg_data) {}
/* 00001594 000015d4 */ void snd_CMD_SL_SETGLOBALEXCITE_A(/* 0x0(sp) */ SInt8* msg_data) {}
/* 000015d4 0000163c */ void snd_CMD_SL_GETMIDIREGISTER(/* 0x0(sp) */ SInt8* msg_data)
{
    /* -0x10(sp) */ SInt32* data;
}

/* 0000163c 000016a4 */ void snd_CMD_SL_SETMIDIREGISTER_A(/* 0x0(sp) */ SInt8* msg_data)
{
    /* -0x10(sp) */ SInt32* data;
}

/* 000016a4 0000172c */ void snd_CMD_SL_INITVAGSTREAMINGEX(/* 0x0(sp) */ SInt8* msg_data)
{
    /* -0x10(sp) */ SInt32* data;
}

/* 0000172c 00001794 */ void snd_CMD_SL_SETVAGSTREAMCHANNELRANGE(/* 0x0(sp) */ SInt8* msg_data)
{
    /* -0x10(sp) */ SInt32* data;
}

/* 00001794 0000180c */ void snd_CMD_SL_SETVAGSTREAMSUBGROUPVOLPAN(/* 0x0(sp) */ SInt8* msg_data)
{
    /* -0x10(sp) */ SInt32* data;
}

/* 0000180c 00001940 */ void snd_CMD_SL_PLAYVAGSTREAMBYNAME(/* 0x0(sp) */ SInt8* msg_data)
{
    /* -0x18(sp) */ char* name1;
    /* -0x14(sp) */ char* name2;
    /* -0x10(sp) */ struct PVSBN_struct* data;
}

/* 00001940 00001a78 */ void snd_CMD_SL_PLAYVAGSTREAMBYLOC(/* 0x0(sp) */ SInt8* msg_data)
{
    /* -0x10(sp) */ SInt32* data;
}

/* 00001a78 00001ab8 */ void snd_CMD_SL_PAUSEVAGSTREAM(/* 0x0(sp) */ SInt8* msg_data) {}
/* 00001ab8 00001af8 */ void snd_CMD_SL_CONTINUEVAGSTREAM(/* 0x0(sp) */ SInt8* msg_data) {}
/* 00001af8 00001b38 */ void snd_CMD_SL_STOPVAGSTREAM(/* 0x0(sp) */ SInt8* msg_data) {}
/* 00001b38 00001b88 */ void snd_CMD_SL_GETVAGSTREAMQUEUECOUNT(/* 0x0(sp) */ SInt8* msg_data) {}
/* 00001b88 00001bd8 */ void snd_CMD_SL_GETVAGSTREAMLOC(/* 0x0(sp) */ SInt8* msg_data) {}
/* 00001bd8 00001c28 */ void snd_CMD_SL_GETVAGSTREAMTIMEREMAINING(/* 0x0(sp) */ SInt8* msg_data) {}
/* 00001c28 00001c78 */ void snd_CMD_SL_GETVAGSTREAMTOTALTIME(/* 0x0(sp) */ SInt8* msg_data) {}
/* 00001c78 00001cc8 */ void snd_CMD_SL_ISVAGSTREAMBUFFERED(/* 0x0(sp) */ SInt8* msg_data) {}
/* 00001cc8 00001cfc */ void snd_CMD_SL_STOPALLVAGSTREAMS(/* 0x0(sp) */ SInt8* msg_data) {}
/* 00001cfc 00001d30 */ void snd_CMD_SL_CLOSEVAGSTREAMING(/* 0x0(sp) */ SInt8* msg_data) {}
/* 00001d30 00001d80 */ void snd_CMD_SL_STREAMSAFECHECKCDIDLE(/* 0x0(sp) */ SInt8* msg_data) {}
/* 00001d80 00001dc4 */ void snd_CMD_SL_STREAMSAFECDBREAK(/* 0x0(sp) */ SInt8* msg_data) {}
/* 00001dc4 00001e2c */ void snd_CMD_SL_STREAMSAFECDREAD(/* 0x0(sp) */ SInt8* msg_data)
{
    /* -0x10(sp) */ UInt32* data;
}

/* 00001e2c 00001f84 */ void snd_CMD_SL_STREAMSAFECDSEARCHFILE(/* 0x0(sp) */ SInt8* msg_data)
{
    /* -0x58(sp) */ UInt32 ee_location;
    /* -0x54(sp) */ char* fname;
    /* -0x50(sp) */ sceCdlFILE cdf;
    /* -0x2c(sp) */ SInt32 ret;
    /* -0x28(sp) */ sceSifDmaData transData;
    /* -0x18(sp) */ SInt32 did;
    /* -0x14(sp) */ SInt32 dis;
    /* -0x10(sp) */ SInt32 intr_state;
}

/* 00001f84 00001fe0 */ void snd_CMD_SL_ALLOCIOPRAM(/* 0x0(sp) */ SInt8* msg_data) {}
/* 00001fe0 00002024 */ void snd_CMD_SL_FREEIOPRAM(/* 0x0(sp) */ SInt8* msg_data) {}
/* 00002024 000020dc */ void snd_CMD_SL_INITMOVIESOUND(/* 0x0(sp) */ SInt8* msg_data)
{
    /* -0x10(sp) */ SInt32* data;
}

/* 000020dc 00002110 */ void snd_CMD_SL_CLOSEMOVIESOUND(/* 0x0(sp) */ SInt8* msg_data) {}
/* 00002110 00002144 */ void snd_CMD_SL_RESETMOVIESOUND(/* 0x0(sp) */ SInt8* msg_data) {}
/* 00002144 000021d4 */ void snd_CMD_SL_STARTMOVIESOUND(/* 0x0(sp) */ SInt8* msg_data)
{
    /* -0x10(sp) */ SInt32* data;
}

/* 000021d4 00002218 */ void snd_CMD_SL_PAUSEMOVIESOUND(/* 0x0(sp) */ SInt8* msg_data) {}
/* 00002218 00002270 */ void snd_CMD_SL_SETMOVIESOUNDVOLPAN(/* 0x0(sp) */ SInt8* msg_data)
{
    /* -0x10(sp) */ SInt32* data;
}

/* 00002270 000022c8 */ void snd_CMD_SL_UPDATEMOVIEADPCM(/* 0x0(sp) */ SInt8* msg_data)
{
    /* -0x10(sp) */ SInt32* data;
}

/* 000022c8 0000230c */ void snd_CMD_SL_GETMOVIENAX(/* 0x0(sp) */ SInt8* msg_data) {}
/* 0000230c 00002350 */ void snd_CMD_SL_GETTRANSSTATUS(/* 0x0(sp) */ SInt8* msg_data) {}
/* 00002350 00002394 */ void snd_CMD_SL_GETSTICK(/* 0x0(sp) */ SInt8* msg_data) {}
/* 00002394 00002488 */ void snd_CMD_SL_GETVOICEALLOC(/* 0x0(sp) */ SInt8* msg_data)
{
    /* -0x18(sp) */ SInt32 core;
    /* -0x14(sp) */ SInt32 x;
    /* -0x10(sp) */ SInt32* SInt32_data;
}

/* 00002488 000024d8 */ void snd_CMD_SL_LOCKVALLOC(/* 0x0(sp) */ SInt8* msg_data) {}
/* 000024d8 00002540 */ void snd_CMD_SL_EXTERNVALLOC(/* 0x0(sp) */ SInt8* msg_data)
{
    /* -0x10(sp) */ SInt32* data;
}

/* 00002540 00002580 */ void snd_CMD_SL_EXTERNVFREE(/* 0x0(sp) */ SInt8* msg_data) {}
/* 00002580 000025b4 */ void snd_CMD_SL_UNLOCKVALLOC(/* 0x0(sp) */ SInt8* msg_data) {}
/* 000025b4 00002634 */ void snd_CMD_SL_SRAMMALLOC(/* 0x0(sp) */ SInt8* msg_data)
{
    /* -0x10(sp) */ SInt32 dis;
    /* -0xc(sp) */ SInt32 oldstat;
}

/* 00002634 000026cc */ void snd_CMD_SL_SRAMMARKUSED(/* 0x0(sp) */ SInt8* msg_data)
{
    /* -0x18(sp) */ SInt32 dis;
    /* -0x14(sp) */ SInt32 oldstat;
    /* -0x10(sp) */ UInt32* data;
}

/* 000026cc 00002754 */ void snd_CMD_SL_SRAMFREE(/* 0x0(sp) */ SInt8* msg_data)
{
    /* -0x18(sp) */ SInt32 dis;
    /* -0x14(sp) */ SInt32 oldstat;
    /* -0x10(sp) */ UInt32* data;
}

/* 00002754 000027c8 */ void snd_CMD_SL_SRAMGETFREE(/* 0x0(sp) */ SInt8* msg_data)
{
    /* -0x10(sp) */ SInt32 dis;
    /* -0xc(sp) */ SInt32 oldstat;
}

/* 000027c8 0000283c */ void snd_CMD_SL_SRAMMAXFREE(/* 0x0(sp) */ SInt8* msg_data)
{
    /* -0x10(sp) */ SInt32 dis;
    /* -0xc(sp) */ SInt32 oldstat;
}

/* 0000283c 0000290c */ void snd_CMD_SL_EXTERNCALL(/* 0x0(sp) */ SInt8* msg_data)
{
    /* -0x10(sp) */ SInt32* data;
}

/* 0000290c 00002990 */ void snd_CMD_SL_EXTERNCALLWITHDATA(/* 0x0(sp) */ SInt8* msg_data)
{
    /* -0x10(sp) */ SInt32* data;
}

/* 00002990 00002a20 */ void snd_CMD_SL_SETREVERBEX(/* 0x0(sp) */ SInt8* msg_data)
{
    /* -0x10(sp) */ SInt32* data;
}

/* 00002a20 00002a78 */ void snd_CMD_SL_PREALLOCREVERBWORKAREA(/* 0x0(sp) */ SInt8* msg_data)
{
    /* -0x10(sp) */ SInt32* data;
}

/* 00002a78 00002abc */ void snd_CMD_SL_GETLASTLOADERROR(/* 0x0(sp) */ SInt8* msg_data) {}
/* 00002abc 00002afc */ void snd_CMD_SL_SETPANMODE(/* 0x0(sp) */ SInt8* msg_data) {}
/* 00002afc 00002b30 */ void snd_CMD_SL_PLAYSOUNDBYNAME(/* 0x0(sp) */ SInt8* msg_data) {}
/* 00002b30 00002b54 */ void snd_CMD_SL_PLAYSOUNDBYNAME_A(/* 0x0(sp) */ SInt8* msg_data) {}
/* 00002b54 00002cf0 */ void snd_CMD_SL_GETSOUNDUD(/* 0x0(sp) */ SInt8* msg_data)
{
    /* -0x40(sp) */ SInt32 return_hold[4];
    /* -0x30(sp) */ SInt32 ret;
    /* -0x2c(sp) */ SInt32 intr_state;
    /* -0x28(sp) */ SInt32 dis;
    /* -0x24(sp) */ GetSoundUserDataCommandStruct* data;
    /* -0x20(sp) */ SInt32 did;
    /* -0x1c(sp) */ sceSifDmaData transData;
}

/* 00002cf0 00002d58 */ void snd_CMD_SL_GETSOUNDREG(/* 0x0(sp) */ SInt8* msg_data)
{
    /* -0x10(sp) */ UInt32* data;
}

/* 00002d58 00002dc0 */ void snd_CMD_SL_SETSOUNDREG(/* 0x0(sp) */ SInt8* msg_data)
{
    /* -0x10(sp) */ UInt32* data;
}

/* 00002dc0 00002e1c */ void snd_CMD_SL_GETSFXGLOBALREG(/* 0x0(sp) */ SInt8* msg_data)
{
    /* -0x10(sp) */ SInt32* data;
}

/* 00002e1c 00002e74 */ void snd_CMD_SL_SETSFXGLOBALREG(/* 0x0(sp) */ SInt8* msg_data)
{
    /* -0x10(sp) */ SInt32* data;
}

/* 00002e74 00002ecc */ void snd_CMD_SL_SETALLSOUNDREG(/* 0x0(sp) */ SInt8* msg_data)
{
    /* -0x10(sp) */ UInt32* data;
}

/* 00002ecc 00002f48 */ void snd_CMD_SL_SETMASTERVOLUMEDUCKER(/* 0x0(sp) */ SInt8* msg_data)
{
    /* -0x10(sp) */ SInt32* data;
    /* -0xc(sp) */ DuckerDefPtr state;
}

/* 00002f48 00002f8c */ void snd_CMD_SL_CDSTATUS(/* 0x0(sp) */ SInt8* msg_data) {}
/* 00002f8c 00003120 */ void snd_CMD_SL_COMMAND_BATCH(/* 0x0(sp) */ SInt8* msg_data)
{
    /* -0x18(sp) */ SndCommandBufferPtr batch;
    /* -0x14(sp) */ char* command_buffer_walk;
    /* -0x10(sp) */ SInt32 x;
    /* -0xc(sp) */ SInt16 command;
    /* -0xa(sp) */ SInt16 size;
}

/* 00003120 000031c8 */ static void* snd_EEMessageParser(/* 0x0(sp) */ UInt32 command, /* 0x4(sp) */ void* data, /* 0x8(sp) */ SInt32 size) {}
/* 000031c8 00003274 */ SInt32 snd_StartEEMessaging()
{
    /* -0x68(sp) */ sceSifQueueData qd;
    /* -0x50(sp) */ sceSifServeData sd;
}

/* 00003274 00003454 */ static void* snd_EELoaderMessageParser(/* 0x0(sp) */ UInt32 command, /* 0x4(sp) */ void* data, /* 0x8(sp) */ SInt32 size)
{
    /* -0x10(sp) */ SInt32 ret_val;
}

/* 00003454 000034f0 */ SInt32 snd_StartEELoaderMessaging()
{
    /* -0x68(sp) */ sceSifQueueData qd;
    /* -0x50(sp) */ sceSifServeData sd;
}

/* 000034f0 0000386c */ SInt32 start(/* 0x0(sp) */ SInt32 argc, /* 0x4(sp) */ SInt8** argv)
{
    /* -0x48(sp) */ SemaParam sema;
    /* -0x38(sp) */ ThreadParam param;
    /* -0x24(sp) */ SInt32 th;
    /* -0x20(sp) */ SInt32 x;
    /* -0x1c(sp) */ SInt32 dis;
    /* -0x18(sp) */ SInt32 oldstat;
    /* -0x14(sp) */ int i;
    /* -0x10(sp) */ char* walk;
}

/* 0000386c 00003a10 */ void snd_ParseCommandLineArg(/* 0x0(sp) */ char* arg) {}
/* 00003a10 00003b1c */ SInt32 snd_GetVal(/* 0x0(sp) */ char* st) {}
/* 00003b1c 00003cf4 */ void snd_DumpVersionAndInfo() {}
/* 00003cf4 00003d24 */ void snd_RegisterErrorDisplayFunc(/* 0x0(sp) */ SndErrorDisplayFunc func) {}
/* 00003d24 00003dec */ void snd_ShowError(/* 0x0(sp) */ SInt32 num, /* 0x4(sp) */ UInt32 a1, /* 0x8(sp) */ UInt32 a2, /* 0xc(sp) */ UInt32 a3, /* 0x10(sp) */ UInt32 a4) {}
/* 00003dec 00003e2c */ void snd_EEDMADone(/* 0x0(sp) */ SInt32* sema_id_ptr) {}
/* 00003e2c 00003e5c */ void snd_Install989Monitor(/* 0x0(sp) */ struct Extern989MonitorInfo* mon) {}
