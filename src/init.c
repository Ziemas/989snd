#include "types.h"

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
}

/* 0000fb64 0000fd00 */ SInt32 snd_InstallSoundTick() {
    /* -0x38(sp) */ int ret;
    /* -0x34(sp) */ SysClock clock;
    /* -0x2c(sp) */ ThreadParam param;
    /* -0x18(sp) */ SemaParam sema;
}

/* 0000fd00 0000fda8 */ void snd_RemoveSoundTick() {
    /* -0x10(sp) */ SInt32 ret;
}

/* 0000fda8 0000fed8 */ SInt32 snd_StopSoundSystem() {}
/* 0000fed8 0000ff04 */ SInt32 snd_GetTickRate() {}
/* 0000ff04 0000ff74 */ SInt32 snd_TimerLLIRQLoop() {}
