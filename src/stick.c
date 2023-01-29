#include "stick.h"
#include "989snd.h"
#include "LFO.h"
#include "init.h"
#include "intr_code.h"
#include "libsd.h"
#include "reverb.h"
#include "sndhand.h"
#include "stdio.h"
#include "types.h"
#include "valloc.h"
#include "vol.h"

/* data 1c2c */ static UInt32 sTick = 0;
/* data 1c30 */ SInt32 gLockMasterTick = 0;
/* data 1c34 */ UInt32 gMasterTickOwner[32] = {};
/* data 1cb4 */ UInt32 gMasterTickOwnerLast = 0;
/* bss 8d00 */ SInt16 gEnvelopeStatus[48];
/* bss 8d60 */ SInt8 gAutoFreeState[48];

/* 00021a9c 00021b48 */ void snd_LockMasterTick(/* 0x0(sp) */ UInt32 ownerID) {
    if (gLockMasterTick < 32) {
        gMasterTickOwner[gLockMasterTick] = ownerID;
    } else {
        printf("snd_LockMasterTick: nest too deep to track!\n");
    }
    gMasterTickOwnerLast = ownerID;
    gLockMasterTick++;
}

/* 00021b48 00021c38 */ void snd_UnlockMasterTick() {
    if (gLockMasterTick != 0) {
        gLockMasterTick--;
        gMasterTickOwnerLast = 0;

        if (gLockMasterTick < 0) {
            snd_ShowError(36, 0, 0, 0, 0);
            return;
        }

        if (gLockMasterTick < 32) {
            gMasterTickOwner[gLockMasterTick] = 0;
        } else {
            printf("snd_LockMasterTick: nest too deep to track!\n");
        }
    }
}

/* 00021c38 000221fc */ SInt32 snd_SoundTickHandler() {
    /* -0x18(sp) */ SInt16 env;
    /* -0x14(sp) */ struct VoiceAttributes *walk;
    /* data 1cb8 */ static SInt32 in_handler;
    /* -0x10(sp) */ SInt32 core;
    /* -0xc(sp) */ SInt32 c_v;

    if (in_handler) {
        return 0;
    }
    in_handler = 1;

    if (gVoiceAllocatorInUse || gLockMasterTick) {
        gTickAttentionNeeded++;
        if (gTickAttentionNeeded > 480) {
            printf("     Tick Owner list:\n");
            for (int i = 0; i < 32; ++i)
                printf("         [%d] = 0x%8.8x\n", i, gMasterTickOwner[i]);
            printf("     VAlloc Owner : 0x%8.8x\n", gVAllocOwnerID);
        }
        in_handler = 0;
        return 0;
    }

    gTickAttentionNeeded++;
    for (walk = gPlayingListHead; walk != NULL; walk = walk->playlist) {
        core = walk->voice / NUM_VOICE_PER_CORE;
        c_v = walk->voice % NUM_VOICE_PER_CORE;

        if ((gAwaitingKeyOn[core] & (1 << c_v)) == 0) {
            if (sceSdGetParam(SD_VPARAM_ENVX | SD_VOICE(core, c_v)) != 0) {
                gAutoFreeState[walk->voice] = 4;

            } else if ((gKeyedOffVoices[core] & (1 << c_v)) != 0) {
                snd_VoiceIsDone(walk->voice);

            } else if ((gKeyedOnVoices[core] & (1 << c_v)) != 0) {
                gAutoFreeState[walk->voice]--;

                if (gAutoFreeState[walk->voice] != 0) {
                    if (gAutoFreeState[walk->voice] < 0) {
                        gAutoFreeState[walk->voice] = 4;
                    }
                } else {
                    snd_KeyOffVoice(walk->voice);
                }
            }
        }
    }

    while (gTickAttentionNeeded > 0) {
        sTick++;
        snd_UpdateHandlers();
        snd_StopPendingVoices();
        snd_StartPendingVoices();
        snd_CheckVerb();
        snd_HandleLFOs();
        if ((sTick & 3) == 0) {
            snd_CheckDuckers();
        }
        gTickAttentionNeeded--;
    }

    if (gCdBusy) {
        gCdBusyTime++;
        if (gCdBusyTime > 2400) {
            if (!gPrefs_Silent) {
                printf("989snd: Cd has been busy reading %s for 10 seconds.\n", (gCdBusy == 1) ? "a vag"
                                                                                               : "data");
            }
            sceCdBreak();
            gCdBusyTime = 0;
        }
    }

    in_handler = 0;
    return 0;
}

/* 000221fc 00022234 */ UInt32 snd_GetTick() {
    return sTick;
}
/* 00022234 00022264 */ void snd_SetTick(/* 0x0(sp) */ UInt32 val) {
    sTick = val;
}
