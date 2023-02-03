#include "989snd.h"
#include "common.h"
#include "util.h"

#include <kernel.h>
#include <libcdvd-common.h>
#include <sifcmd.h>
#include <sifrpc.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

/* data 2c0450 */ unsigned int *gCommBusy;
/* data 2c0454 */ int gAwaitingInts;
/* data 2c0458 */ int gLocalLoadError;
/* data 2c045c */ int gStreamingInited;
/* data 2c0460 */ void (*gCdCallback)(int);
/* data 2c0464 */ int gSSRead;
/* data 2c0468 */ int gSSReadDone;
/* data 2c046c */ int gLoadingFromFS = 0;
/* data 2c0470 */ SndCommandBufferPtr gSndCommandBuffePtr[2];
/* data 2c0478 */ int gCommandBuffeBytesAvail[2];
/* data 2c0480 */ SndCommandReturnDefPtr gSndCommandReturnDefPtr[2];
/* data 2c0488 */ unsigned int *gReturnValuesPtr[2];
/* data 2c0490 */ int gCommandFillBuffer;
/* data 2c0494 */ int gCaching;
/* data 2c0498 */ unsigned int gLoadBusy = 0;
/* data 2c04a0 */ SndCommandReturnDef gLoadReturnDef;
/* data 2c04b0 */ sceCdlFILE *gSFStructPtr;
/* data 2c04b4 */ static SndCompleteProc gLoadCB;
/* data 2c04b8 */ static unsigned long long gLoadUserData;
/* data 2c04c0 */ unsigned int gLoadCommand;
/* data 2c04c4 */ int gLoadAwaits;
/* data 2c04c8 */ int gPrefs_Silent = 0;
/* bss 356a00 */ static SifRpcClientData_t gSLClientData;
/* bss 356a40 */ unsigned int gSyncBuffer[16] __attribute__((aligned(64)));
/* bss 356a80 */ char gSyncSendBuffer[512] __attribute__((aligned(64)));
/* bss 356c80 */ char gSyncStringBuffer[256] __attribute__((aligned(64)));
/* bss 356d80 */ SndCommandBuffer gSndCommandBuffer1 __attribute__((aligned(64)));
/* bss 357d80 */ SndCommandBuffer gSndCommandBuffer2 __attribute__((aligned(64)));
/* bss 358d80 */ SndCommandReturnDef gSndCommandReturnDef1[256];
/* bss 359d80 */ SndCommandReturnDef gSndCommandReturnDef2[256];
/* bss 35ad80 */ unsigned int gActualReturnValues1[264] __attribute__((aligned(64)));
/* bss 35b1c0 */ unsigned int gActualReturnValues2[264] __attribute__((aligned(64)));
/* bss 35b600 */ SndSystemStatus gStats;
/* bss 35b640 */ static SifRpcClientData_t gSLClientLoaderData;
/* bss 35b680 */ unsigned int gLoadReturnValue[4] __attribute__((aligned(64)));
/* bss 35b6c0 */ int gLoadParams[8];
/* bss 35b700 */ char gSFBuffer[64];

#define CMD_RPC_ID 0x123456
#define LOADER_RPC_ID 0x123457

unsigned int snd_SendIOPCommandAndWait(int command, int data_size, char *data);
void snd_SendIOPCommandNoWait(int command, int data_size, char *data, SndCompleteProc done, unsigned long long u_data);
void snd_PostMessage();
int snd_StreamSafeCdSync(int mode);
int snd_GotReturns();
void snd_SendCurrentBatch();

/* 001aa8a8 001aab04 */ void snd_StartSoundSystemEx(/* s7 23 */ unsigned int flags) {
    /* v0 2 */ int i;
    /* -0xb0(sp) */ int data[2];

    gSndCommandBuffePtr[0] = &gSndCommandBuffer1;
    gSndCommandBuffePtr[1] = &gSndCommandBuffer2;
    gReturnValuesPtr[0] = gActualReturnValues1;
    gReturnValuesPtr[1] = gActualReturnValues2;
    gSndCommandReturnDefPtr[0] = gSndCommandReturnDef1;
    gSndCommandReturnDefPtr[1] = gSndCommandReturnDef2;
    gPrefs_Silent = (flags & 2) != 0;

    SifInitRpc(0);

    do {
        if (SifBindRpc(&gSLClientData, CMD_RPC_ID, 0) < 0) {
            printf("error: sceSifBindRpc in %s, at line %d\n", __FILE__, __LINE__);
            while (1)
                ;
        }
        i = 10000;
        while (i--)
            ;
    } while (gSLClientData.server == NULL);


    gLoadBusy = 0;
    gLoadReturnDef.done = NULL;
    gLoadReturnDef.u_data = 0;
    gLoadReturnValue[0] = 0;

    do {
        if (SifBindRpc(&gSLClientLoaderData, LOADER_RPC_ID, 0) < 0) {
            printf("error: sceSifBindRpc in %s, at line %d\n", __FILE__, __LINE__);
            while (1)
                ;
        }
        i = 10000;
        while (i--)
            ;
    } while (gSLClientLoaderData.server == NULL);

    gCommandBuffeBytesAvail[1] = 4096;
    gCommandBuffeBytesAvail[0] = 4096;
    gSndCommandBuffer1.num_commands = 0;
    gSndCommandBuffer2.num_commands = 0;

    gStats.cd_busy = 0;
    gStats.cd_error = 0;

    data[0] = (int)&gStats;
    data[1] = flags;

    snd_SendIOPCommandAndWait(SL_INIT, sizeof(data), (char *)data);
}

/* 001aab08 001aad38 */ int snd_FlushSoundCommands() {
    /* v0 2 */ int which;
    /* s0 16 */ int x;
    /* a1 5 */ unsigned long u_data;

    if (gCommBusy != NULL && snd_GotReturns()) {
        if (gLoadingFromFS) {
            if (gLoadCB != NULL) {
                gLoadCB(gSyncBuffer[1], gLoadUserData);
            }
            gLoadCB = NULL;
            gLoadingFromFS = 0;
        } else {
            which = !gCommandFillBuffer;
            for (x = 0; x < gSndCommandBuffePtr[which]->num_commands; x++) {
                if (gSndCommandReturnDefPtr[which][x].done != NULL) {
                    u_data = gSndCommandReturnDefPtr[which][x].u_data;
                    gSndCommandReturnDefPtr[which][x].done(gReturnValuesPtr[which][x + 1], u_data);
                }
            }
        }
    }

    if (gLoadBusy != 0) {
        FlushCache(WRITEBACK_DCACHE);
        if (gLoadReturnValue[0] != -1) {
            if (gLoadReturnDef.done != NULL) {
                gLoadReturnDef.done(gLoadReturnValue[0], gLoadReturnDef.u_data);
                gLoadReturnDef.done = NULL;
                gLoadReturnDef.u_data = 0;
            }
            gLoadReturnValue[0] = 0;
            gLoadBusy = 0;
        }
    }

    if (!gCommBusy &&
        gSndCommandBuffePtr[gCommandFillBuffer]->num_commands != 0 &&
        gCaching) {
        snd_SendCurrentBatch();
    }

    if (gSSRead) {
        snd_StreamSafeCdSync(1);
        if (gSSReadDone) {
            gSSRead = 0;
            gSSReadDone = 0;
            if (gCdCallback != NULL) {
                gCdCallback(1);
            }
        }
    }

    return (gCommBusy != 0 || gLoadBusy != 0);
}

/* 001aad38 001aad64 */ void snd_StopSoundSystem() {
    snd_SendIOPCommandAndWait(SL_CLOSE, 0, NULL);
    gStreamingInited = 0;
}

/* 001aad68 001aad8c */ unsigned int snd_GetTick() {
    return snd_SendIOPCommandAndWait(SL_GETSTICK, 0, NULL);
}

/* 001aad90 001aadbc */ void snd_GetTick_CB(/* a0 4 */ SndCompleteProc cb, /* a1 5 */ unsigned long user_data) {
    snd_SendIOPCommandNoWait(SL_GETSTICK, 0, NULL, cb, user_data);
}

/* 001aadc0 001aae68 */ int snd_GotReturns() {
    FlushCache(WRITEBACK_DCACHE);
    if (gCommBusy == NULL) {
        return 1;
    }

    if (SifCheckStatRpc(&gSLClientData) != 0) {
        return 0;
    }

    if (*gCommBusy == -1 && gCommBusy[gAwaitingInts + 1] == -1) {
        gCommBusy = NULL;
        return 1;
    }

    if (!gPrefs_Silent) {
        printf("989snd.c: Sif says RPC isn\'t busy, but we still don\'t have returns from  the IOP!\n");
    }

    return 0;
}

/* 001aae68 001aaecc */ void snd_PrepareReturnBuffer(/* s0 16 */ unsigned int *buffer, /* a1 5 */ int num_ints) {
    gCommBusy = buffer;
    gAwaitingInts = num_ints;
    buffer[num_ints + 1] = 0;
    buffer[0] = 0;
    SyncDCache(buffer, buffer + 3);
    SyncDCache(buffer + num_ints + 1, (buffer + num_ints + 1) + 3);
}

/* 001aaed0 001ab06c */ SoundBankPtr snd_BankLoad(/* s3 19 */ char *name, /* v0 2 */ int offset) {
    gLocalLoadError = 0;
    UNIMPLEMENTED();
}

/* 001ab070 001ab18c */ int snd_BankLoad_CB(/* s3 19 */ char *name, /* s5 21 */ int offset, /* s2 18 */ SndCompleteProc cb, /* s4 20 */ unsigned long long user_data) {
    UNIMPLEMENTED();
}
/* 001ab190 001ab348 */ SoundBankPtr snd_BankLoadByLoc(/* s1 17 */ int loc, /* s2 18 */ int offset) {
    UNIMPLEMENTED();
}
/* 001ab348 001ab4c4 */ void snd_BankLoadByLoc_CB(/* s4 20 */ int loc, /* s1 17 */ int offset, /* s2 18 */ SndCompleteProc cb, /* s3 19 */ unsigned long long user_data) {
    UNIMPLEMENTED();
}
/* 001ab4c8 001ab640 */ SoundBankPtr snd_BankLoadFromEE(/* s1 17 */ void *ee_loc) {
    UNIMPLEMENTED();
}
/* 001ab640 001ab770 */ void snd_BankLoadFromEE_CB(/* s3 19 */ void *ee_loc, /* s1 17 */ SndCompleteProc cb, /* s2 18 */ unsigned long long user_data) {
    UNIMPLEMENTED();
}
/* 001ab770 001ab8e8 */ SoundBankPtr snd_BankLoadFromIOP(/* s1 17 */ void *iop_loc) {
    UNIMPLEMENTED();
}
/* 001ab8e8 001aba18 */ void snd_BankLoadFromIOP_CB(/* s3 19 */ void *iop_loc, /* s1 17 */ SndCompleteProc cb, /* s2 18 */ unsigned long long user_data) {
    UNIMPLEMENTED();
}
/* 001aba18 001abb70 */ MMDPtr snd_MMDLoad(/* s3 19 */ char *name, /* v0 2 */ int offset) {
    UNIMPLEMENTED();
}
/* 001abb70 001abc84 */ int snd_MMDLoad_CB(/* s4 20 */ char *name, /* s5 21 */ int offset, /* s2 18 */ SndCompleteProc cb, /* s3 19 */ unsigned long long user_data) {
    UNIMPLEMENTED();
}
/* 001abc88 001abdfc */ MMDPtr snd_MMDLoadByLoc(/* s2 18 */ int loc, /* s1 17 */ int offset) {
    UNIMPLEMENTED();
}
/* 001abe00 001abf50 */ void snd_MMDLoadByLoc_CB(/* s2 18 */ int loc, /* s1 17 */ int offset, /* a2 6 */ SndCompleteProc cb, /* a3 7 */ unsigned long long user_data) {
    UNIMPLEMENTED();
}

/* 001abf50 001abf7c */ void snd_ResolveBankXREFS() {
    snd_SendIOPCommandNoWait(SL_RESOLVEBANKS, 0, NULL, NULL, 0);
}
/* 001abf80 001abfb0 */ void snd_UnloadBank(/* -0x20(sp) */ SoundBankPtr bank) {
    snd_SendIOPCommandNoWait(SL_UNLOADBANK_A, sizeof(bank), (char *)&bank, NULL, 0);
}

/* 001abfb0 001abfe0 */ void snd_UnloadBank_CB(/* -0x20(sp) */ SoundBankPtr bank, /* a1 5 */ SndCompleteProc cb, /* a2 6 */ unsigned long long user_data) {
    snd_SendIOPCommandNoWait(SL_UNLOADBANK_A, sizeof(bank), (char *)&bank, cb, user_data);
}

/* 001abfe0 001ac010 */ void snd_UnloadMMD(/* -0x20(sp) */ MMDPtr mmd) {
    snd_SendIOPCommandNoWait(SL_UNLOADMIDI_A, sizeof(mmd), (char *)&mmd, NULL, 0);
}

/* 001ac010 001ac040 */ void snd_UnloadMMD_CB(/* -0x20(sp) */ MMDPtr mmd, /* a1 5 */ SndCompleteProc cb, /* a2 6 */ unsigned long long user_data) {
    snd_SendIOPCommandNoWait(SL_UNLOADMIDI_A, sizeof(mmd), (char *)&mmd, cb, user_data);
}

/* 001ac040 001ac074 */ void snd_SetMasterVolume(/* a0 4 */ int which, /* a1 5 */ int vol) {
    /* -0x20(sp) */ int data[2];
    data[0] = which;
    data[1] = vol;

    snd_SendIOPCommandNoWait(SL_SETMASTERVOL_A, sizeof(data), (char *)&data, NULL, 0);
}

/* 001ac078 001ac0a0 */ int snd_GetMasterVolume(/* -0x20(sp) */ int which) {
    return snd_SendIOPCommandAndWait(SL_GETMASTERVOL, sizeof(which), (char *)&which);
}

/* 001ac0a0 001ac0d0 */ void snd_GetMasterVolume_CB(/* -0x20(sp) */ int which, /* a1 5 */ SndCompleteProc cb, /* a2 6 */ unsigned long long user_data) {
    snd_SendIOPCommandNoWait(SL_GETMASTERVOL, sizeof(which), (char *)&which, cb, user_data);
}

/* 001ac0d0 001ac144 */ void snd_SetMasterVolumeDucker(/* a0 4 */ int which, /* a1 5 */ DuckerDefPtr state) {
    /* -0x30(sp) */ int data[7];
    data[0] = which;

    if (state != NULL) {
        memcpy((void *)&data[1], state, sizeof(*state));
    } else {
        data[1] = -1;
    }

    snd_SendIOPCommandNoWait(SL_SETMASTERVOLUMEDUCKER, sizeof(data), (char *)data, NULL, 0);
}
/* 001ac148 001ac178 */ void snd_SetPlaybackMode(/* -0x20(sp) */ int mode) {
    UNIMPLEMENTED();
}
/* 001ac178 001ac19c */ int snd_GetPlaybackMode() {
    UNIMPLEMENTED();
}
/* 001ac1a0 001ac1cc */ void snd_GetPlaybackMode_CB(/* a0 4 */ SndCompleteProc cb, /* a1 5 */ unsigned long long user_data) {
    UNIMPLEMENTED();
}
/* 001ac1d0 001ac204 */ void snd_SetMixerMode(/* a0 4 */ int channel_mode, /* a1 5 */ int reverb_mode) {
    /* -0x20(sp) */ int data[2];
    UNIMPLEMENTED();
}
/* 001ac208 001ac238 */ void snd_SetPanMode(/* -0x20(sp) */ int pan_mode) {
    UNIMPLEMENTED();
}
/* 001ac238 001ac274 */ void snd_SetGroupVoiceRange(/* a0 4 */ int group, /* v0 2 */ int min, /* a2 6 */ int max) {
    /* -0x20(sp) */ int data[3];
    UNIMPLEMENTED();
}
/* 001ac278 001ac2a8 */ void snd_SetReverbMode(/* -0x20(sp) */ int reverb_mode) {
    UNIMPLEMENTED();
}
/* 001ac2a8 001ac2d4 */ int snd_IsSoundALooper(/* a0 4 */ SoundBankPtr bank, /* a1 5 */ int sound) {
    /* -0x20(sp) */ int data[2];
    UNIMPLEMENTED();
}
/* 001ac2d8 001ac30c */ void snd_IsSoundALooper_CB(/* a0 4 */ SoundBankPtr bank, /* a1 5 */ int sound, /* a2 6 */ SndCompleteProc cb, /* t0 8 */ unsigned long long user_data) {
    /* -0x20(sp) */ int data[2];
    UNIMPLEMENTED();
}
/* 001ac310 001ac34c */ unsigned int snd_PlaySoundVolPanPMPB(/* a0 4 */ SoundBankPtr bank, /* a1 5 */ int sound, /* a2 6 */ int vol, /* a3 7 */ int pan, /* t0 8 */ int pitch_mod, /* t1 9 */ int bend) {
    /* -0x30(sp) */ int data[6];
    UNIMPLEMENTED();
}
/* 001ac350 001ac3b0 */ unsigned int snd_PlaySoundByNameVolPanPMPB(/* a0 4 */ SoundBankPtr bank, /* a1 5 */ unsigned long long bank_name, /* a2 6 */ unsigned __int128 sound, /* a3 7 */ int vol, /* t0 8 */ int pan, /* t1 9 */ int pitch_mod, /* t2 10 */ int bend) {
    /* -0x40(sp) */ SndPlayParams pss;
    UNIMPLEMENTED();
}
/* 001ac3b0 001ac3f4 */ void snd_PlaySoundVolPanPMPB_A(/* a0 4 */ SoundBankPtr bank, /* a1 5 */ int sound, /* a2 6 */ int vol, /* a3 7 */ int pan, /* t0 8 */ int pitch_mod, /* t1 9 */ int bend) {
    /* -0x30(sp) */ int data[6];
    UNIMPLEMENTED();
}
/* 001ac3f8 001ac460 */ void snd_PlaySoundByNameVolPanPMPB_A(/* a0 4 */ SoundBankPtr bank, /* a1 5 */ unsigned long bank_name, /* a2 6 */ unsigned __int128 sound, /* a3 7 */ int vol, /* t0 8 */ int pan, /* t1 9 */ int pitch_mod, /* t2 10 */ int bend) {
    /* -0x40(sp) */ SndPlayParams pss;
    UNIMPLEMENTED();
}
/* 001ac460 001ac4a8 */ void snd_PlaySoundVolPanPMPB_CB(/* a0 4 */ SoundBankPtr bank, /* a1 5 */ int sound, /* a2 6 */ int vol, /* v0 2 */ int pan, /* t0 8 */ int pitch_mod, /* t1 9 */ int bend, /* t2 10 */ SndCompleteProc cb, /* t3 11 */ unsigned long long user_data) {
    /* -0x30(sp) */ int data[6];
    UNIMPLEMENTED();
}
/* 001ac4a8 001ac518 */ void snd_PlaySoundByNameVolPanPMPB_CB(/* a0 4 */ SoundBankPtr bank, /* a1 5 */ unsigned long long bank_name, /* a2 6 */ unsigned __int128 sound, /* v1 3 */ int vol, /* t4 12 */ int pan, /* t1 9 */ int pitch_mod, /* t2 10 */ int bend, /* a3 7 */ SndCompleteProc cb, /* 0x0(sp) */ unsigned long long user_data) {
    /* -0x40(sp) */ SndPlayParams pss;
    UNIMPLEMENTED();
}
/* 001ac518 001ac53c */ unsigned int snd_PlaySoundEx(/* a0 4 */ SndPlayParamsPtr params) {
    UNIMPLEMENTED();
}
/* 001ac540 001ac56c */ void snd_PlaySoundEx_A(/* a0 4 */ SndPlayParamsPtr params) {
    UNIMPLEMENTED();
}
/* 001ac570 001ac59c */ void snd_PlaySoundEx_CB(/* a0 4 */ SndPlayParamsPtr params, /* a1 5 */ SndCompleteProc cb, /* t0 8 */ unsigned long long user_data) {
    UNIMPLEMENTED();
}
/* 001ac5a0 001ac5d0 */ void snd_StopSound(/* -0x20(sp) */ unsigned int handle) {
    UNIMPLEMENTED();
}
/* 001ac5d0 001ac60c */ int snd_GetSoundUserData(/* a0 4 */ SoundBankPtr bank, /* v0 2 */ unsigned long long bank_name, /* a2 6 */ int sound_index, /* a3 7 */ unsigned __int128 sound_name, /* t0 8 */ int *destination) {
    /* -0x40(sp) */ GetSoundUserDataCommandStruct ud;
    UNIMPLEMENTED();
}
/* 001ac610 001ac654 */ void snd_GetSoundUserData_CB(/* a0 4 */ SoundBankPtr bank, /* v0 2 */ unsigned long long bank_name, /* a2 6 */ int sound_index, /* a3 7 */ unsigned __int128 sound_name, /* t0 8 */ int *destination, /* t1 9 */ SndCompleteProc cb, /* t2 10 */ unsigned long long user_data) {
    /* -0x40(sp) */ GetSoundUserDataCommandStruct ud;
    UNIMPLEMENTED();
}
/* 001ac658 001ac68c */ char snd_GetSoundReg(/* a0 4 */ unsigned int handle, /* a1 5 */ int which) {
    /* -0x20(sp) */ int data[2];
    UNIMPLEMENTED();
}
/* 001ac690 001ac6c4 */ void snd_GetSoundReg_CB(/* a0 4 */ unsigned int handle, /* a1 5 */ int which, /* a2 6 */ SndCompleteProc cb, /* t0 8 */ unsigned long long user_data) {
    /* -0x20(sp) */ int data[2];
    UNIMPLEMENTED();
}
/* 001ac6c8 001ac708 */ void snd_SetSoundReg(/* a0 4 */ unsigned int handle, /* a1 5 */ int which, /* a2 6 */ char val) {
    /* -0x20(sp) */ int data[3];
    UNIMPLEMENTED();
}
/* 001ac708 001ac75c */ void snd_SetAllSoundReg(/* a0 4 */ unsigned int handle, /* v0 2 */ char *vals) {
    /* -0x30(sp) */ unsigned int data[5];
    UNIMPLEMENTED();
}
/* 001ac760 001ac790 */ char snd_GetSFXGlobalReg(/* -0x20(sp) */ int which) {
    UNIMPLEMENTED();
}
/* 001ac790 001ac7c0 */ void snd_GetSFXGlobalReg_CB(/* -0x20(sp) */ int which, /* a1 5 */ SndCompleteProc cb, /* a2 6 */ unsigned long long user_data) {
    UNIMPLEMENTED();
}
/* 001ac7c0 001ac7fc */ void snd_SetSFXGlobalReg(/* a0 4 */ int which, /* a1 5 */ char val) {
    /* -0x20(sp) */ unsigned int data[2];
    UNIMPLEMENTED();
}
/* 001ac800 001ac830 */ void snd_PauseSound(/* -0x20(sp) */ unsigned int handle) {
    UNIMPLEMENTED();
}
/* 001ac830 001ac860 */ void snd_ContinueSound(/* -0x20(sp) */ unsigned int handle) {
    UNIMPLEMENTED();
}
/* 001ac860 001ac88c */ void snd_StopAllSounds() {
    UNIMPLEMENTED();
}
/* 001ac890 001ac8c0 */ void snd_PauseAllSoundsInGroup(/* -0x20(sp) */ unsigned int groups) {
    UNIMPLEMENTED();
}
/* 001ac8c0 001ac8f0 */ void snd_ContinueAllSoundsInGroup(/* -0x20(sp) */ unsigned int groups) {
    UNIMPLEMENTED();
}
/* 001ac8f0 001ac920 */ void snd_StopAllSoundsInGroup(/* -0x20(sp) */ unsigned int groups) {
    UNIMPLEMENTED();
}
/* 001ac920 001ac948 */ unsigned int snd_SoundIsStillPlaying(/* -0x20(sp) */ unsigned int handle) {
    UNIMPLEMENTED();
}
/* 001ac948 001ac978 */ void snd_SoundIsStillPlaying_CB(/* -0x20(sp) */ unsigned int handle, /* a1 5 */ SndCompleteProc cb, /* a2 6 */ unsigned long long user_data) {
    UNIMPLEMENTED();
}
/* 001ac978 001ac9b4 */ void snd_SetSoundVolPan(/* a0 4 */ unsigned int handle, /* v0 2 */ int vol, /* a2 6 */ int pan) {
    /* -0x20(sp) */ int data[3];
    UNIMPLEMENTED();
}
/* 001ac9b8 001ac9e4 */ int snd_GetSoundOriginalPitch(/* a0 4 */ SoundBankPtr bank, /* a1 5 */ int index) {
    /* -0x20(sp) */ int data[2];
    UNIMPLEMENTED();
}
/* 001ac9e8 001aca1c */ void snd_GetSoundOriginalPitch_CB(/* a0 4 */ SoundBankPtr bank, /* a1 5 */ int index, /* a2 6 */ SndCompleteProc cb, /* t0 8 */ unsigned long long user_data) {
    /* -0x20(sp) */ int data[2];
    UNIMPLEMENTED();
}
/* 001aca20 001aca48 */ int snd_GetSoundCurrentPitch(/* -0x20(sp) */ unsigned int handle) {
    UNIMPLEMENTED();
}
/* 001aca48 001aca78 */ void snd_GetSoundCurrentPitch_CB(/* -0x20(sp) */ unsigned int handle, /* a1 5 */ SndCompleteProc cb, /* a2 6 */ unsigned long long user_data) {
    UNIMPLEMENTED();
}
/* 001aca78 001acaac */ void snd_SetSoundPitch(/* a0 4 */ unsigned int handle, /* a1 5 */ int pitch) {
    /* -0x20(sp) */ int data[2];
    UNIMPLEMENTED();
}
/* 001acab0 001acae4 */ void snd_SetSoundPitchBend(/* a0 4 */ unsigned int handle, /* a1 5 */ int bend) {
    /* -0x20(sp) */ int data[2];
    UNIMPLEMENTED();
}
/* 001acae8 001acb1c */ void snd_SetSoundPitchModifier(/* a0 4 */ unsigned int handle, /* a1 5 */ int mod) {
    /* -0x20(sp) */ int data[2];
    UNIMPLEMENTED();
}
/* 001acb20 001acb5c */ unsigned int snd_SetSoundParams(/* a0 4 */ unsigned int handle, /* a1 5 */ unsigned int mask, /* a2 6 */ int vol, /* a3 7 */ int pan, /* t0 8 */ int pitch_mod, /* t1 9 */ int bend) {
    /* -0x30(sp) */ int data[6];
    UNIMPLEMENTED();
}
/* 001acb60 001acb80 */ void snd_SetSoundParams_A(/* a0 4 */ unsigned int handle, /* a1 5 */ unsigned int mask, /* a2 6 */ int vol, /* a3 7 */ int pan, /* t0 8 */ int pitch_mod, /* t1 9 */ int bend) {
    UNIMPLEMENTED();
}
/* 001acb80 001acbc8 */ void snd_SetSoundParams_CB(/* a0 4 */ unsigned int handle, /* a1 5 */ unsigned int mask, /* a2 6 */ int vol, /* v0 2 */ int pan, /* t0 8 */ int pitch_mod, /* t1 9 */ int bend, /* t2 10 */ SndCompleteProc cb, /* t3 11 */ unsigned long long user_data) {
    /* -0x30(sp) */ int data[6];
    UNIMPLEMENTED();
}
/* 001acbc8 001acc04 */ void snd_AutoVol(/* a0 4 */ unsigned int handle, /* a1 5 */ int vol, /* a2 6 */ int delta_time, /* a3 7 */ int delta_from) {
    /* -0x20(sp) */ int data[4];
    UNIMPLEMENTED();
}
/* 001acc08 001acc48 */ void snd_AutoPan(/* a0 4 */ unsigned int handle, /* a1 5 */ int pan, /* a2 6 */ int dir, /* a3 7 */ int delta_time, /* t0 8 */ int delta_from) {
    /* -0x30(sp) */ int data[5];
    UNIMPLEMENTED();
}
/* 001acc48 001acc84 */ void snd_AutoPitch(/* a0 4 */ unsigned int handle, /* a1 5 */ int pitch, /* a2 6 */ int delta_time, /* a3 7 */ int delta_from) {
    /* -0x20(sp) */ int data[4];
    UNIMPLEMENTED();
}
/* 001acc88 001accc4 */ void snd_AutoPitchBend(/* a0 4 */ unsigned int handle, /* a1 5 */ int pb, /* a2 6 */ int delta_time, /* a3 7 */ int delta_from) {
    /* -0x20(sp) */ int data[4];
    UNIMPLEMENTED();
}
/* 001accc8 001acd04 */ void snd_SetMIDIRegister(/* a0 4 */ unsigned int handle, /* v0 2 */ int reg, /* a2 6 */ int value) {
    /* -0x20(sp) */ int data[3];
    UNIMPLEMENTED();
}
/* 001acd08 001acd34 */ int snd_GetMIDIRegister(/* a0 4 */ unsigned int handle, /* a1 5 */ int reg) {
    /* -0x20(sp) */ int data[2];
    UNIMPLEMENTED();
}
/* 001acd38 001acd6c */ void snd_GetMIDIRegister_CB(/* a0 4 */ unsigned int handle, /* a1 5 */ int reg, /* a2 6 */ SndCompleteProc cb, /* t0 8 */ unsigned long long user_data) {
    /* -0x20(sp) */ int data[2];
    UNIMPLEMENTED();
}
/* 001acd70 001acda0 */ void snd_SetGlobalExcite(/* -0x20(sp) */ int value) {
    UNIMPLEMENTED();
}
/* 001acda0 001acdc4 */ int snd_GetGlobalExcite() {
    UNIMPLEMENTED();
}
/* 001acdc8 001acdf4 */ void snd_GetGlobalExcite_CB(/* a0 4 */ SndCompleteProc cb, /* a1 5 */ unsigned long long user_data) {
    UNIMPLEMENTED();
}
/* 001acdf8 001ace20 */ int snd_GetVoiceAllocation(/* -0x20(sp) */ int core) {
    UNIMPLEMENTED();
}
/* 001ace20 001ace50 */ void snd_GetVoiceAllocation_CB(/* -0x20(sp) */ int core, /* a1 5 */ SndCompleteProc cb, /* a2 6 */ unsigned long long user_data) {
    UNIMPLEMENTED();
}
/* 001ace50 001ad0a8 */ unsigned int snd_SendIOPCommandAndWait(/* s2 18 */ int command, /* s0 16 */ int data_size, /* a3 7 */ char *data) {
    /* a1 5 */ int x;
    /* s0 16 */ unsigned int ret_val;
    UNIMPLEMENTED();
}

/* 001ad0a8 001ad428 */ void snd_SendIOPCommandNoWait(/* s7 23 */ int command, /* s0 16 */ int data_size, /* s5 21 */ char *data, /* -0xc0(sp) */ SndCompleteProc done, /* -0xb8(sp) */ unsigned long long u_data) {
    /* a2 6 */ int x;
    /* -0xb0(sp) */ int was_caching;
    /* s2 18 */ int y;
    /* a1 5 */ char *msg;
    /* s1 17 */ int msg_size;

    if (!gCaching && !gCommBusy) {
        if (data_size == 0 && done == NULL) {
            snd_PrepareReturnBuffer(gSyncBuffer, 1);
            while (SifCheckStatRpc(&gSLClientData) != 0) {
                if (!gPrefs_Silent) {
                    printf("989snd.c: RPC collision!\n");
                }
                snd_FlushSoundCommands();
                FlushCache(WRITEBACK_DCACHE);
            }

            SifCallRpc(&gSLClientData, command, 1, NULL, 0, &gSyncBuffer, sizeof(gSyncBuffer), NULL, NULL);
            return;
        }
    }

    msg_size = data_size + 4;
    if (!IS_ALIGNED(msg_size, 4)) {
        msg_size = ALIGN(msg_size, 4);
    }

    if (msg_size > 512) {
        printf("989snd ERROR: command too large!\n");
        while (true)
            ;
    }

    y = 0;
    while (gSndCommandBuffePtr[gCommandFillBuffer]->num_commands == 256 ||
           gCommandBuffeBytesAvail[gCommandFillBuffer] < msg_size) {
        if (gCaching) {
            gCaching = 0;
            was_caching = 1;
        }

        snd_FlushSoundCommands();
        if (y == 1) {
            if (!gPrefs_Silent) {
                printf("snd_SendIOPCommandNoWait: BUFFER %d FULL(%d)! Sound stalled waiting for  IOP comands to finish!\n",
                       gCommandFillBuffer, gSndCommandBuffePtr[gCommandFillBuffer]->num_commands);
            }
        }

        y++;
    }

    if (y != 0 && !gPrefs_Silent) {
        printf("snd_SendIOPCommandNoWait: continuing (%d).\n", y);
    }

    if (was_caching) {
        gCaching = 1;
    }

    SndCommandEntryPtr cmd = &gSndCommandBuffePtr[gCommandFillBuffer]->buffer[4096 - gCommandBuffeBytesAvail[gCommandFillBuffer]];
    cmd->command = command;
    cmd->size = msg_size;

    if (command == SL_EXTERNCALLWITHDATA) {
        // TODO
        UNIMPLEMENTED();
    } else {
        memcpy(((char *)&cmd) + sizeof(*cmd), data, msg_size);
    }

    gCommandBuffeBytesAvail[gCommandFillBuffer] -= msg_size;
    gSndCommandReturnDefPtr[gCommandFillBuffer][gSndCommandBuffePtr[gCommandFillBuffer]->num_commands].done = done;
    gSndCommandReturnDefPtr[gCommandFillBuffer][gSndCommandBuffePtr[gCommandFillBuffer]->num_commands].u_data = u_data;
    snd_PostMessage();
}

/* 001ad428 001ad468 */ void snd_PostMessage() {
    gSndCommandBuffePtr[gCommandFillBuffer]->num_commands++;
    snd_FlushSoundCommands();
}

/* 001ad468 001ad5b0 */ void snd_SendCurrentBatch() {
    snd_PrepareReturnBuffer(gReturnValuesPtr[gCommandFillBuffer], gSndCommandBuffePtr[gCommandFillBuffer]->num_commands);
    while (SifCheckStatRpc(&gSLClientData) != 0) {
        if (gPrefs_Silent == 0) {
            printf("989snd.c: RPC collision!\n");
        }
        FlushCache(0);
    }
    SifCallRpc(&gSLClientData,
               SL_COMMAND_BATCH,
               1,
               gSndCommandBuffePtr[gCommandFillBuffer],
               0x1000 - gCommandBuffeBytesAvail[gCommandFillBuffer],
               gReturnValuesPtr[gCommandFillBuffer],
               gSndCommandBuffePtr[gCommandFillBuffer]->num_commands * 4 + 8,
               NULL, NULL);

    gCommandFillBuffer = !gCommandFillBuffer;
    gSndCommandBuffePtr[gCommandFillBuffer]->num_commands = 0;
    gCommandBuffeBytesAvail[gCommandFillBuffer] = 4092;
}
/* 001ad5b0 001ad5c0 */ void snd_StartCaching() {
    gCaching = 1;
}
/* 001ad5c0 001ad5e0 */ void snd_SendCache() {
    gCaching = 0;
    snd_FlushSoundCommands();
}
/* 001ad5e0 001ad69c */ int snd_InitVAGStreamingEx(/* s1 17 */ int num_channels, /* s0 16 */ int buffer_size, /* s2 18 */ unsigned int read_mode, /* s3 19 */ int enable_streamsafe_from_ee) {
    /* -0x70(sp) */ int data[4];
    UNIMPLEMENTED();
}
/* 001ad6a0 001ad6dc */ void snd_SetVAGStreamSubGroupVolPan(/* a0 4 */ unsigned int handle, /* a1 5 */ int group, /* a2 6 */ int vol, /* a3 7 */ int pan) {
    /* -0x20(sp) */ int data[4];
    UNIMPLEMENTED();
}
/* 001ad6e0 001ad71c */ void snd_SetVAGStreamChannelRange(/* a0 4 */ int vol_group, /* v0 2 */ int ch_min, /* a2 6 */ int ch_max) {
    /* -0x20(sp) */ int data[3];
    UNIMPLEMENTED();
}
/* 001ad720 001ad750 */ void snd_StopVAGStream(/* -0x20(sp) */ unsigned int stream) {
    UNIMPLEMENTED();
}
/* 001ad750 001ad77c */ void snd_StopAllStreams() {
    UNIMPLEMENTED();
}
/* 001ad780 001ad7f4 */ void snd_CloseVAGStreaming() {
    UNIMPLEMENTED();
}
/* 001ad7f8 001ad85c */ unsigned int snd_PlayVAGStreamByLocEx(/* a0 4 */ int loc1, /* a1 5 */ int loc2, /* a2 6 */ int offset1, /* a3 7 */ int offset2, /* t0 8 */ int vol, /* t1 9 */ int pan, /* t2 10 */ int vol_group, /* t3 11 */ unsigned int queue, /* 0x0(sp) */ int sub_group, /* 0x8(sp) */ unsigned int flags) {
    //    /* v1 3 */ int sub_group;
    //    /* v0 2 */ unsigned int flags;
    //    /* -0x30(sp) */ int data[8];
    UNIMPLEMENTED();
}
/* 001ad860 001ad890 */ void snd_PlayVAGStreamByLocEx_A(/* a0 4 */ int loc1, /* a1 5 */ int loc2, /* a2 6 */ int offset1, /* a3 7 */ int offset2, /* t0 8 */ int vol, /* t1 9 */ int pan, /* t2 10 */ int vol_group, /* t3 11 */ unsigned int queue, /* 0x0(sp) */ int sub_group, /* 0x8(sp) */ unsigned int flags) {
    //    /* v0 2 */ int sub_group;
    //    /* v1 3 */ unsigned int flags;
    UNIMPLEMENTED();
}
/* 001ad890 001ad900 */ void snd_PlayVAGStreamByLocEx_CB(/* a0 4 */ int loc1, /* v1 3 */ int loc2, /* a2 6 */ int offset1, /* a3 7 */ int offset2, /* t0 8 */ int vol, /* t1 9 */ int pan, /* t2 10 */ int vol_group, /* t3 11 */ unsigned int queue, /* 0x0(sp) */ int sub_group, /* 0x8(sp) */ unsigned int flags, /* 0x10(sp) */ SndCompleteProc cb, /* 0x18(sp) */ unsigned long long user_data) {
    //    /* t5 13 */ int sub_group;
    //    /* t4 12 */ unsigned int flags;
    //    /* a3 7 */ SndCompleteProc cb;
    //    /* t0 8 */ unsigned long long user_data;
    /* -0x30(sp) */ int data[8];
    UNIMPLEMENTED();
}
/* 001ad900 001ad9d4 */ unsigned int snd_PlayVAGStreamByNameEx(/* a0 4 */ char *name1, /* s0 16 */ char *name2, /* s1 17 */ unsigned int offset1, /* s2 18 */ unsigned int offset2, /* s3 19 */ int vol, /* s4 20 */ int pan, /* s5 21 */ int vol_group, /* s6 22 */ unsigned int queue, /* 0x0(sp) */ int sub_group, /* 0x8(sp) */ unsigned int flags) {
    //    /* v1 3 */ int sub_group;
    //    /* v0 2 */ unsigned int flags;
    /* -0x120(sp) */ PVSBN_struct data;
    UNIMPLEMENTED();
}
/* 001ad9d8 001ada08 */ void snd_PlayVAGStreamByNameEx_A(/* a0 4 */ char *name1, /* a1 5 */ char *name2, /* a2 6 */ unsigned int offset1, /* a3 7 */ unsigned int offset2, /* t0 8 */ int vol, /* t1 9 */ int pan, /* t2 10 */ int vol_group, /* t3 11 */ unsigned int queue, /* 0x0(sp) */ int sub_group, /* 0x8(sp) */ unsigned int flags) {
    //    /* v0 2 */ int sub_group;
    //    /* v1 3 */ unsigned int flags;
    UNIMPLEMENTED();
}
/* 001ada08 001adae4 */ void snd_PlayVAGStreamByNameEx_CB(/* a0 4 */ char *name1, /* s0 16 */ char *name2, /* s1 17 */ unsigned int offset1, /* s2 18 */ unsigned int offset2, /* s3 19 */ int vol, /* s4 20 */ int pan, /* s5 21 */ int vol_group, /* s6 22 */ unsigned int queue, /* 0x0(sp) */ int sub_group, /* 0x8(sp) */ unsigned int flags, /* 0x10(sp) */ SndCompleteProc cb, /* 0x18(sp) */ unsigned long long user_data) {
    //    /* v1 3 */ int sub_group;
    //    /* v0 2 */ unsigned int flags;
    /* -0x120(sp) */ PVSBN_struct data;
    UNIMPLEMENTED();
}
/* 001adae8 001adb18 */ void snd_PauseVAGStream(/* -0x20(sp) */ unsigned int stream) {
    UNIMPLEMENTED();
}
/* 001adb18 001adb48 */ void snd_ContinueVAGStream(/* -0x20(sp) */ unsigned int stream) {
    UNIMPLEMENTED();
}
/* 001adb48 001adb70 */ int snd_GetVAGStreamQueueCount(/* -0x20(sp) */ unsigned int stream) {
    UNIMPLEMENTED();
}
/* 001adb70 001adba0 */ void snd_GetVAGStreamQueueCount_CB(/* -0x20(sp) */ unsigned int stream, /* a1 5 */ SndCompleteProc cb, /* a2 6 */ unsigned long long user_data) {
    UNIMPLEMENTED();
}
/* 001adba0 001adbc8 */ unsigned int snd_GetVAGStreamLoc(/* -0x20(sp) */ unsigned int stream) {
    UNIMPLEMENTED();
}
/* 001adbc8 001adbf8 */ void snd_GetVAGStreamLoc_CB(/* -0x20(sp) */ unsigned int stream, /* a1 5 */ SndCompleteProc cb, /* a2 6 */ unsigned long long user_data) {
    UNIMPLEMENTED();
}
/* 001adbf8 001adc20 */ unsigned int snd_GetVAGStreamTimeRemaining(/* -0x20(sp) */ unsigned int stream) {
    UNIMPLEMENTED();
}
/* 001adc20 001adc50 */ void snd_GetVAGStreamTimeRemaining_CB(/* -0x20(sp) */ unsigned int stream, /* a1 5 */ SndCompleteProc cb, /* a2 6 */ unsigned long long user_data) {
    UNIMPLEMENTED();
}
/* 001adc50 001adc78 */ unsigned int snd_GetVAGStreamTotalTime(/* -0x20(sp) */ unsigned int stream) {
    UNIMPLEMENTED();
}
/* 001adc78 001adca8 */ void snd_GetVAGStreamTotalTime_CB(/* -0x20(sp) */ unsigned int stream, /* a1 5 */ SndCompleteProc cb, /* a2 6 */ unsigned long long user_data) {
    UNIMPLEMENTED();
}
/* 001adca8 001adcd0 */ int snd_IsVAGStreamBuffered(/* -0x20(sp) */ unsigned int stream) {
    UNIMPLEMENTED();
}
/* 001adcd0 001add00 */ void snd_IsVAGStreamBuffered_CB(/* -0x20(sp) */ unsigned int stream, /* a1 5 */ SndCompleteProc cb, /* a2 6 */ unsigned long long user_data) {
    UNIMPLEMENTED();
}
/* 001add00 001add28 */ int snd_StreamSafeCheckCDIdle(/* -0x20(sp) */ int block_ee_iop) {
    UNIMPLEMENTED();
}
/* 001add28 001add58 */ void snd_StreamSafeCheckCDIdle_CB(/* -0x20(sp) */ int block_iop, /* a1 5 */ SndCompleteProc cb, /* a2 6 */ unsigned long long user_data) {
    UNIMPLEMENTED();
}
/* 001add58 001ade20 */ int snd_StreamSafeCdRead(/* s1 17 */ unsigned int lbn, /* s2 18 */ unsigned int sectors, /* s3 19 */ void *buf, /* a3 7 */ sceCdRMode *mode) {
    /* -0x60(sp) */ int data[3];
    UNIMPLEMENTED();
}
/* 001ade20 001adee0 */ int snd_StreamSafeCdSync(/* s0 16 */ int mode) {
    /* s1 17 */ char *end;
    UNIMPLEMENTED();
}
/* 001adee0 001adf28 */ int snd_StreamSafeCdBreak() {
    UNIMPLEMENTED();
}
/* 001adf28 001adf64 */ int snd_StreamSafeCdGetError() {
    UNIMPLEMENTED();
}
/* 001adf68 001ae060 */ int snd_StreamSafeCdSearchFile(/* s3 19 */ sceCdlFILE *fp, /* a1 5 */ char *name) {
    /* -0x150(sp) */ char buff[256];
    /* s0 16 */ int ret;
    /* s1 17 */ char *end;
    UNIMPLEMENTED();
}
/* 001ae060 001ae084 */ int snd_StreamSafeCdStatus() {
    UNIMPLEMENTED();
}
/* 001ae088 001ae0b4 */ void snd_StreamSafeCdStatus_CB(/* a0 4 */ SndCompleteProc cb, /* a1 5 */ unsigned long long user_data) {
    UNIMPLEMENTED();
}
/* 001ae0b8 001ae0f0 */ int *snd_StreamSafeCdCallback(/* a0 4 */ void (*func)(/* parameters unknown */)) {
    /* v0 2 */ void (*func_hold)(/* parameters unknown */);
    UNIMPLEMENTED();
}
/* 001ae0f0 001ae124 */ void snd_SetReverbType(/* a0 4 */ int core, /* a1 5 */ int type) {
    /* -0x20(sp) */ int data[2];
    UNIMPLEMENTED();
}
/* 001ae128 001ae164 */ void snd_SetReverbDepth(/* a0 4 */ int core, /* v0 2 */ int left, /* a2 6 */ int right) {
    /* -0x20(sp) */ int data[3];
    UNIMPLEMENTED();
}
/* 001ae168 001ae1a8 */ void snd_SetReverbEx(/* a0 4 */ int core, /* a1 5 */ int type, /* a2 6 */ int depth, /* a3 7 */ int delay, /* t0 8 */ int feedback) {
    /* -0x30(sp) */ int data[5];
    UNIMPLEMENTED();
}
/* 001ae1a8 001ae1dc */ void snd_PreAllocReverbWorkArea(/* a0 4 */ int core, /* a1 5 */ int type) {
    /* -0x20(sp) */ int data[2];
    UNIMPLEMENTED();
}
/* 001ae1e0 001ae21c */ void snd_AutoReverb(/* a0 4 */ int core, /* a1 5 */ int depth, /* a2 6 */ int delta_time, /* a3 7 */ int channel_flags) {
    /* -0x20(sp) */ int data[4];
    UNIMPLEMENTED();
}
/* 001ae220 001ae244 */ int snd_LockVoiceAllocator() {
    UNIMPLEMENTED();
}
/* 001ae248 001ae274 */ int snd_ExternVoiceAlloc(/* a0 4 */ int vol_group, /* a1 5 */ int priority) {
    /* -0x20(sp) */ int data[2];
    UNIMPLEMENTED();
}
/* 001ae278 001ae2a8 */ void snd_ExternVoiceFree(/* -0x20(sp) */ int voice) {
    UNIMPLEMENTED();
}
/* 001ae2a8 001ae2d4 */ void snd_UnlockVoiceAllocator() {
    UNIMPLEMENTED();
}
/* 001ae2d8 001ae300 */ unsigned int snd_SRAMMalloc(/* -0x20(sp) */ unsigned int size) {
    UNIMPLEMENTED();
}
/* 001ae300 001ae32c */ unsigned int snd_SRAMMarkUsed(/* a0 4 */ unsigned int loc, /* a1 5 */ unsigned int size) {
    /* -0x20(sp) */ unsigned int data[2];
    UNIMPLEMENTED();
}
/* 001ae330 001ae364 */ void snd_SRAMFree(/* a0 4 */ unsigned int loc, /* a1 5 */ unsigned int size) {
    /* -0x20(sp) */ unsigned int data[2];
    UNIMPLEMENTED();
}
/* 001ae368 001ae38c */ int snd_SRAMGetFreeMem() {
    UNIMPLEMENTED();
}
/* 001ae390 001ae3bc */ void snd_SRAMGetFreeMem_CB(/* a0 4 */ SndCompleteProc cb, /* a1 5 */ unsigned long long user_data) {
    UNIMPLEMENTED();
}
/* 001ae3c0 001ae3e4 */ int snd_SRAMMaxFreeBlockSize() {
    UNIMPLEMENTED();
}
/* 001ae3e8 001ae414 */ void snd_SRAMMaxFreeBlockSize_CB(/* a0 4 */ SndCompleteProc cb, /* a1 5 */ unsigned long long user_data) {
    UNIMPLEMENTED();
}
/* 001ae418 001ae454 */ int snd_InitMovieSoundEx(/* a0 4 */ int sizeOfIOPBuffer, /* a1 5 */ int sizeOfSPUBuffer, /* a2 6 */ int volumeLevel, /* a3 7 */ int panCenter, /* t0 8 */ int volumeGroup, /* t1 9 */ int type) {
    /* -0x30(sp) */ int data[6];
    UNIMPLEMENTED();
}
/* 001ae458 001ae47c */ void snd_ResetMovieSound() {
    UNIMPLEMENTED();
}
/* 001ae480 001ae4a4 */ unsigned int snd_PauseMovieSound() {
    UNIMPLEMENTED();
}
/* 001ae4a8 001ae4dc */ void snd_SetMovieSoundVolPan(/* a0 4 */ int vol, /* a1 5 */ int pan) {
    /* -0x20(sp) */ int data[2];
    UNIMPLEMENTED();
}
/* 001ae4e0 001ae504 */ void snd_CloseMovieSound() {
    UNIMPLEMENTED();
}
/* 001ae508 001ae540 */ void snd_StartMovieSoundEx(/* a0 4 */ int iopBuffer, /* a1 5 */ int iopBufferSize, /* a2 6 */ int iopPausePosition, /* a3 7 */ unsigned int sr, /* t0 8 */ int ch) {
    /* -0x30(sp) */ int data[5];
    UNIMPLEMENTED();
}
/* 001ae540 001ae564 */ int snd_GetTransStatus() {
    UNIMPLEMENTED();
}
/* 001ae568 001ae594 */ void snd_UpdateMovieADPCM(/* a0 4 */ int data_size, /* a1 5 */ unsigned int offset) {
    /* -0x20(sp) */ int data[2];
    UNIMPLEMENTED();
}
/* 001ae598 001ae5bc */ unsigned int snd_GetMovieNAX() {
    UNIMPLEMENTED();
}
/* 001ae5c0 001ae5fc */ int snd_GetLastLoadError() {
    UNIMPLEMENTED();
}
/* 001ae600 001ae640 */ int snd_DoExternCall(/* a0 4 */ unsigned int proc_id, /* a1 5 */ int func_index, /* a2 6 */ int arg1, /* a3 7 */ int arg2, /* t0 8 */ int arg3, /* t1 9 */ int arg4, /* t2 10 */ int arg5) {
    /* -0x30(sp) */ int data[8];
    UNIMPLEMENTED();
}
/* 001ae640 001ae688 */ void snd_DoExternCall_A(/* a0 4 */ unsigned int proc_id, /* a1 5 */ int func_index, /* a2 6 */ int arg1, /* a3 7 */ int arg2, /* t0 8 */ int arg3, /* t1 9 */ int arg4, /* t2 10 */ int arg5) {
    /* -0x30(sp) */ int data[8];
    UNIMPLEMENTED();
}
/* 001ae688 001ae6d8 */ void snd_DoExternCall_CB(/* a0 4 */ unsigned int proc_id, /* a1 5 */ int func_index, /* a2 6 */ int arg1, /* v1 3 */ int arg2, /* t0 8 */ int arg3, /* t1 9 */ int arg4, /* t2 10 */ int arg5, /* t3 11 */ SndCompleteProc cb, /* 0x0(sp) */ unsigned long long user_data) {
    ///* v0 2 */ unsigned long long user_data;
    /* -0x30(sp) */ int data[8];
    UNIMPLEMENTED();
}
/* 001ae6d8 001ae70c */ int snd_DoExternCallWithData(/* a0 4 */ unsigned int proc_id, /* a1 5 */ int func_index, /* a2 6 */ int data_size, /* a3 7 */ void *data_ptr) {
    /* -0x20(sp) */ int data[4];
    UNIMPLEMENTED();
}
/* 001ae710 001ae74c */ void snd_DoExternCallWithData_A(/* a0 4 */ unsigned int proc_id, /* a1 5 */ int func_index, /* a2 6 */ int data_size, /* a3 7 */ void *data_ptr) {
    /* -0x20(sp) */ int data[4];
    UNIMPLEMENTED();
}
/* 001ae750 001ae790 */ void snd_DoExternCallWithData_CB(/* a0 4 */ unsigned int proc_id, /* a1 5 */ int func_index, /* a2 6 */ int data_size, /* v0 2 */ void *data_ptr, /* t0 8 */ SndCompleteProc cb, /* t1 9 */ unsigned long long user_data) {
    /* -0x20(sp) */ int data[4];
    UNIMPLEMENTED();
}
/* 001ae790 001ae7b4 */ int snd_GetDopplerPitchMod(/* a0 4 */ int approaching_mph) {
    UNIMPLEMENTED();
}
