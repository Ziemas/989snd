#include "init.h"
#include "989snd.h"
#include "intr_code.h"
#include "loader.h"
#include "moviesnd.h"
#include "reverb.h"
#include "sndhand.h"
#include "sram.h"
#include "stick.h"
#include "stream.h"
#include "types.h"
#include "valloc.h"
#include "vol.h"

#include <libsd.h>
#include <xtimrman.h>

/* data 12e0 */ SInt32 gSTickSema = -1;
/* data 12e4 */ SInt32 gTimerThreadID = 0;
/* data 12e8 */ SInt32 gTimerID = 0;
/* data 12ec */ SInt32 gTickTrigger = 0;
/* data 12f0 */ SInt32 gTickMode = -1;
/* data 12f4 */ UInt32 gMicsPerTick = 0;
/* data 12f8 */ SInt32 gTickRate = 0;
/* data 12fc */ SInt32 gPercent = 0;
/* data 1300 */ bool gPrefs_ShowMissingSounds = true;
/* data 1304 */ bool gPrefs_Silent = false;
/* bss 0 */ struct timercommon gTimerInfo;

/* 0000f7c8 0000f808 */ SInt32 snd_SystemRunning() {
    return gTickMode != -1;
}

/* 0000f808 0000fb64 */ SInt32 snd_StartSoundSystemEx(/* 0x0(sp) */ UInt32 flags) {
    /* -0x18(sp) */ SInt32 ret;
    /* -0x14(sp) */ SInt32 i;
    /* -0x10(sp) */ UInt16 val;

    if (gTickMode != -1) {
        return -1;
    }

    gPrefs_ShowMissingSounds = (flags & 1) == 0;
    gPrefs_Silent = (flags & 2) != 0;
    snd_SRAMInitMalloc();
    snd_InitLoader();
    snd_InitVoiceAllocator();
    snd_InitSoundHandlers();
    snd_InitDuckers();

    if ((flags & 4) == 0) {
        sceSdInit(0);
        sceSdSetParam(SD_PARAM_MMIX, 0xf00);
        sceSdSetParam(SD_PARAM_MMIX | 1, 0xf0c);
        sceSdSetParam(SD_PARAM_MVOLL, 0x3fff);
        sceSdSetParam(SD_PARAM_MVOLR, 0x3fff);
        sceSdSetParam(SD_PARAM_EVOLL, 0);
        sceSdSetParam(SD_PARAM_EVOLR, 0);
        sceSdSetParam(SD_PARAM_EVOLL | 1, 0);
        sceSdSetParam(SD_PARAM_EVOLR | 1, 0);
        sceSdSetParam(SD_PARAM_AVOLL | 1, 0x7fff);
        sceSdSetParam(SD_PARAM_AVOLR | 1, 0x7fff);
        val = 0;
        if ((flags & 8) != 0) {
            val = 0x800;
        }
        if ((flags & 0x10) == 0) {
            val |= 0x80;
        }
        sceSdSetCoreAttr(SD_CORE_SPDIF_MODE, val);
        sceSdSetCoreAttr(SD_CORE_EFFECT_ENABLE, 0);
        sceSdSetCoreAttr(SD_CORE_EFFECT_ENABLE | 1, 0);
    }

    gTickMode = 1;
    gTickRate = snd_GetTickRate();
    gMicsPerTick = 100000000 / (999 * gTickRate / 10);

    ret = snd_InstallSoundTick();
    if (ret) {
        gTickMode = -1;
        return ret;
    }

    snd_SetPlaybackMode(0);
    for (i = 0; i < 15; i++) {
        snd_SetMasterVolume(i, 0x400);
    }
    snd_SetMasterVolume(16, 0x400);
    gLockMasterTick = 0;

    return 0;
}

/* 0000fb64 0000fd00 */ SInt32 snd_InstallSoundTick() {
    /* -0x38(sp) */ int ret;
    /* -0x34(sp) */ SysClock clock;
    /* -0x2c(sp) */ ThreadParam param;
    /* -0x18(sp) */ SemaParam sema;

    sema.attr = 1;
    sema.initial = 0;
    sema.max = 1;
    gSTickSema = CreateSema(&sema);

    param.attr = TH_C;
    param.thread = snd_TimerLLIRQLoop;
    param.priority = gThreadPriority_TICK;
    param.stacksize = 0x1000;
    param.option = 0;
    gTimerThreadID = CreateThread(&param);

    gKillTimer = false;
    USec2SysClock(1000000, &clock);
    gTimerInfo.thid = &gTimerThreadID;
    gTimerInfo.compare = clock.lo / 0x0f0;
    gTimerID = AllocHardTimer(1, 32, 1);
    SetTimerHandler(gTimerID, gTimerInfo.compare, snd_TimerLLTick, &gTimerInfo);
    SetupHardTimer(gTimerID, 1, 0, 1);
    StartHardTimer(gTimerID);
    if (gTimerThreadID > 0) {
        StartThread(gTimerThreadID, 0);
    }

    return 0;
}

/* 0000fd00 0000fda8 */ void snd_RemoveSoundTick() {
    ///* -0x10(sp) */ SInt32 ret;
    TerminateThread(gTimerThreadID);
    DeleteThread(gTimerThreadID);
    gKillTimer = 1;
    while (gKillTimer) {
        DelayThread(33000);
    }
    StopHardTimer(gTimerID);
    FreeHardTimer(gTimerID);
}

/* 0000fda8 0000fed8 */ SInt32 snd_StopSoundSystem() {
    gLockMasterTick = 1;
    snd_UnloadAllBanks();
    snd_UnloadAllMMD();
    snd_CleanupVoiceAllocator();
    snd_SetReverbDepth(3, 0, 0);
    snd_SetReverbType(3, 0);
    snd_CloseMovieSound();
    snd_CloseVAGStreaming();
    gAwaitingKeyOff[0] = -1;
    gAwaitingKeyOff[1] = -1;
    snd_StopPendingVoices();
    snd_RemoveSoundTick();
    sceSdSetSwitch(SD_SWITCH_VMIXL, 0);
    sceSdSetSwitch(SD_SWITCH_VMIXR, 0);
    sceSdSetSwitch(SD_SWITCH_VMIXL | 1, 0);
    sceSdSetSwitch(SD_SWITCH_VMIXR | 1, 0);
    snd_SetMasterVolume(16, 0);
    snd_WaitDMADone();
    snd_ShutdownLoader();
    gLockMasterTick = 0;
    gTickMode = -1;
    return 0;
}

/* 0000fed8 0000ff04 */ SInt32 snd_GetTickRate() {
    return 240;
}

/* 0000ff04 0000ff74 */ SInt32 snd_TimerLLIRQLoop() {
    while (1) {
        SleepThread();
        snd_SoundTickHandler();
        SignalSema(gSTickSema);
    }
}
