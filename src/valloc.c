#include "valloc.h"
#include "989snd.h"
#include "reverb.h"
#include "stick.h"
#include "types.h"

/* data 1dfc */ SInt32 gLockBlockerSema = -1;
/* data 1e00 */ SInt32 gTickAttentionNeeded = 0;
/* data 1e04 */ SInt32 gVoiceAllocationBlocked = 0;
/* data 1e08 */ SInt32 gVoiceAllocatorInUse = 0;
/* data 1e0c */ UInt32 gVAllocOwnerID = 0;
/* data 1e10 */ SInt32 gChannelMode = 0;
/* data 1e14 */ VoiceRange gVoiceRanges[16];
/* data 1e54 */ struct VoiceAttributes *gPlayingListHead = NULL;
/* data 1e58 */ SInt32 gNoiseOwnerPriority[2] = {-1, -1};
/* data 1e60 */ SInt32 gNoiseOwnerVoice[2] = {-1, -1};
/* data 1e68 */ SInt32 gThreadWaitingForVoiceAllocator = -1;
/* bss 92d0 */ struct VoiceAttributes gChannelStatus[48];
/* bss 20 */ UInt32 gAwaitingKeyOn[2];
/* bss 28 */ UInt32 gAwaitingKeyOff[2];
/* bss 30 */ UInt32 gKeyedOnVoices[2];
/* bss 38 */ UInt32 gKeyedOffVoices[2];
/* bss 18 */ UInt32 gReverbVoices[2];
/* bss 10 */ UInt32 gExternVoices[2];

/* 00033514 00033708 */ void snd_InitVoiceAllocator() {
    /* -0x20(sp) */ SInt32 count;
    /* -0x1c(sp) */ SemaParam sema;

    gVoiceAllocationBlocked = 0;
    gTickAttentionNeeded = 0;
    for (int i = 0; i < 48; i++) {
        snd_MarkVoiceFree(i);
        gChannelStatus[i].voice = i;
        gAutoFreeState[i] = 0;
    }
    gAwaitingKeyOn[0] = 0;
    gAwaitingKeyOn[1] = 0;
    gAwaitingKeyOff[0] = 0;
    gAwaitingKeyOff[1] = 0;
    gKeyedOnVoices[0] = 0;
    gKeyedOnVoices[1] = 0;
    gKeyedOffVoices[0] = 0;
    gKeyedOffVoices[1] = 0;
    gReverbVoices[0] = 0;
    gReverbVoices[1] = 0;
    gExternVoices[0] = 0;
    gExternVoices[1] = 0;
    gChannelMode = 0;
    gReverbMode = 0;
    gPlayingListHead = NULL;
    for (int i = 0; i < 16; i++) {
        gVoiceRanges[i].min = 0;
        gVoiceRanges[i].max = 47;
    }

    sema.attr = SA_THPRI;
    sema.initial = 0;
    sema.max = 1;
    gLockBlockerSema = CreateSema(&sema);
    SignalSema(gLockBlockerSema);
}

/* 00033708 0003374c */ void snd_CleanupVoiceAllocator() {
    DeleteSema(gLockBlockerSema);
    gLockBlockerSema = -1;
}

/* 0003374c 000338c0 */ void snd_SetMixerMode(/* 0x0(sp) */ SInt32 channel_mode, /* 0x4(sp) */ SInt32 reverb_mode) {
    /* -0x10(sp) */ SInt32 count;
    snd_SetReverbMode(reverb_mode);
    gChannelMode = channel_mode;

    if (channel_mode == 1) {
        for (int i = 0; i < 16; i++) {
            gVoiceRanges[i].min = 24;
            gVoiceRanges[i].max = 47;
        }
        gVoiceRanges[1].min = 0;
        gVoiceRanges[1].max = 23;
    } else {
        for (int i = 0; i < 16; i++) {
            gVoiceRanges[i].min = 0;
            gVoiceRanges[i].max = 47;
        }
    }
}

/* 000338c0 0003396c */ void snd_SetGroupVoiceRange(/* 0x0(sp) */ SInt32 group, /* 0x4(sp) */ SInt32 min, /* 0x8(sp) */ SInt32 max) {
    if (min < 0) {
        min = 0;
    }
    if (max >= 48) {
        max = 47;
    }
    gVoiceRanges[group].min = min;
    gVoiceRanges[group].max = max;
}

/* 0003396c 00033a50 */ SInt32 snd_LockVoiceAllocatorEx(/* 0x0(sp) */ bool block, /* 0x4(sp) */ UInt32 ownerID) {
    /* -0x10(sp) */ SInt32 ws_ret;
    if (block) {
        ws_ret = WaitSema(gLockBlockerSema);
        if (ws_ret) {
            snd_ShowError(80, ws_ret, 0, 0, 0);
            return 0;
        }
    } else {
        if (PollSema(gLockBlockerSema) == -419) {
            return 0;
        }
    }

    gVAllocOwnerID = ownerID;
    gVoiceAllocatorInUse = 1;

    return 1;
}

/* 00033a50 00033a88 */ SInt32 snd_IsVoiceAllocatorLocked() {
    return gVoiceAllocatorInUse;
}

/* 00033a88 00033aec */ SInt32 snd_CheckAllocatorCueTick() {
    if (!gVoiceAllocatorInUse) {
        return 0;
    }
    gTickAttentionNeeded++;
    return 1;
}

/* 00033aec 00033b34 */ void snd_UnlockVoiceAllocator() {
    gVoiceAllocatorInUse = 0;
    gVAllocOwnerID = 0;
    SignalSema(gLockBlockerSema);
}

/* 00033b34 00033c9c */ SInt32 snd_ExternVoiceAlloc(/* 0x0(sp) */ SInt32 vol_group, /* 0x4(sp) */ SInt32 priority) {
    /* -0x18(sp) */ SInt32 voice;
    /* -0x14(sp) */ UInt32 core;
    /* -0x10(sp) */ UInt32 c_v;
}

/* 00033c9c 00033d8c */ void snd_ExternVoiceFree(/* 0x0(sp) */ SInt32 voice) {
    /* -0x10(sp) */ UInt32 core;
    /* -0xc(sp) */ UInt32 c_v;
}

/* 00033d8c 00033f60 */ void snd_SetVoiceNoiseOrWave(/* 0x0(sp) */ SInt32 voice, /* 0x4(sp) */ bool wave) {
    /* -0x18(sp) */ SInt32 intr_state;
    /* -0x14(sp) */ SInt32 dis;
    /* -0x10(sp) */ UInt32 core;
    /* -0xc(sp) */ UInt32 c_v;
}

/* 00033f60 00034294 */ SInt32 snd_AllocateNoiseVoice(/* 0x0(sp) */ SInt32 vol_group, /* 0x4(sp) */ SInt32 priority) {
    /* -0x18(sp) */ SInt32 range_temp;
    /* -0x14(sp) */ SInt32 voice;
    /* -0x10(sp) */ SInt32 intr_state;
    /* -0xc(sp) */ SInt32 dis;
}

/* 00034294 000348d8 */ SInt32 snd_AllocateVoice(/* 0x0(sp) */ SInt32 vol_group, /* 0x4(sp) */ SInt32 priority) {
    /* -0x18(sp) */ SInt32 count;
    /* -0x14(sp) */ SInt32 start;
    /* -0x10(sp) */ SInt32 end;
    /* -0xc(sp) */ SInt32 lowest_priority;
}

/* 000348d8 00035254 */ void snd_StartVAGVoice(/* 0x0(sp) */ SInt32 voice, /* 0x4(sp) */ bool noise) {
    /* -0x30(sp) */ SInt32 note;
    /* -0x2c(sp) */ SInt32 fine;
    /* -0x28(sp) */ UInt32 pitch;
    /* -0x24(sp) */ SInt32 intr_state;
    /* -0x20(sp) */ SInt32 dis;
    /* -0x1c(sp) */ UInt32 flags;
    /* -0x18(sp) */ UInt32 core;
    /* -0x14(sp) */ UInt32 c_v;
    /* -0x10(sp) */ SInt32 clock;
}

/* 00035254 000353c0 */ void snd_StartVoices(/* 0x0(sp) */ struct VoiceFlags *voices) {
    /* -0x10(sp) */ SInt32 count;
}

/* 000353c0 0003555c */ void snd_StartPendingVoices() {
    /* -0x10(sp) */ SInt32 intr_state;
    /* -0xc(sp) */ SInt32 dis;
}

/* 0003555c 000356a8 */ void snd_StopPendingVoices() {
    /* -0x10(sp) */ SInt32 intr_state;
    /* -0xc(sp) */ SInt32 dis;
}

/* 000356a8 000356f4 */ void snd_GetKeyedOnVoices(/* 0x0(sp) */ struct VoiceFlags *voices) {}
/* 000356f4 00035740 */ void snd_GetKeyedOffVoices(/* 0x0(sp) */ struct VoiceFlags *voices) {}
/* 00035740 00035908 */ void snd_VoiceIsDone(/* 0x0(sp) */ SInt32 voice) {}
/* 00035908 000359b8 */ UInt32 snd_GetVoiceAge(/* 0x0(sp) */ SInt32 voice) {}
/* 000359b8 00035a14 */ SInt32 snd_GetVoiceStatus(/* 0x0(sp) */ SInt32 voice) {}
/* 00035a14 00035a70 */ SInt32 snd_GetVoicePriority(/* 0x0(sp) */ SInt32 voice) {}
/* 00035a70 00035cdc */ void snd_KeyOffVoice(/* 0x0(sp) */ SInt32 voice) {
    /* -0x10(sp) */ SInt32 core;
    /* -0xc(sp) */ SInt32 c_v;
}

/* 00035cdc 00035fd0 */ void snd_KeyOffVoicesEx(/* 0x0(sp) */ struct VoiceFlags *voices, /* 0x4(sp) */ bool do_owner_proc) {
    /* -0x18(sp) */ SInt32 count;
    /* -0x14(sp) */ SInt32 core;
    /* -0x10(sp) */ SInt32 c_v;
}

/* 00035fd0 000362b0 */ void snd_SilenceVoicesEx(/* 0x0(sp) */ struct VoiceFlags *voices, /* 0x4(sp) */ bool do_owner_proc) {
    /* -0x20(sp) */ SInt32 count;
    /* -0x1c(sp) */ SInt32 core;
    /* -0x18(sp) */ SInt32 c_v;
    /* -0x14(sp) */ SInt32 intr_state;
    /* -0x10(sp) */ SInt32 dis;
}

/* 000362b0 00036470 */ void snd_PauseVoices(/* 0x0(sp) */ struct VoiceFlags *voices) {
    /* -0x20(sp) */ SInt32 count;
    /* -0x1c(sp) */ SInt32 core;
    /* -0x18(sp) */ SInt32 c_v;
    /* -0x14(sp) */ SInt32 intr_state;
    /* -0x10(sp) */ SInt32 dis;
}

/* 00036470 00036678 */ void snd_PauseVoicesOwnedWithOwner(/* 0x0(sp) */ GSoundHandlerPtr owner) {
    /* -0x20(sp) */ SInt32 count;
    /* -0x1c(sp) */ SInt32 core;
    /* -0x18(sp) */ SInt32 c_v;
    /* -0x14(sp) */ SInt32 intr_state;
    /* -0x10(sp) */ SInt32 dis;
}

/* 00036678 00036a74 */ void snd_UnPauseVoices(/* 0x0(sp) */ struct VoiceFlags *voices) {
    /* -0x28(sp) */ SInt32 note;
    /* -0x24(sp) */ SInt32 fine;
    /* -0x20(sp) */ SInt32 count;
    /* -0x1c(sp) */ SInt32 core;
    /* -0x18(sp) */ SInt32 c_v;
    /* -0x14(sp) */ UInt32 pitch;
    /* -0x10(sp) */ SInt32 intr_state;
    /* -0xc(sp) */ SInt32 dis;
}

/* 00036a74 00036eb8 */ void snd_UnPauseVoicesOwnedWithOwner(/* 0x0(sp) */ GSoundHandlerPtr owner) {
    /* -0x28(sp) */ SInt32 note;
    /* -0x24(sp) */ SInt32 fine;
    /* -0x20(sp) */ SInt32 count;
    /* -0x1c(sp) */ SInt32 core;
    /* -0x18(sp) */ SInt32 c_v;
    /* -0x14(sp) */ UInt32 pitch;
    /* -0x10(sp) */ SInt32 intr_state;
    /* -0xc(sp) */ SInt32 dis;
}

/* 00036eb8 00036f0c */ struct VoiceAttributes *snd_GetVoiceStatusPtr(/* 0x0(sp) */ SInt32 voice) {}
/* 00036f0c 0003702c */ UInt16 PS1Note2Pitch(/* -0x18(sp) */ SInt8 center_note, /* -0x17(sp) */ SInt8 center_fine, /* -0x16(sp) */ UInt16 note, /* -0x14(sp) */ SInt16 fine) {
    /* -0x10(sp) */ UInt32 pitch;
    /* -0xc(sp) */ bool ps1;
}

/* 0003702c 000371a0 */ void snd_PitchBendTone(/* 0x0(sp) */ TonePtr tone, /* 0x4(sp) */ SInt32 pb, /* 0x8(sp) */ SInt32 pm, /* 0xc(sp) */ SInt32 note, /* 0x10(sp) */ SInt32 fine, /* 0x14(sp) */ SInt32 *new_note, /* 0x18(sp) */ SInt32 *new_fine) {
    /* -0x10(sp) */ SInt32 pitch;
}

/* 000371a0 00037338 */ void snd_MarkVoicePlaying(/* 0x0(sp) */ SInt32 voice) {
    /* -0x10(sp) */ struct VoiceAttributes *va;
    /* -0xc(sp) */ struct VoiceAttributes *walk;
}

/* 00037338 000374fc */ void snd_MarkVoiceFree(/* 0x0(sp) */ SInt32 voice) {
    /* -0x18(sp) */ struct VoiceAttributes *va;
    /* -0x14(sp) */ UInt32 core;
    /* -0x10(sp) */ struct VoiceAttributes *walk;
}

/* 000374fc 00037534 */ void snd_HardFreeVoice(/* 0x0(sp) */ SInt32 voice) {}
/* 00037534 00037684 */ void snd_KeyOnVoiceRaw(/* 0x0(sp) */ int core, /* 0x4(sp) */ int voice, /* 0x8(sp) */ bool reverb) {}
/* 00037684 00037760 */ void snd_KeyOffVoiceRaw(/* 0x0(sp) */ int core, /* 0x4(sp) */ int voice) {}
