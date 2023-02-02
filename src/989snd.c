#include "989snd.h"
#include "ame.h"
#include "autopan.h"
#include "autopb.h"
#include "autoptch.h"
#include "autoverb.h"
#include "autovol.h"
#include "blocksnd.h"
#include "extern.h"
#include "init.h"
#include "loader.h"
#include "moviesnd.h"
#include "pantable.h"
#include "playsnd.h"
#include "reverb.h"
#include "sndhand.h"
#include "sram.h"
#include "stick.h"
#include "stream.h"
#include "types.h"
#include "valloc.h"
#include "vol.h"

#ifdef INTERNAL_SNDERR
#include "error.h"
#endif

#include <intrman.h>
#include <stdio.h>
#include <sysclib.h>
#include <sysmem.h>

/* data 0 */ UInt16 g989Version = 0x301;
/* data 4 */ // ModuleInfo Module = {"989snd_Library", g989Version};
/* data c */ struct Extern989MonitorInfo *g989Monitor = NULL;
/* data 10 */ SInt32 gThreadPriority_TICK = 9;
/* data 14 */ SInt32 gThreadPriority_RPC = 0x3a;
/* data 18 */ SInt32 gThreadPriority_STREAM = 0x3c;
/* data 1c */ SInt32 gMainRPCThreadId = 0;
/* data 20 */ SInt32 gEEDMADoneSema = 0;
/* data 24 */ SInt32 gFileReadMutex = 0;
/* data 28 */ bool gDoRPC = true;
/* data 2c */ SInt8 *snd_MESSAGE_RECIEVE_BUFFER = NULL;
/* data 30 */ UInt32 *snd_MESSAGE_RETURN_BUFFER = NULL;
/* data 34 */ UInt32 *gWriteBackdataOffset = NULL;
/* data 38 */ Extern989HandlerPtr gExternHandlersList = NULL;
/* data 3c */ CommandHandler gCommandFunc[] = {
    snd_CMD_SL_INIT,
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
    snd_CMD_SL_EXTERNCALLWITHDATA};
/* data 1e4 */ SndErrorDisplayFunc gErrorDisplayFunc = NULL;
/* bss 0 */ SInt8 snd_LOADER_MESSAGE_RECIEVE_BUFFER[256];
/* bss 100 */ UInt32 snd_LOADER_MESSAGE_RETURN_BUFFER[4];

/* 00000000 00000120 */ bool snd_RegisterExternProcHandler(/* 0x0(sp) */ Extern989HandlerPtr hand) {
    /* -0x10(sp) */ Extern989HandlerPtr work;

    if (snd_FindExternProcHandler(hand->proc_id)) {
        snd_ShowError(1, hand->proc_id, hand->proc_id, 0, 0);
        return false;
    }

    hand->reserved1 = 0;
    hand->reserved2 = 0;

    if (gExternHandlersList != NULL) {
        work = gExternHandlersList;
        while (work->reserved1 != 0) {
            work = (Extern989HandlerPtr)work->reserved1;
        }
        work->reserved1 = (UInt32)hand;
    } else {
        gExternHandlersList = hand;
    }

    return true;
}

/* 00000120 00000260 */ bool snd_UnregisterExternProcHandler(/* 0x0(sp) */ Extern989HandlerPtr hand) {
    /* -0x10(sp) */ Extern989HandlerPtr work;
    if (!snd_FindExternProcHandler(hand->proc_id)) {
        snd_ShowError(2, hand->proc_id, hand->proc_id, 0, 0);
        return false;
    }

    if (gExternHandlersList == hand) {
        gExternHandlersList = (Extern989HandlerPtr)hand->reserved1;
    } else {
        work = gExternHandlersList;
        while (work->reserved1 != (UInt32)hand) {
            work = (Extern989HandlerPtr)work->reserved1;
        }
        work->reserved1 = (UInt32)((Extern989HandlerPtr)work->reserved1)->reserved1;
    }

    return true;
}

/* 00000260 0000030c */ Extern989HandlerPtr snd_FindExternProcHandler(/* 0x0(sp) */ UInt32 id) {
    /* -0x10(sp) */ Extern989HandlerPtr work;
    for (work = gExternHandlersList; work != NULL; work = (Extern989HandlerPtr)work->reserved1) {
        if (work->proc_id == id) {
            return work;
        }
    }

    return NULL;
}

/* 0000030c 00000424 */ SInt32 snd_DoExternCall(/* 0x0(sp) */ UInt32 proc_id, /* 0x4(sp) */ SInt32 func_index, /* 0x8(sp) */ SInt32 arg1, /* 0xc(sp) */ SInt32 arg2, /* 0x10(sp) */ SInt32 arg3, /* 0x14(sp) */ SInt32 arg4, /* 0x18(sp) */ SInt32 arg5) {
    /* -0x10(sp) */ Extern989HandlerPtr work;
    work = snd_FindExternProcHandler(proc_id);
    if (work == NULL) {
        snd_ShowError(3, proc_id, proc_id, 0, 0);
        return 0;
    }

    if (func_index > work->num_funcs) {
        snd_ShowError(4, proc_id, 0, 0, 0);
    }

    return work->procs[func_index](arg1, arg2, arg3, arg4, arg5);
}

/* 00000424 00000534 */ SInt32 snd_DoExternCallWithData(/* 0x0(sp) */ UInt32 proc_id, /* 0x4(sp) */ SInt32 func_index, /* 0x8(sp) */ SInt32 data_size, /* 0xc(sp) */ void *data_ptr) {
    /* -0x10(sp) */ Extern989HandlerPtr work;
    work = snd_FindExternProcHandler(proc_id);
    if (work == NULL) {
        snd_ShowError(3, proc_id, proc_id, 0, 0);
        return 0;
    }

    if (func_index > work->num_funcs) {
        snd_ShowError(4, proc_id, 0, 0, 0);
    }

    return work->procs[func_index](data_size, (SInt32)data_ptr, 0, 0, 0);
}

/* 00000534 00000598 */ void snd_CMD_SL_INIT(/* 0x0(sp) */ SInt8 *msg_data) {
    /* -0x10(sp) */ UInt32 *data = msg_data;
    gEEStatusAddr = (char *)data[0];
    snd_StartSoundSystemEx(data[1]);
}

/* 00000598 000005cc */ void snd_CMD_SL_CLOSE(/* 0x0(sp) */ SInt8 *msg_data) {
    snd_StopSoundSystem();
}

/* 000005cc 00000630 */ void snd_CMD_SL_LOADBANK(/* 0x0(sp) */ SInt8 *msg_data) {
    *gWriteBackdataOffset = (UInt32)snd_BankLoadEx(&msg_data[4], *(SInt32 *)msg_data, 0, 0);
}

/* 00000630 000006a0 */ void snd_CMD_SL_LOADBANKBYLOC(/* 0x0(sp) */ SInt8 *msg_data) {
    /* -0x10(sp) */ SInt32 *data = msg_data;
    *gWriteBackdataOffset = (UInt32)snd_BankLoadByLocEx(data[0], data[1], 0, 0);
}

/* 000006a0 00000704 */ void snd_CMD_SL_BANKLOADFROMEE(/* 0x0(sp) */ SInt8 *msg_data) {
    /* -0x10(sp) */ UInt32 *data = msg_data;
    *gWriteBackdataOffset = (UInt32)snd_BankLoadFromEEEx(data[0], 0, 0);
}

/* 00000704 00000768 */ void snd_CMD_SL_BANKLOADFROMIOP(/* 0x0(sp) */ SInt8 *msg_data) {
    /* -0x10(sp) */ UInt32 *data = msg_data;
    *gWriteBackdataOffset = (UInt32)snd_BankLoadFromIOPEx((void *)data[0], 0, 0);
}

/* 00000768 000007c4 */ void snd_CMD_SL_LOADMMD(/* 0x0(sp) */ SInt8 *msg_data) {
    *gWriteBackdataOffset = (UInt32)snd_MMDLoad(&msg_data[4], *(SInt32 *)msg_data);
}

/* 000007c4 0000082c */ void snd_CMD_SL_LOADMMDBYLOC(/* 0x0(sp) */ SInt8 *msg_data) {
    /* -0x10(sp) */ SInt32 *data = msg_data;
    *gWriteBackdataOffset = (UInt32)snd_MMDLoadByLoc(data[0], data[1]);
}

/* 0000082c 0000087c */ void snd_CMD_SL_UNLOADBANK_A(/* 0x0(sp) */ SInt8 *msg_data) {
    snd_UnloadBank(*(SoundBankPtr *)msg_data);
    gWriteBackdataOffset = 0;
}

/* 0000087c 000008bc */ void snd_CMD_SL_UNLOADMIDI_A(/* 0x0(sp) */ SInt8 *msg_data) {
    snd_UnloadMMD(*(MultiMIDIBlockHeaderPtr *)msg_data);
}

/* 000008bc 000008f0 */ void snd_CMD_SL_RESOLVEBANKS(/* 0x0(sp) */ SInt8 *msg_data) {
    snd_ResolveBankXREFS();
}

/* 000008f0 00000948 */ void snd_CMD_SL_SETMASTERVOL_A(/* 0x0(sp) */ SInt8 *msg_data) {
    /* -0x10(sp) */ SInt32 *data = msg_data;
    snd_SetMasterVolume(data[0], data[1]);
}

/* 00000948 00000998 */ void snd_CMD_SL_GETMASTERVOL(/* 0x0(sp) */ SInt8 *msg_data) {
    *gWriteBackdataOffset = snd_GetMasterVolume(*(SInt32 *)msg_data);
}

/* 00000998 000009d8 */ void snd_CMD_SL_SETPLAYBACKMODE_A(/* 0x0(sp) */ SInt8 *msg_data) {
    snd_SetPlaybackMode(*(SInt32 *)msg_data);
}

/* 000009d8 00000a1c */ void snd_CMD_SL_GETPLAYBACKMODE(/* 0x0(sp) */ SInt8 *msg_data) {
    *gWriteBackdataOffset = snd_GetPlaybackMode();
}

/* 00000a1c 00000a74 */ void snd_CMD_SL_SETMIXERMODE_A(/* 0x0(sp) */ SInt8 *msg_data) {
    /* -0x10(sp) */ SInt32 *data = msg_data;
    snd_SetMixerMode(data[0], data[1]);
}

/* 00000a74 00000ab4 */ void snd_CMD_SL_SETREVERBMODE(/* 0x0(sp) */ SInt8 *msg_data) {
    snd_SetReverbMode(*(SInt32 *)msg_data);
}

/* 00000ab4 00000b1c */ void snd_CMD_SL_SETGROUPVOICERANGE(/* 0x0(sp) */ SInt8 *msg_data) {
    /* -0x10(sp) */ SInt32 *data = msg_data;
    snd_SetGroupVoiceRange(data[0], data[1], data[2]);
}

/* 00000b1c 00000b74 */ void snd_CMD_SL_SETREVERBTYPE(/* 0x0(sp) */ SInt8 *msg_data) {
    /* -0x10(sp) */ SInt32 *data = msg_data;
    snd_SetReverbType(data[0], data[1]);
}

/* 00000b74 00000bdc */ void snd_CMD_SL_SETREVERBDEPTH(/* 0x0(sp) */ SInt8 *msg_data) {
    /* -0x10(sp) */ SInt32 *data = msg_data;
    snd_SetReverbDepth(data[0], data[1], data[2]);
}

/* 00000bdc 00000c54 */ void snd_CMD_SL_AUTOVERB(/* 0x0(sp) */ SInt8 *msg_data) {
    /* -0x10(sp) */ SInt32 *data = msg_data;
    snd_AutoReverb(data[0], data[1], data[2], data[3]);
}

/* 00000c54 00000d0c */ void snd_CMD_SL_PLAYSOUND(/* 0x0(sp) */ SInt8 *msg_data) {
    /* -0x10(sp) */ SInt32 *data = (SInt32 *)msg_data;
    *gWriteBackdataOffset = snd_PlaySoundVolPanPMPB((SoundBankPtr)data[0], data[1], data[2], data[3], *(SInt16 *)&data[4], *((SInt16 *)(&data[4]) + 2));
}

/* 00000d0c 00000db4 */ void snd_CMD_SL_PLAYSOUND_A(/* 0x0(sp) */ SInt8 *msg_data) {
    /* -0x10(sp) */ SInt32 *data = msg_data;
    snd_PlaySoundVolPanPMPB((SoundBankPtr)data[0], data[1], data[2], data[3], *(SInt16 *)&data[4], *((SInt16 *)(&data[4]) + 2));
}

/* 00000db4 00000e08 */ void snd_CMD_SL_PLAYSOUNDEX(/* 0x0(sp) */ SInt8 *msg_data) {
    /* -0x10(sp) */ SndPlayParamsPtr params = msg_data;
    *gWriteBackdataOffset = snd_PlaySoundEx(params);
}

/* 00000e08 00000e48 */ void snd_CMD_SL_PAUSESOUND(/* 0x0(sp) */ SInt8 *msg_data) {
    snd_PauseSound(*(UInt32 *)msg_data);
}

/* 00000e48 00000e88 */ void snd_CMD_SL_CONTINUESOUND(/* 0x0(sp) */ SInt8 *msg_data) {
    snd_ContinueSound(*(UInt32 *)msg_data);
}

/* 00000e88 00000ec8 */ void snd_CMD_SL_STOPSOUND(/* 0x0(sp) */ SInt8 *msg_data) {
    snd_StopSound(*(UInt32 *)msg_data);
}

/* 00000ec8 00000f08 */ void snd_CMD_SL_PAUSEALLSOUNDS(/* 0x0(sp) */ SInt8 *msg_data) {
    snd_PauseAllSoundsInGroup(*(UInt32 *)msg_data);
}

/* 00000f08 00000f48 */ void snd_CMD_SL_CONTINUEALLSOUNDS(/* 0x0(sp) */ SInt8 *msg_data) {
    snd_ContinueAllSoundsInGroup(*(UInt32 *)msg_data);
}

/* 00000f48 00000f7c */ void snd_CMD_SL_STOPALLSOUNDS(/* 0x0(sp) */ SInt8 *msg_data) {
    snd_StopAllSounds();
}

/* 00000f7c 00000fbc */ void snd_CMD_SL_STOPALLSOUNDSINGROUP(/* 0x0(sp) */ SInt8 *msg_data) {
    snd_StopAllSoundsInGroup(*(UInt32 *)msg_data);
}

/* 00000fbc 0000100c */ void snd_CMD_SL_SOUNDISSTILLPLAYING(/* 0x0(sp) */ SInt8 *msg_data) {
    *gWriteBackdataOffset = snd_SoundIsStillPlaying(*(UInt32 *)msg_data);
}

/* 0000100c 00001074 */ void snd_CMD_SL_ISSOUNDALOOPER(/* 0x0(sp) */ SInt8 *msg_data) {
    /* -0x10(sp) */ SInt32 *data = msg_data;
    *gWriteBackdataOffset = snd_IsSoundALooper((SoundBankPtr)data[0], data[1]);
}

/* 00001074 000010dc */ void snd_CMD_SL_SETSOUNDVOLPAN_A(/* 0x0(sp) */ SInt8 *msg_data) {
    /* -0x10(sp) */ SInt32 *data = msg_data;
    snd_SetSoundVolPan(data[0], data[1], data[2]);
}

/* 000010dc 00001144 */ void snd_CMD_SL_GETSOUNDORIGPITCH(/* 0x0(sp) */ SInt8 *msg_data) {
    /* -0x10(sp) */ SInt32 *data = msg_data;
    *gWriteBackdataOffset = snd_GetSoundOriginalPitch((SoundBankPtr)data[0], data[1]);
}

/* 00001144 00001194 */ void snd_CMD_SL_GETSOUNDCURRPITCH(/* 0x0(sp) */ SInt8 *msg_data) {
    *gWriteBackdataOffset = snd_GetSoundCurrentPitch(*(UInt32 *)msg_data);
}

/* 00001194 000011ec */ void snd_CMD_SL_SETSOUNDPITCH_A(/* 0x0(sp) */ SInt8 *msg_data) {
    /* -0x10(sp) */ SInt32 *data = msg_data;
    snd_SetSoundPitch(data[0], data[1]);
}

/* 000011ec 00001244 */ void snd_CMD_SL_SETSOUNDPITCHBEND_A(/* 0x0(sp) */ SInt8 *msg_data) {
    /* -0x10(sp) */ SInt32 *data = msg_data;
    snd_SetSoundPitchBend(data[0], data[1]);
}

/* 00001244 0000129c */ void snd_CMD_SL_SETSOUNDPITCHMOD_A(/* 0x0(sp) */ SInt8 *msg_data) {
    /* -0x10(sp) */ SInt32 *data = msg_data;
    snd_SetSoundPitchModifier(data[0], data[1]);
}

/* 0000129c 00001354 */ void snd_CMD_SL_SETSOUNDPARAMS(/* 0x0(sp) */ SInt8 *msg_data) {
    /* -0x10(sp) */ SInt32 *data = msg_data;
    *gWriteBackdataOffset = snd_SetSoundParams(data[0], data[1], data[2], data[3], *(SInt16 *)&data[4], *((SInt16 *)(&data[4]) + 2));
}

/* 00001354 000013d0 */ void snd_CMD_SL_AUTOVOL_A(/* 0x0(sp) */ SInt8 *msg_data) {
    /* -0x10(sp) */ SInt32 *data = msg_data;
    snd_AutoVol(data[0], *(SInt16 *)&data[1], *((SInt16 *)(&data[1]) + 2), *(SInt16 *)&data[1]);
}

/* 000013d0 00001460 */ void snd_CMD_SL_AUTOPAN_A(/* 0x0(sp) */ SInt8 *msg_data) {
    /* -0x10(sp) */ SInt32 *data = msg_data;
    snd_AutoPan(data[0], data[1], data[2], data[3], data[4]);
}

/* 00001460 000014d8 */ void snd_CMD_SL_AUTOPITCH_A(/* 0x0(sp) */ SInt8 *msg_data) {
    /* -0x10(sp) */ SInt32 *data = msg_data;
    snd_AutoPitch(data[0], data[1], data[2], data[3]);
}

/* 000014d8 00001550 */ void snd_CMD_SL_AUTOPITCHBEND_A(/* 0x0(sp) */ SInt8 *msg_data) {
    /* -0x10(sp) */ SInt32 *data = msg_data;
    snd_AutoPitchBend(data[0], data[1], data[2], data[3]);
}

/* 00001550 00001594 */ void snd_CMD_SL_GETGLOBALEXCITE(/* 0x0(sp) */ SInt8 *msg_data) {
    *gWriteBackdataOffset = snd_GetGlobalExcite();
}

/* 00001594 000015d4 */ void snd_CMD_SL_SETGLOBALEXCITE_A(/* 0x0(sp) */ SInt8 *msg_data) {
    snd_SetGlobalExcite(*(UInt32 *)msg_data);
}

/* 000015d4 0000163c */ void snd_CMD_SL_GETMIDIREGISTER(/* 0x0(sp) */ SInt8 *msg_data) {
    /* -0x10(sp) */ SInt32 *data = msg_data;
    *gWriteBackdataOffset = snd_GetMIDIRegister(data[0], data[1]);
}

/* 0000163c 000016a4 */ void snd_CMD_SL_SETMIDIREGISTER_A(/* 0x0(sp) */ SInt8 *msg_data) {
    /* -0x10(sp) */ SInt32 *data = msg_data;
    snd_SetMIDIRegister(data[0], data[1], (SInt16)data[3]);
}

/* 000016a4 0000172c */ void snd_CMD_SL_INITVAGSTREAMINGEX(/* 0x0(sp) */ SInt8 *msg_data) {
    /* -0x10(sp) */ SInt32 *data = msg_data;
    *gWriteBackdataOffset = snd_InitVAGStreamingEx(data[0], data[1], data[2], data[3]) != 0;
}

/* 0000172c 00001794 */ void snd_CMD_SL_SETVAGSTREAMCHANNELRANGE(/* 0x0(sp) */ SInt8 *msg_data) {
    /* -0x10(sp) */ SInt32 *data = msg_data;
    snd_SetVAGStreamChannelRange(data[0], data[1], data[2]);
}

/* 00001794 0000180c */ void snd_CMD_SL_SETVAGSTREAMSUBGROUPVOLPAN(/* 0x0(sp) */ SInt8 *msg_data) {
    /* -0x10(sp) */ SInt32 *data = msg_data;
    snd_SetVAGStreamSubGroupVolPan(data[0], data[1], data[2], data[3]);
}

/* 0000180c 00001940 */ void snd_CMD_SL_PLAYVAGSTREAMBYNAME(/* 0x0(sp) */ SInt8 *msg_data) {
    /* -0x18(sp) */ char *name1 = NULL;
    /* -0x14(sp) */ char *name2 = NULL;
    /* -0x10(sp) */ struct PVSBN_struct *data = msg_data;

    if (data->name1[0] != 0) {
        name1 = data->name1;
    }

    if (data->name2[0] != 0) {
        name2 = data->name2;
    }

    *gWriteBackdataOffset = snd_PlayVAGStreamByNameEx(name1,
                                                      name2,
                                                      data->offset1,
                                                      data->offset2,
                                                      data->vol,
                                                      data->pan,
                                                      data->vol_group,
                                                      data->queue,
                                                      data->sub_group,
                                                      data->flags);
}

/* 00001940 00001a78 */ void snd_CMD_SL_PLAYVAGSTREAMBYLOC(/* 0x0(sp) */ SInt8 *msg_data) {
    /* -0x10(sp) */ SInt32 *data = msg_data;
    *gWriteBackdataOffset = snd_PlayVAGStreamByLocEx(data[0],
                                                     data[1],
                                                     data[2] & 0xFFFF,
                                                     data[3] & 0xFFFF,
                                                     ((UInt32)data[2]) >> 16,
                                                     ((UInt32)data[3]) >> 16,
                                                     data[4],
                                                     data[5],
                                                     data[6],
                                                     data[7]);
}

/* 00001a78 00001ab8 */ void snd_CMD_SL_PAUSEVAGSTREAM(/* 0x0(sp) */ SInt8 *msg_data) {
    snd_PauseVAGStream(*(UInt32 *)msg_data);
}

/* 00001ab8 00001af8 */ void snd_CMD_SL_CONTINUEVAGSTREAM(/* 0x0(sp) */ SInt8 *msg_data) {
    snd_ContinueVAGStream(*(UInt32 *)msg_data);
}

/* 00001af8 00001b38 */ void snd_CMD_SL_STOPVAGSTREAM(/* 0x0(sp) */ SInt8 *msg_data) {
    snd_StopVAGStream(*(UInt32 *)msg_data);
}

/* 00001b38 00001b88 */ void snd_CMD_SL_GETVAGSTREAMQUEUECOUNT(/* 0x0(sp) */ SInt8 *msg_data) {
    *gWriteBackdataOffset = snd_GetVAGStreamQueueCount(*(UInt32 *)msg_data);
}

/* 00001b88 00001bd8 */ void snd_CMD_SL_GETVAGSTREAMLOC(/* 0x0(sp) */ SInt8 *msg_data) {
    *gWriteBackdataOffset = snd_GetVAGStreamLoc(*(UInt32 *)msg_data);
}

/* 00001bd8 00001c28 */ void snd_CMD_SL_GETVAGSTREAMTIMEREMAINING(/* 0x0(sp) */ SInt8 *msg_data) {
    *gWriteBackdataOffset = snd_GetVAGStreamTimeRemaining(*(UInt32 *)msg_data);
}

/* 00001c28 00001c78 */ void snd_CMD_SL_GETVAGSTREAMTOTALTIME(/* 0x0(sp) */ SInt8 *msg_data) {
    *gWriteBackdataOffset = snd_GetVAGStreamTotalTime(*(UInt32 *)msg_data);
}

/* 00001c78 00001cc8 */ void snd_CMD_SL_ISVAGSTREAMBUFFERED(/* 0x0(sp) */ SInt8 *msg_data) {
    *gWriteBackdataOffset = snd_IsVAGStreamBuffered(*(UInt32 *)msg_data);
}

/* 00001cc8 00001cfc */ void snd_CMD_SL_STOPALLVAGSTREAMS(/* 0x0(sp) */ SInt8 *msg_data) {
    snd_StopAllStreams();
}

/* 00001cfc 00001d30 */ void snd_CMD_SL_CLOSEVAGSTREAMING(/* 0x0(sp) */ SInt8 *msg_data) {
    snd_CloseVAGStreaming();
}

/* 00001d30 00001d80 */ void snd_CMD_SL_STREAMSAFECHECKCDIDLE(/* 0x0(sp) */ SInt8 *msg_data) {
    *gWriteBackdataOffset = snd_StreamSafeCheckCDIdle(*(SInt32 *)msg_data);
}

/* 00001d80 00001dc4 */ void snd_CMD_SL_STREAMSAFECDBREAK(/* 0x0(sp) */ SInt8 *msg_data) {
    *gWriteBackdataOffset = snd_StreamSafeCdBreak();
}

/* 00001dc4 00001e2c */ void snd_CMD_SL_STREAMSAFECDREAD(/* 0x0(sp) */ SInt8 *msg_data) {
    /* -0x10(sp) */ UInt32 *data = msg_data;
    snd_StreamSafeCdRead(data[0], data[1], (void *)data[2]);
}

/* 00001e2c 00001f84 */ void snd_CMD_SL_STREAMSAFECDSEARCHFILE(/* 0x0(sp) */ SInt8 *msg_data) {
    /* -0x58(sp) */ UInt32 ee_location = *(UInt32 *)msg_data;
    /* -0x54(sp) */ char *fname;
    /* -0x50(sp) */ sceCdlFILE cdf;
    /* -0x2c(sp) */ SInt32 ret;
    /* -0x28(sp) */ sceSifDmaData transData;
    /* -0x18(sp) */ SInt32 did;
    /* -0x14(sp) */ SInt32 dis;
    /* -0x10(sp) */ SInt32 intr_state;

    ret = snd_StreamSafeCdSearchFile(&cdf, (char *)((UInt32 *)msg_data)[1]);
    if (ret) {
        transData.src = &cdf;
        transData.dest = (void *)ee_location;
        transData.size = sizeof(cdf);
        transData.attr = 0;
        dis = CpuSuspendIntr(&intr_state);
        did = sceSifSetDmaIntr(&transData, 1, snd_EEDMADone, &gEEDMADoneSema);
        if (!dis) {
            CpuResumeIntr(intr_state);
        }
        if (did != 0) {
            while (sceSifDmaStat(did) >= 0) {
                WaitSema(gEEDMADoneSema);
            }
        } else {
            ret = 0;
        }
    }

    *gWriteBackdataOffset = ret;
}

/* 00001f84 00001fe0 */ void snd_CMD_SL_ALLOCIOPRAM(/* 0x0(sp) */ SInt8 *msg_data) {
    *gWriteBackdataOffset = (UInt32)gAllocProc(*(UInt32 *)msg_data, 7, 0);
}

/* 00001fe0 00002024 */ void snd_CMD_SL_FREEIOPRAM(/* 0x0(sp) */ SInt8 *msg_data) {
    gFreeProc(*(void **)msg_data);
}

/* 00002024 000020dc */ void snd_CMD_SL_INITMOVIESOUND(/* 0x0(sp) */ SInt8 *msg_data) {
    /* -0x10(sp) */ SInt32 *data = msg_data;
    *gWriteBackdataOffset = (UInt32)snd_InitMovieSoundEx(data[0], data[1], data[2], data[3], data[4], data[5]);
}

/* 000020dc 00002110 */ void snd_CMD_SL_CLOSEMOVIESOUND(/* 0x0(sp) */ SInt8 *msg_data) {
    snd_CloseMovieSound();
}

/* 00002110 00002144 */ void snd_CMD_SL_RESETMOVIESOUND(/* 0x0(sp) */ SInt8 *msg_data) {
    snd_ResetMovieSound();
}

/* 00002144 000021d4 */ void snd_CMD_SL_STARTMOVIESOUND(/* 0x0(sp) */ SInt8 *msg_data) {
    /* -0x10(sp) */ SInt32 *data = msg_data;
    snd_StartMovieSoundEx((void *)data[0], data[1], data[2], data[3], data[4]);
}

/* 000021d4 00002218 */ void snd_CMD_SL_PAUSEMOVIESOUND(/* 0x0(sp) */ SInt8 *msg_data) {
    *gWriteBackdataOffset = snd_PauseMovieSound();
}

/* 00002218 00002270 */ void snd_CMD_SL_SETMOVIESOUNDVOLPAN(/* 0x0(sp) */ SInt8 *msg_data) {
    /* -0x10(sp) */ SInt32 *data = msg_data;
    snd_SetMovieSoundVolPan(data[0], data[1]);
}

/* 00002270 000022c8 */ void snd_CMD_SL_UPDATEMOVIEADPCM(/* 0x0(sp) */ SInt8 *msg_data) {
    /* -0x10(sp) */ SInt32 *data = msg_data;
    snd_UpdateMovieADPCM(data[0], data[1]);
}

/* 000022c8 0000230c */ void snd_CMD_SL_GETMOVIENAX(/* 0x0(sp) */ SInt8 *msg_data) {
    *gWriteBackdataOffset = snd_GetMovieNAX();
}

/* 0000230c 00002350 */ void snd_CMD_SL_GETTRANSSTATUS(/* 0x0(sp) */ SInt8 *msg_data) {
    *gWriteBackdataOffset = snd_GetMovieBufferPos();
}

/* 00002350 00002394 */ void snd_CMD_SL_GETSTICK(/* 0x0(sp) */ SInt8 *msg_data) {
    *gWriteBackdataOffset = snd_GetTick();
}

/* 00002394 00002488 */ void snd_CMD_SL_GETVOICEALLOC(/* 0x0(sp) */ SInt8 *msg_data) {
    /* -0x18(sp) */ SInt32 core = *(UInt32 *)msg_data;
    /* -0x14(sp) */ SInt32 x;
    /* -0x10(sp) */ SInt32 *SInt32_data = (SInt32 *)gWriteBackdataOffset;
    *SInt32_data = 0;
    for (x = 0; x < NUM_VOICE_PER_CORE; x++) {
        if (snd_GetVoiceStatus(24 * core + x)) {
            *SInt32_data |= 1 << x;
        }
    }
}

/* 00002488 000024d8 */ void snd_CMD_SL_LOCKVALLOC(/* 0x0(sp) */ SInt8 *msg_data) {
    *gWriteBackdataOffset = snd_LockVoiceAllocatorEx(false, 0x12345678);
}

/* 000024d8 00002540 */ void snd_CMD_SL_EXTERNVALLOC(/* 0x0(sp) */ SInt8 *msg_data) {
    /* -0x10(sp) */ SInt32 *data = msg_data;
    *gWriteBackdataOffset = snd_ExternVoiceAlloc(data[0], data[1]);
}

/* 00002540 00002580 */ void snd_CMD_SL_EXTERNVFREE(/* 0x0(sp) */ SInt8 *msg_data) {
    snd_ExternVoiceFree(*(UInt32 *)msg_data);
}

/* 00002580 000025b4 */ void snd_CMD_SL_UNLOCKVALLOC(/* 0x0(sp) */ SInt8 *msg_data) {
    snd_UnlockVoiceAllocator();
}

/* 000025b4 00002634 */ void snd_CMD_SL_SRAMMALLOC(/* 0x0(sp) */ SInt8 *msg_data) {
    /* -0x10(sp) */ SInt32 dis;
    /* -0xc(sp) */ SInt32 oldstat;
    dis = CpuSuspendIntr(&oldstat);
    *gWriteBackdataOffset = snd_SRAMMalloc(*(UInt32 *)msg_data);
    if (!dis) {
        CpuResumeIntr(oldstat);
    }
}

/* 00002634 000026cc */ void snd_CMD_SL_SRAMMARKUSED(/* 0x0(sp) */ SInt8 *msg_data) {
    /* -0x18(sp) */ SInt32 dis;
    /* -0x14(sp) */ SInt32 oldstat;
    /* -0x10(sp) */ UInt32 *data = msg_data;
    dis = CpuSuspendIntr(&oldstat);
    *gWriteBackdataOffset = snd_SRAMMarkUsed(data[0], data[1]);
    if (!dis) {
        CpuResumeIntr(oldstat);
    }
}

/* 000026cc 00002754 */ void snd_CMD_SL_SRAMFREE(/* 0x0(sp) */ SInt8 *msg_data) {
    /* -0x18(sp) */ SInt32 dis;
    /* -0x14(sp) */ SInt32 oldstat;
    /* -0x10(sp) */ UInt32 *data = msg_data;

    dis = CpuSuspendIntr(&oldstat);
    snd_SRAMFree(data[0], data[1]);
    if (!dis) {
        CpuResumeIntr(oldstat);
    }
}

/* 00002754 000027c8 */ void snd_CMD_SL_SRAMGETFREE(/* 0x0(sp) */ SInt8 *msg_data) {
    /* -0x10(sp) */ SInt32 dis;
    /* -0xc(sp) */ SInt32 oldstat;

    dis = CpuSuspendIntr(&oldstat);
    *gWriteBackdataOffset = snd_SRAMGetFreeMem();
    if (!dis) {
        CpuResumeIntr(oldstat);
    }
}

/* 000027c8 0000283c */ void snd_CMD_SL_SRAMMAXFREE(/* 0x0(sp) */ SInt8 *msg_data) {
    /* -0x10(sp) */ SInt32 dis;
    /* -0xc(sp) */ SInt32 oldstat;

    dis = CpuSuspendIntr(&oldstat);
    *gWriteBackdataOffset = snd_SRAMMaxFreeBlockSize();
    if (!dis) {
        CpuResumeIntr(oldstat);
    }
}

/* 0000283c 0000290c */ void snd_CMD_SL_EXTERNCALL(/* 0x0(sp) */ SInt8 *msg_data) {
    /* -0x10(sp) */ SInt32 *data = msg_data;
    *gWriteBackdataOffset = snd_DoExternCall(data[0], data[1], data[2], data[3], data[4], data[5], data[6]);
}

/* 0000290c 00002990 */ void snd_CMD_SL_EXTERNCALLWITHDATA(/* 0x0(sp) */ SInt8 *msg_data) {
    /* -0x10(sp) */ SInt32 *data = msg_data;
    *gWriteBackdataOffset = snd_DoExternCallWithData(data[0], data[1], data[2], (void *)data[3]);
}

/* 00002990 00002a20 */ void snd_CMD_SL_SETREVERBEX(/* 0x0(sp) */ SInt8 *msg_data) {
    /* -0x10(sp) */ SInt32 *data = msg_data;
    snd_SetReverbEx(data[0], data[1], data[2], data[3], data[4]);
}

/* 00002a20 00002a78 */ void snd_CMD_SL_PREALLOCREVERBWORKAREA(/* 0x0(sp) */ SInt8 *msg_data) {
    /* -0x10(sp) */ SInt32 *data = msg_data;
    snd_PreAllocReverbWorkArea(data[0], data[1]);
}

/* 00002a78 00002abc */ void snd_CMD_SL_GETLASTLOADERROR(/* 0x0(sp) */ SInt8 *msg_data) {
    *gWriteBackdataOffset = snd_GetLastLoadError();
}

/* 00002abc 00002afc */ void snd_CMD_SL_SETPANMODE(/* 0x0(sp) */ SInt8 *msg_data) {
    snd_SetPanMode(*(SInt32 *)msg_data);
}

/* 00002afc 00002b30 */ void snd_CMD_SL_PLAYSOUNDBYNAME(/* 0x0(sp) */ SInt8 *msg_data) {
    *gWriteBackdataOffset = 0;
}

/* 00002b30 00002b54 */ void snd_CMD_SL_PLAYSOUNDBYNAME_A(/* 0x0(sp) */ SInt8 *msg_data) {
}

/* 00002b54 00002cf0 */ void snd_CMD_SL_GETSOUNDUD(/* 0x0(sp) */ SInt8 *msg_data) {
    /* -0x40(sp) */ SInt32 return_hold[4];
    /* -0x30(sp) */ SInt32 ret;
    /* -0x2c(sp) */ SInt32 intr_state;
    /* -0x28(sp) */ SInt32 dis;
    /* -0x24(sp) */ struct GetSoundUserDataCommandStruct *data = msg_data;
    /* -0x20(sp) */ SInt32 did;
    /* -0x1c(sp) */ sceSifDmaData transData;

    ret = snd_GetSoundUserData((SoundBankPtr)data->bank, (char *)data->bank_name, data->snd_index, (char *)data->snd_name, return_hold);
    if (ret) {
        transData.src = return_hold;
        transData.dest = data->destination;
        transData.size = sizeof(return_hold);
        transData.attr = 0;
        dis = CpuSuspendIntr(&intr_state);
        if (!did) {
            snd_ShowError(99, 0, 0, 0, 0);
            *gWriteBackdataOffset = 0;
            return;
        }
        if (!dis) {
            CpuResumeIntr(intr_state);
        }

        while (sceSifDmaStat(did) >= 0) {
            WaitSema(gEEDMADoneSema);
        }
    }

    *gWriteBackdataOffset = ret;
}

/* 00002cf0 00002d58 */ void snd_CMD_SL_GETSOUNDREG(/* 0x0(sp) */ SInt8 *msg_data) {
    /* -0x10(sp) */ UInt32 *data = msg_data;
    *gWriteBackdataOffset = snd_GetSoundReg(data[0], data[1]);
}

/* 00002d58 00002dc0 */ void snd_CMD_SL_SETSOUNDREG(/* 0x0(sp) */ SInt8 *msg_data) {
    /* -0x10(sp) */ UInt32 *data = msg_data;
    snd_SetSoundReg(data[0], data[1], data[2]);
}

/* 00002dc0 00002e1c */ void snd_CMD_SL_GETSFXGLOBALREG(/* 0x0(sp) */ SInt8 *msg_data) {
    /* -0x10(sp) */ SInt32 *data = msg_data;
    *gWriteBackdataOffset = snd_GetSFXGlobalReg(data[0]);
}

/* 00002e1c 00002e74 */ void snd_CMD_SL_SETSFXGLOBALREG(/* 0x0(sp) */ SInt8 *msg_data) {
    /* -0x10(sp) */ SInt32 *data = msg_data;
    snd_SetSFXGlobalReg(data[0], data[1]);
}

/* 00002e74 00002ecc */ void snd_CMD_SL_SETALLSOUNDREG(/* 0x0(sp) */ SInt8 *msg_data) {
    /* -0x10(sp) */ UInt32 *data = msg_data;
    snd_SetAllSoundReg(msg_data[0], &msg_data[1]);
}

/* 00002ecc 00002f48 */ void snd_CMD_SL_SETMASTERVOLUMEDUCKER(/* 0x0(sp) */ SInt8 *msg_data) {
    /* -0x10(sp) */ SInt32 *data = (SInt32 *)msg_data;
    /* -0xc(sp) */ DuckerDefPtr state = &data[1];

    if (data[1] == -1) {
        state = NULL;
    }

    snd_SetMasterVolumeDucker(*data, state);
}

/* 00002f48 00002f8c */ void snd_CMD_SL_CDSTATUS(/* 0x0(sp) */ SInt8 *msg_data) {
    *(SInt32 *)gWriteBackdataOffset = sceCdStatus();
}

/* 00002f8c 00003120 */ void snd_CMD_SL_COMMAND_BATCH(/* 0x0(sp) */ SInt8 *msg_data) {
    /* -0x18(sp) */ SndCommandBufferPtr batch = (SndCommandBufferPtr)msg_data;
    /* -0x14(sp) */ char *command_buffer_walk;
    /* -0x10(sp) */ SInt32 x;
    /* -0xc(sp) */ SInt16 command;
    /* -0xa(sp) */ SInt16 size;

    command_buffer_walk = batch->buffer;
    for (x = 0; x < batch->num_commands; x++) {
        SndCommandEntryPtr entry = command_buffer_walk;
        command = entry->command;
        size = entry->size;
        command_buffer_walk += sizeof(struct SndCommandEntry);

        if ((size & 3) != 0) {
            size = 4 - -4 * (size / 4);
        }

        gCommandFunc[command](command_buffer_walk);
        command_buffer_walk += size;
        gWriteBackdataOffset += 4;
    }

    gWriteBackdataOffset -= 4;
}

/* 00003120 000031c8 */ static void *snd_EEMessageParser(/* 0x0(sp) */ UInt32 command, /* 0x4(sp) */ void *data, /* 0x8(sp) */ SInt32 size) {
    gWriteBackdataOffset = &snd_MESSAGE_RETURN_BUFFER[1];
    gCommandFunc[command](data);
    gWriteBackdataOffset[1] = -1;
    return snd_MESSAGE_RETURN_BUFFER;
}

/* 000031c8 00003274 */ SInt32 snd_StartEEMessaging() {
    /* -0x68(sp) */ sceSifQueueData qd;
    /* -0x50(sp) */ sceSifServeData sd;

    *(SInt32 *)snd_MESSAGE_RECIEVE_BUFFER = -1;
    sceSifInitRpc(0);
    sceSifSetRpcQueue(&qd, GetThreadId());
    sceSifRegisterRpc(&sd, 0x123456, snd_EEMessageParser, snd_MESSAGE_RECIEVE_BUFFER, NULL, NULL, &qd);
    sceSifRpcLoop(&qd);

    return 0;
}

/* 00003274 00003454 */ static void *snd_EELoaderMessageParser(/* 0x0(sp) */ UInt32 command, /* 0x4(sp) */ void *data, /* 0x8(sp) */ SInt32 size) {
    /* -0x10(sp) */ SInt32 ret_val = 0;

    // added
    SndMessageDataPtr msg = data;

    switch (command) {
    case SL_LOADBANK:
        ret_val = (SInt32)snd_BankLoadEx((char *)&msg->data[1], msg->data[0], 0, 0);
        break;
    case SL_LOADBANKBYLOC:
        ret_val = (SInt32)snd_BankLoadByLocEx(msg->data[0], msg->data[1], 0, 0);
        break;
    case SL_LOADMMD:
        ret_val = (SInt32)snd_MMDLoad((SInt8 *)&msg->data[1], msg->data[0]);
        break;
    case SL_LOADMMDBYLOC:
        ret_val = (SInt32)snd_MMDLoadByLoc(msg->data[0], msg->data[1]);
        break;
    case SL_BANKLOADFROMEE:
        ret_val = (SInt32)snd_BankLoadFromEEEx(msg->data[0], 0, 0);
        break;
    case SL_BANKLOADFROMIOP:
        ret_val = (SInt32)snd_BankLoadFromIOPEx((void *)&msg->data[0], 0, 0);
        break;
    }

    snd_LOADER_MESSAGE_RETURN_BUFFER[0] = (UInt32)ret_val;
    return snd_LOADER_MESSAGE_RETURN_BUFFER;
}

/* 00003454 000034f0 */ SInt32 snd_StartEELoaderMessaging() {
    /* -0x68(sp) */ sceSifQueueData qd;
    /* -0x50(sp) */ sceSifServeData sd;

    sceSifInitRpc(0);
    sceSifSetRpcQueue(&qd, GetThreadId());
    sceSifRegisterRpc(&sd, 0x123457, snd_EELoaderMessageParser, snd_LOADER_MESSAGE_RECIEVE_BUFFER, NULL, NULL, &qd);
    sceSifRpcLoop(&qd);

    return 0;
}

/* 000034f0 0000386c */ SInt32 _start(/* 0x0(sp) */ SInt32 argc, /* 0x4(sp) */ SInt8 **argv) {
    /* -0x48(sp) */ SemaParam sema;
    /* -0x38(sp) */ ThreadParam param;
    /* -0x24(sp) */ SInt32 th;
    /* -0x20(sp) */ SInt32 x;
    /* -0x1c(sp) */ SInt32 dis;
    /* -0x18(sp) */ SInt32 oldstat;
    /* -0x14(sp) */ int i;
    /* -0x10(sp) */ char *walk;

#ifdef INTERNAL_SNDERR
    snd_RegisterErrorDisplayFunc(snderr_DisplayError);
#endif

    for (x = 1; x < argc; x++) {
        snd_ParseCommandLineArg(argv[x]);
    }

    InitResidentModule();
    snd_DumpVersionAndInfo();
    if (!sceSifCheckInit()) {
        sceSifInit();
    }

    sceSifInitRpc(0);

    if (!gDoRPC) {
        return 0;
    }

    dis = CpuSuspendIntr(&oldstat);
    snd_MESSAGE_RECIEVE_BUFFER = AllocSysMemory(0, 0x1000, 0);
    if (!snd_MESSAGE_RECIEVE_BUFFER) {
        snd_ShowError(5, 0, 0, 0, 0);
        return 1;
    }

    for (i = 0, walk = snd_MESSAGE_RECIEVE_BUFFER; i < 0x1000; i++, walk++) {
        *walk = -85;
    }

    snd_MESSAGE_RETURN_BUFFER = AllocSysMemory(0, 0x420, 0);
    if (!snd_MESSAGE_RETURN_BUFFER) {
        snd_ShowError(6, 0, 0, 0, 0);
        return 1;
    }

    if (!dis) {
        CpuResumeIntr(oldstat);
    }

    gWriteBackdataOffset = snd_MESSAGE_RETURN_BUFFER + 4;

    sema.attr = SA_THFIFO;
    sema.initial = 0;
    sema.max = 1;

    gEEDMADoneSema = CreateSema(&sema);
    gFileReadMutex = CreateSema(&sema);
    SignalSema(gFileReadMutex);

    param.attr = TH_C;
    param.thread = snd_StartEEMessaging;
    param.priority = gThreadPriority_RPC;
    param.stacksize = 0x1000;
    param.option = 0;

    gMainRPCThreadId = CreateThread(&param);
    if (gMainRPCThreadId <= 0) {
        return 1;
    }

    StartThread(gMainRPCThreadId, 0);

    param.attr = TH_C;
    param.thread = snd_StartEELoaderMessaging;
    param.priority = gThreadPriority_RPC + 1;
    param.stacksize = 0x800;
    param.option = 0;

    th = CreateThread(&param);
    if (th <= 0) {
        return 1;
    }

    StartThread(th, 0);

    return 0;
}

/* 0000386c 00003a10 */ void snd_ParseCommandLineArg(/* 0x0(sp) */ char *arg) {
    if (!strncmp(arg, "tick_priority", 13)) {
        gThreadPriority_TICK = snd_GetVal(arg + 13);
    } else if (!strncmp(arg, "rpc_priority", 12)) {
        gThreadPriority_RPC = snd_GetVal(arg + 12);
    } else if (!strncmp(arg, "stream_priority", 15)) {
        gThreadPriority_STREAM = snd_GetVal(arg + 15);
    } else if (!strncmp(arg, "do_rpc", 6)) {
        gDoRPC = snd_GetVal(arg + 6) == 1;
    } else if (!strncmp(arg, "limit_2meg", 10)) {
        gLimit2Meg = true;
    } else {
        snd_ShowError(7, (int)arg, 0, 0, 0);
    }
}

/* 00003a10 00003b1c */ SInt32 snd_GetVal(/* 0x0(sp) */ char *st) {
    while (*st != 45 && (*st < 48 || *st >= 58) && *st) {
        st++;
    }

    if (*st) {
        return strtol(st, NULL, 10);
    }

    return 0;
}

/* 00003b1c 00003cf4 */ void snd_DumpVersionAndInfo() {
    printf("\n");
    printf("==========================================================\n");
    printf(" 989snd (c)2000-2003 Sony Computer Entertainment America\n");
    printf(" by Buzz Burrowes                               v%d.%d.%d\n", 3, 1, 2);
    printf("                        (built %s at %s)\n", "Nov 19 2003", "14:31:04");
    printf(" (build with sce library rev %d.%d.%d)\n", 2, 7, 0);
    printf("==========================================================\n");
    printf("    Thread Priorities:\n");
    printf("                         Sound Tick = %d\n", gThreadPriority_TICK);
    printf("                           Main RPC = %d\n", gThreadPriority_RPC);
    printf("                        Loading RPC = %d\n", gThreadPriority_RPC + 1);
    printf(
        "                          Streaming = %d, %d, %d, %d\n",
        gThreadPriority_STREAM,
        gThreadPriority_STREAM + 1,
        gThreadPriority_STREAM + 2,
        gThreadPriority_STREAM + 3);
    printf("   Memory Locations...\n");
    printf("       Voice Allocator Owner  : 0x%8.8x\n", &gVAllocOwnerID);
    printf("       Master Tick Owner List : 0x%8.8x\n", gMasterTickOwner);
    printf("==========================================================\n");
    printf("\n");
}

/* 00003cf4 00003d24 */ void snd_RegisterErrorDisplayFunc(/* 0x0(sp) */ SndErrorDisplayFunc func) {
    gErrorDisplayFunc = func;
}

/* 00003d24 00003dec */ void snd_ShowError(/* 0x0(sp) */ SInt32 num, /* 0x4(sp) */ UInt32 a1, /* 0x8(sp) */ UInt32 a2, /* 0xc(sp) */ UInt32 a3, /* 0x10(sp) */ UInt32 a4) {
    if (gErrorDisplayFunc) {
        gErrorDisplayFunc(num, a1, a2, a3, a4);
    } else if (!gPrefs_Silent) {
        printf("989snd Error: cause %d -> %u, %u, %u, %u\n", num, a1, a2, a3, a4);
    }
}

/* 00003dec 00003e2c */ void snd_EEDMADone(/* 0x0(sp) */ SInt32 *sema_id_ptr) {
    iSignalSema(*sema_id_ptr);
}

/* 00003e2c 00003e5c */ void snd_Install989Monitor(/* 0x0(sp) */ struct Extern989MonitorInfo *mon) {
    g989Monitor = mon;
}
