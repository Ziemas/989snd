#include "valloc.h"
#include "989snd.h"
#include "reverb.h"
#include "stick.h"
#include "types.h"
#include "vol.h"

#include <intrman.h>
#include <libsd.h>

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
    for (count = 0; count < NUM_VOICES; count++) {
        snd_MarkVoiceFree(count);
        gChannelStatus[count].voice = count;
        gAutoFreeState[count] = 0;
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
    for (count = 0; count < 16; count++) {
        gVoiceRanges[count].min = 0;
        gVoiceRanges[count].max = 47;
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
        for (count = 0; count < 16; count++) {
            gVoiceRanges[count].min = 24;
            gVoiceRanges[count].max = 47;
        }
        gVoiceRanges[1].min = 0;
        gVoiceRanges[1].max = 23;
    } else {
        for (count = 0; count < 16; count++) {
            gVoiceRanges[count].min = 0;
            gVoiceRanges[count].max = 47;
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

    if (vol_group >= 16) {
        vol_group = 15;
    }
    voice = snd_AllocateVoice(vol_group, priority);
    if (voice >= 0) {
        core = voice / 24;
        c_v = voice % 24;
        gChannelStatus[voice].Status = 3;
        gExternVoices[core] |= 1 << c_v;
    }

    return voice;
}

/* 00033c9c 00033d8c */ void snd_ExternVoiceFree(/* 0x0(sp) */ SInt32 voice) {
    /* -0x10(sp) */ UInt32 core = voice / 24;
    /* -0xc(sp) */ UInt32 c_v = voice % 24;
    snd_MarkVoiceFree(voice);
    gExternVoices[core] &= ~(1 << c_v);
}

/* 00033d8c 00033f60 */ void snd_SetVoiceNoiseOrWave(/* 0x0(sp) */ SInt32 voice, /* 0x4(sp) */ bool wave) {
    /* -0x18(sp) */ SInt32 intr_state;
    /* -0x14(sp) */ SInt32 dis;
    /* -0x10(sp) */ UInt32 core = voice / 24;
    /* -0xc(sp) */ UInt32 c_v = voice % 24;

    dis = CpuSuspendIntr(&intr_state);
    sceSdSetParam(SD_VPARAM_VOLL | SD_VOICE(core, c_v), 0);
    sceSdSetParam(SD_VPARAM_VOLR | SD_VOICE(core, c_v), 0);

    if (wave) {
        sceSdSetSwitch(SD_SWITCH_NON | core,
                       sceSdGetSwitch(SD_SWITCH_NON | core) & ~(1 << c_v));
    } else {
        sceSdSetSwitch(SD_SWITCH_NON | core,
                       sceSdGetSwitch(SD_SWITCH_NON | core) | (1 << c_v));
    }

    if (!dis) {
        CpuResumeIntr(intr_state);
    }
}

/* 00033f60 00034294 */ SInt32 snd_AllocateNoiseVoice(/* 0x0(sp) */ SInt32 vol_group, /* 0x4(sp) */ SInt32 priority) {
    /* -0x18(sp) */ SInt32 range_temp = -1;
    /* -0x14(sp) */ SInt32 voice = -1;
    /* -0x10(sp) */ SInt32 intr_state;
    /* -0xc(sp) */ SInt32 dis;
    dis = CpuSuspendIntr(&intr_state);

    if (gNoiseOwnerPriority[0] < priority && gVoiceRanges[vol_group].min < 24) {
        if (gVoiceRanges[vol_group].max >= 24) {
            range_temp = gVoiceRanges[vol_group].max;
            gVoiceRanges[vol_group].max = 23;
        }
        voice = snd_AllocateVoice(vol_group, priority);
        if (range_temp >= 0) {
            gVoiceRanges[vol_group].max = range_temp;
        }
        if (voice != -1) {
            snd_SetVoiceNoiseOrWave(voice, 0);
            gNoiseOwnerPriority[0] = priority;
            gNoiseOwnerVoice[0] = voice;
        }
    }

    if (voice == -1 && gNoiseOwnerPriority[1] < priority && gVoiceRanges[vol_group].max >= 24) {
        if (gVoiceRanges[vol_group].min < 24) {
            range_temp = gVoiceRanges[vol_group].min;
            gVoiceRanges[vol_group].min = 24;
        }
        voice = snd_AllocateVoice(vol_group, priority);
        if (range_temp >= 0) {
            gVoiceRanges[vol_group].min = range_temp;
        }
        if (voice != -1) {
            snd_SetVoiceNoiseOrWave(voice, 0);
            gNoiseOwnerPriority[1] = priority;
            gNoiseOwnerVoice[1] = voice;
        }
    }

    if (!dis) {
        CpuResumeIntr(intr_state);
    }

    return voice;
}

/* 00034294 000348d8 */ SInt32 snd_AllocateVoice(/* 0x0(sp) */ SInt32 vol_group, /* 0x4(sp) */ SInt32 priority) {
    /* -0x18(sp) */ SInt32 count;
    /* -0x14(sp) */ SInt32 start;
    /* -0x10(sp) */ SInt32 end;
    /* -0xc(sp) */ SInt32 lowest_priority;

    if (vol_group >= 16) {
        vol_group = 15;
    }

    // search for a free voice
    end = gVoiceRanges[vol_group].max + 1;
    start = gVoiceRanges[vol_group].min;
    for (count = start; count < end; ++count) {
        if (!gChannelStatus[count].Status) {
            gChannelStatus[count].Status = 2;
            gChannelStatus[count].StartTick = snd_GetTick();
            gChannelStatus[count].Priority = priority;
            gChannelStatus[count].VolGroup = vol_group;
            gAutoFreeState[count] = 4;
            snd_SetVoiceNoiseOrWave(count, 1);
            return count;
        }
    }

    // no free voices, find a lower priority voice to replace
    lowest_priority = start;
    for (count = start + 1; count < end; ++count) {
        if (gChannelStatus[count].Status != 3 &&
            (gChannelStatus[count].Priority < (unsigned int)gChannelStatus[lowest_priority].Priority ||
             (gChannelStatus[count].Priority == gChannelStatus[lowest_priority].Priority && gChannelStatus[count].StartTick < gChannelStatus[lowest_priority].StartTick))) {
            lowest_priority = count;
        }
    }
    if (priority < gChannelStatus[lowest_priority].Priority || gChannelStatus[lowest_priority].Priority >= 0x7Eu || gChannelStatus[lowest_priority].Status == 3) {
        return -1;
    }

    if (gChannelStatus[lowest_priority].OwnerProc) {
        gChannelStatus[lowest_priority].OwnerProc(lowest_priority, (UInt32)gChannelStatus[lowest_priority].Owner, 1);
    }

    snd_MarkVoiceFree(lowest_priority);
    gChannelStatus[lowest_priority].Status = 2;
    gChannelStatus[lowest_priority].StartTick = snd_GetTick();
    gChannelStatus[lowest_priority].Priority = priority;
    gChannelStatus[lowest_priority].VolGroup = vol_group;
    gChannelStatus[lowest_priority].OwnerProc = 0;
    gAutoFreeState[lowest_priority] = 4;
    snd_SetVoiceNoiseOrWave(lowest_priority, 1);
    return lowest_priority;
}

/* 000348d8 00035254 */ void snd_StartVAGVoice(/* 0x0(sp) */ SInt32 voice, /* 0x4(sp) */ bool noise) {
    /* -0x30(sp) */ SInt32 note;
    /* -0x2c(sp) */ SInt32 fine;
    /* -0x28(sp) */ UInt32 pitch;
    /* -0x24(sp) */ SInt32 intr_state;
    /* -0x20(sp) */ SInt32 dis;
    ///* -0x1c(sp) */ UInt32 flags;
    /* -0x18(sp) */ UInt32 core = voice / 24;
    /* -0x14(sp) */ UInt32 c_v = voice % 24;
    /* -0x10(sp) */ SInt32 clock;

    gChannelStatus[voice].StartTick = snd_GetTick();
    dis = CpuSuspendIntr(&intr_state);
    sceSdSetParam(SD_VPARAM_VOLL | SD_VOICE(core, c_v),
                  snd_AdjustVolToGroup(gChannelStatus[voice].Volume.left, gChannelStatus[voice].VolGroup) >> 1);
    sceSdSetParam(SD_VPARAM_VOLR | SD_VOICE(core, c_v),
                  snd_AdjustVolToGroup(gChannelStatus[voice].Volume.right, gChannelStatus[voice].VolGroup) >> 1);

    if ((gChannelStatus[voice].Tone->Flags & TONE_REVERB_ONLY) != 0) {
        sceSdSetSwitch(SD_SWITCH_VMIXL | core, sceSdGetSwitch(SD_SWITCH_VMIXL | core) & ~(1 << c_v));
        sceSdSetSwitch(SD_SWITCH_VMIXR | core, sceSdGetSwitch(SD_SWITCH_VMIXR | core) & ~(1 << c_v));
    } else {
        sceSdSetSwitch(SD_SWITCH_VMIXL | core, sceSdGetSwitch(SD_SWITCH_VMIXL | core) | (1 << c_v));
        sceSdSetSwitch(SD_SWITCH_VMIXR | core, sceSdGetSwitch(SD_SWITCH_VMIXR | core) | (1 << c_v));
    }

    if (noise) {
        clock = gChannelStatus[voice].Tone->CenterNote + 32 * gChannelStatus[voice].Current_PB / 0x7fff;
        if (clock < 0) {
            clock = 0;
        }
        if (clock >= 64) {
            clock = 63;
        }
        sceSdSetSwitch(SD_SWITCH_NON | core, sceSdGetSwitch(SD_SWITCH_NON | core) | (1 << c_v));
        sceSdSetCoreAttr(SD_CORE_NOISE_CLK | core, clock);
    } else {
        sceSdSetSwitch(SD_SWITCH_NON | core, sceSdGetSwitch(SD_SWITCH_NON | core) & ~(1 << c_v));
        snd_PitchBendTone(gChannelStatus[voice].Tone,
                          gChannelStatus[voice].Current_PB,
                          gChannelStatus[voice].Current_PM,
                          gChannelStatus[voice].StartNote,
                          gChannelStatus[voice].StartFine,
                          &note, &fine);
        pitch = PS1Note2Pitch(gChannelStatus[voice].Tone->CenterNote, gChannelStatus[voice].Tone->CenterFine, note, fine);
        sceSdSetParam(SD_VPARAM_PITCH | SD_VOICE(core, c_v), pitch);
        sceSdSetAddr(SD_VADDR_SSA | SD_VOICE(core, c_v), (UInt32)gChannelStatus[voice].Tone->VAGInSR);
    }

    sceSdSetParam(SD_VPARAM_ADSR1 | SD_VOICE(core, c_v), gChannelStatus[voice].Tone->ADSR1);
    sceSdSetParam(SD_VPARAM_ADSR2 | SD_VOICE(core, c_v), gChannelStatus[voice].Tone->ADSR2);

    if (!dis) {
        CpuResumeIntr(intr_state);
    }

    if ((gChannelStatus[voice].Tone->Flags & TONE_REVERB) != 0) {
        gReverbVoices[core] |= (1 << c_v);
    } else {
        gReverbVoices[core] &= ~(1 << c_v);
    }
    gAwaitingKeyOn[core] |= (1 << c_v);
    snd_MarkVoicePlaying(voice);
}

/* 00035254 000353c0 */ void snd_StartVoices(/* 0x0(sp) */ struct VoiceFlags *voices) {
    /* -0x10(sp) */ SInt32 count;
    for (count = 0; count < NUM_VOICES; count++) {
        if ((count < 24 && ((voices->core[0] & (1 << count)) != 0)) ||
            (count >= 24 && ((voices->core[1] & (1 << (count - 24))) != 0))) {
            snd_MarkVoicePlaying(count);
            gChannelStatus[count].StartTick = snd_GetTick();
        }
    }

    gAwaitingKeyOn[0] |= voices->core[0];
    gAwaitingKeyOn[1] |= voices->core[1];
}

/* 000353c0 0003555c */ void snd_StartPendingVoices() {
    /* -0x10(sp) */ SInt32 intr_state;
    /* -0xc(sp) */ SInt32 dis;

    if (gAwaitingKeyOn[0] || gAwaitingKeyOn[1]) {
        dis = CpuSuspendIntr(&intr_state);
        sceSdSetSwitch(SD_SWITCH_VMIXEL, gReverbVoices[0]);
        sceSdSetSwitch(SD_SWITCH_VMIXER, gReverbVoices[0]);
        sceSdSetSwitch(SD_SWITCH_VMIXEL | 1, gReverbVoices[1]);
        sceSdSetSwitch(SD_SWITCH_VMIXER | 1, gReverbVoices[1]);
        sceSdSetSwitch(SD_SWITCH_KON, gAwaitingKeyOn[0]);
        sceSdSetSwitch(SD_SWITCH_KON | 1, gAwaitingKeyOn[1]);

        if (!dis) {
            CpuResumeIntr(intr_state);
        }

        gKeyedOffVoices[0] &= ~gAwaitingKeyOn[0];
        gKeyedOffVoices[1] &= ~gAwaitingKeyOn[1];
        gKeyedOnVoices[0] |= gAwaitingKeyOn[0];
        gKeyedOnVoices[1] |= gAwaitingKeyOn[1];
        gAwaitingKeyOn[0] = 0;
        gAwaitingKeyOn[1] = 0;
    }
}

/* 0003555c 000356a8 */ void snd_StopPendingVoices() {
    /* -0x10(sp) */ SInt32 intr_state;
    /* -0xc(sp) */ SInt32 dis;

    if (gAwaitingKeyOff[0] || gAwaitingKeyOff[1]) {
        dis = CpuSuspendIntr(&intr_state);
        sceSdSetSwitch(SD_SWITCH_KOFF, gAwaitingKeyOff[0]);
        sceSdSetSwitch(SD_SWITCH_KOFF | 1, gAwaitingKeyOff[1]);

        if (!dis) {
            CpuResumeIntr(intr_state);
        }

        gKeyedOnVoices[0] &= ~gAwaitingKeyOff[0];
        gKeyedOnVoices[1] &= ~gAwaitingKeyOff[1];
        gKeyedOffVoices[0] |= gAwaitingKeyOff[0];
        gKeyedOffVoices[1] |= gAwaitingKeyOff[1];
        gAwaitingKeyOff[1] = 0;
        gAwaitingKeyOff[0] = 0;
    }
}

/* 000356a8 000356f4 */ void snd_GetKeyedOnVoices(/* 0x0(sp) */ struct VoiceFlags *voices) {
    voices->core[0] = gKeyedOnVoices[0];
    voices->core[1] = gKeyedOnVoices[1];
}

/* 000356f4 00035740 */ void snd_GetKeyedOffVoices(/* 0x0(sp) */ struct VoiceFlags *voices) {
    voices->core[0] = gKeyedOffVoices[0];
    voices->core[1] = gKeyedOffVoices[1];
}

/* 00035740 00035908 */ void snd_VoiceIsDone(/* 0x0(sp) */ SInt32 voice) {
    if (gChannelStatus[voice].Priority != VOICE_PRIORITY_PERM) {
        snd_MarkVoiceFree(voice);
    }
    if (gChannelStatus[voice].OwnerProc) {
        gChannelStatus[voice].OwnerProc(voice, (UInt32)gChannelStatus[voice].Owner, 2);
    }
    gChannelStatus[voice].OwnerProc = NULL;
    gKeyedOffVoices[voice / 24] &= ~(1 << (voice % 24));
}

/* 00035908 000359b8 */ UInt32 snd_GetVoiceAge(/* 0x0(sp) */ SInt32 voice) {
    if (gChannelStatus[voice].Status == 1) {
        return snd_GetTick() - gChannelStatus[voice].StartTick;
    }

    return 0;
}

/* 000359b8 00035a14 */ SInt32 snd_GetVoiceStatus(/* 0x0(sp) */ SInt32 voice) {
    return gChannelStatus[voice].Status;
}

/* 00035a14 00035a70 */ SInt32 snd_GetVoicePriority(/* 0x0(sp) */ SInt32 voice) {
    return gChannelStatus[voice].Priority;
}

/* 00035a70 00035cdc */ void snd_KeyOffVoice(/* 0x0(sp) */ SInt32 voice) {
    /* -0x10(sp) */ SInt32 core = voice / 24;
    /* -0xc(sp) */ SInt32 c_v = voice % 24;

    if (gChannelStatus[voice].Status == 1 && gChannelStatus[voice].OwnerProc) {
        gChannelStatus[voice].OwnerProc(voice, (UInt32)gChannelStatus[voice].Owner, 3);
    }
    gChannelStatus[voice].OwnerProc = NULL;
    if ((gAwaitingKeyOn[core] & (1 << c_v)) != 0) {
        gAwaitingKeyOn[core] &= ~(1 << c_v);
    }
    gAwaitingKeyOff[core] |= (1 << c_v);
}

/* 00035cdc 00035fd0 */ void snd_KeyOffVoicesEx(/* 0x0(sp) */ struct VoiceFlags *voices, /* 0x4(sp) */ bool do_owner_proc) {
    /* -0x18(sp) */ SInt32 count;
    /* -0x14(sp) */ SInt32 core;
    /* -0x10(sp) */ SInt32 c_v;

    for (count = 0; count < NUM_VOICES; count++) {
        core = count / 24;
        c_v = count % 24;
        if (gChannelStatus[count].Status == 1 &&
            (voices->core[core] & (1 << c_v)) != 0) {
            gAwaitingKeyOff[core] |= (1 << c_v);
            if (do_owner_proc && gChannelStatus[count].OwnerProc) {
                gChannelStatus[count].OwnerProc(count, (UInt32)gChannelStatus[count].Owner, 3);
            }
            gChannelStatus[count].OwnerProc = NULL;
            if ((gAwaitingKeyOn[core] & (1 << c_v)) != 0) {
                gAwaitingKeyOn[core] &= ~(1 << c_v);
            }
        }
    }
}

/* 00035fd0 000362b0 */ void snd_SilenceVoicesEx(/* 0x0(sp) */ struct VoiceFlags *voices, /* 0x4(sp) */ bool do_owner_proc) {
    /* -0x20(sp) */ SInt32 count;
    /* -0x1c(sp) */ SInt32 core;
    /* -0x18(sp) */ SInt32 c_v;
    /* -0x14(sp) */ SInt32 intr_state;
    /* -0x10(sp) */ SInt32 dis;

    for (count = 0; count < NUM_VOICES; count++) {
        core = count / 24;
        c_v = count % 24;

        if ((voices->core[core] & (1 << c_v)) != 0) {
            if (do_owner_proc && gChannelStatus[count].OwnerProc) {
                gChannelStatus[count].OwnerProc(count, (UInt32)gChannelStatus[count].Owner, 3);
            }
        }

        gChannelStatus[count].OwnerProc = NULL;
        if (gChannelStatus[count].Priority != VOICE_PRIORITY_PERM) {
            snd_MarkVoiceFree(count);
        }

        dis = CpuSuspendIntr(&intr_state);
        sceSdSetParam(SD_VPARAM_VOLL | SD_VOICE(core, c_v), 0);
        sceSdSetParam(SD_VPARAM_VOLR | SD_VOICE(core, c_v), 0);
        if (!dis) {
            CpuResumeIntr(intr_state);
        }

        snd_KeyOffVoiceRaw(core, c_v);
    }
}

/* 000362b0 00036470 */ void snd_PauseVoices(/* 0x0(sp) */ struct VoiceFlags *voices) {
    /* -0x20(sp) */ SInt32 count;
    /* -0x1c(sp) */ SInt32 core;
    /* -0x18(sp) */ SInt32 c_v;
    /* -0x14(sp) */ SInt32 intr_state;
    /* -0x10(sp) */ SInt32 dis;

    for (count = 0; count < NUM_VOICES; count++) {
        core = count / 24;
        c_v = count % 24;

        if ((voices->core[core] & (1 << c_v)) != 0) {
            dis = CpuSuspendIntr(&intr_state);
            sceSdSetParam(SD_VPARAM_VOLL | SD_VOICE(core, c_v), 0);
            sceSdSetParam(SD_VPARAM_VOLR | SD_VOICE(core, c_v), 0);
            sceSdSetParam(SD_VPARAM_PITCH | SD_VOICE(core, c_v), 0);
            if (!dis) {
                CpuResumeIntr(intr_state);
            }
        }
    }
}

/* 00036470 00036678 */ void snd_PauseVoicesOwnedWithOwner(/* 0x0(sp) */ GSoundHandlerPtr owner) {
    /* -0x20(sp) */ SInt32 count;
    /* -0x1c(sp) */ SInt32 core;
    /* -0x18(sp) */ SInt32 c_v;
    /* -0x14(sp) */ SInt32 intr_state;
    /* -0x10(sp) */ SInt32 dis;

    snd_LockVoiceAllocatorEx(1, 122);
    for (count = 0; count < NUM_VOICES; count++) {
        core = count / 24;
        c_v = count % 24;

        if (gChannelStatus[count].Status && gChannelStatus[count].Owner == owner) {
            dis = CpuSuspendIntr(&intr_state);
            sceSdSetParam(SD_VPARAM_VOLL | SD_VOICE(core, c_v), 0);
            sceSdSetParam(SD_VPARAM_VOLR | SD_VOICE(core, c_v), 0);
            sceSdSetParam(SD_VPARAM_PITCH | SD_VOICE(core, c_v), 0);
            if (!dis) {
                CpuResumeIntr(intr_state);
            }
        }
    }
    snd_UnlockVoiceAllocator();
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

    for (count = 0; count < NUM_VOICES; count++) {
        core = count / 24;
        c_v = count % 24;

        if ((voices->core[core] & (1 << c_v)) != 0) {
            dis = CpuSuspendIntr(&intr_state);

            sceSdSetParam(SD_VPARAM_VOLL | SD_VOICE(core, c_v),
                          snd_AdjustVolToGroup(gChannelStatus[count].Volume.left, gChannelStatus[count].VolGroup) >> 1);
            sceSdSetParam(SD_VPARAM_VOLR | SD_VOICE(core, c_v),
                          snd_AdjustVolToGroup(gChannelStatus[count].Volume.right, gChannelStatus[count].VolGroup) >> 1);
            snd_PitchBendTone(gChannelStatus[count].Tone,
                              gChannelStatus[count].Current_PB,
                              gChannelStatus[count].Current_PM,
                              gChannelStatus[count].StartNote,
                              gChannelStatus[count].StartFine,
                              &note,
                              &fine);
            pitch = PS1Note2Pitch(gChannelStatus[count].Tone->CenterNote, gChannelStatus[count].Tone->CenterFine, note, fine);
            sceSdSetParam(SD_VPARAM_PITCH | SD_VOICE(core, c_v), pitch);

            if (!dis) {
                CpuResumeIntr(intr_state);
            }
        }
    }
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

    snd_LockVoiceAllocatorEx(true, 123);
    for (count = 0; count < NUM_VOICES; count++) {
        core = count / 24;
        c_v = count % 24;

        if (gChannelStatus[count].Status && gChannelStatus[count].Owner == owner) {
            dis = CpuSuspendIntr(&intr_state);

            sceSdSetParam(SD_VPARAM_VOLL | SD_VOICE(core, c_v),
                          snd_AdjustVolToGroup(gChannelStatus[count].Volume.left, gChannelStatus[count].VolGroup) >> 1);
            sceSdSetParam(SD_VPARAM_VOLR | SD_VOICE(core, c_v),
                          snd_AdjustVolToGroup(gChannelStatus[count].Volume.right, gChannelStatus[count].VolGroup) >> 1);
            snd_PitchBendTone(gChannelStatus[count].Tone,
                              gChannelStatus[count].Current_PB,
                              gChannelStatus[count].Current_PM,
                              gChannelStatus[count].StartNote,
                              gChannelStatus[count].StartFine,
                              &note,
                              &fine);
            pitch = PS1Note2Pitch(gChannelStatus[count].Tone->CenterNote, gChannelStatus[count].Tone->CenterFine, note, fine);
            sceSdSetParam(SD_VPARAM_PITCH | SD_VOICE(core, c_v), pitch);

            if (!dis) {
                CpuResumeIntr(intr_state);
            }
        }
    }
    snd_UnlockVoiceAllocator();
}

/* 00036eb8 00036f0c */ struct VoiceAttributes *snd_GetVoiceStatusPtr(/* 0x0(sp) */ SInt32 voice) {
    return &gChannelStatus[voice];
}

/* 00036f0c 0003702c */ UInt16 PS1Note2Pitch(/* -0x18(sp) */ SInt8 center_note, /* -0x17(sp) */ SInt8 center_fine, /* -0x16(sp) */ UInt16 note, /* -0x14(sp) */ SInt16 fine) {
    /* -0x10(sp) */ UInt32 pitch;
    /* -0xc(sp) */ bool ps1;

    if (center_note >= 0) {
        ps1 = true;
    } else {
        ps1 = false;
        center_note = -center_note;
    }
    pitch = sceSdNote2Pitch(center_note, center_fine, note, fine);
    if (ps1) {
        return 44100 * pitch / 48000;
    }

    return pitch;
}

/* 0003702c 000371a0 */ void snd_PitchBendTone(/* 0x0(sp) */ TonePtr tone, /* 0x4(sp) */ SInt32 pb, /* 0x8(sp) */ SInt32 pm, /* 0xc(sp) */ SInt32 note, /* 0x10(sp) */ SInt32 fine, /* 0x14(sp) */ SInt32 *new_note, /* 0x18(sp) */ SInt32 *new_fine) {
    /* -0x10(sp) */ SInt32 pitch;

    pitch = (note << 7) + fine + pm;
    if (pb >= 0) {
        *new_note = (tone->PBHigh * (pb << 7) / 0x7FFF + pitch) / 128;
        *new_fine = (tone->PBHigh * (pb << 7) / 0x7FFF + pitch) % 128;
    } else {
        *new_note = (tone->PBLow * (pb << 7) / 0x8000 + pitch) / 128;
        *new_fine = (tone->PBLow * (pb << 7) / 0x8000 + pitch) % 128;
    }
}

/* 000371a0 00037338 */ void snd_MarkVoicePlaying(/* 0x0(sp) */ SInt32 voice) {
    /* -0x10(sp) */ struct VoiceAttributes *va;
    /* -0xc(sp) */ struct VoiceAttributes *walk;
    va = &gChannelStatus[voice];
    va->Status = 1;
    va->playlist = NULL;
    if (gPlayingListHead != NULL) {
        // TODO funky decompiler output, take a look
        walk = gPlayingListHead;
        if (va == gPlayingListHead) {
            snd_ShowError(82, 0, 0, 0, 0);
            return;
        }

        while (walk->playlist != NULL && walk->playlist != va) {
            walk = walk->playlist;
        }

        if (walk->playlist == va) {
            snd_ShowError(82, 0, 0, 0, 0);
            return;
        } else {
            walk->playlist = va;
        }
    } else {
        gPlayingListHead = va;
    }
}

/* 00037338 000374fc */ void snd_MarkVoiceFree(/* 0x0(sp) */ SInt32 voice) {
    /* -0x18(sp) */ struct VoiceAttributes *va;
    /* -0x14(sp) */ UInt32 core = voice / 24;
    /* -0x10(sp) */ struct VoiceAttributes *walk;
    va = &gChannelStatus[voice];
    va->Status = 0;
    va->Owner = NULL;

    if (va == gPlayingListHead) {
        gPlayingListHead = va->playlist;
    } else {
        walk = gPlayingListHead;
        while (walk != NULL && walk->playlist != va) {
            walk = walk->playlist;
        }

        if (walk) {
            walk->playlist = va->playlist;
        }
    }

    if (gNoiseOwnerVoice[core] == voice) {
        gNoiseOwnerVoice[core] = -1;
        gNoiseOwnerPriority[core] = -1;
    }
}

/* 000374fc 00037534 */ void snd_HardFreeVoice(/* 0x0(sp) */ SInt32 voice) {
    snd_MarkVoiceFree(voice);
}

/* 00037534 00037684 */ void snd_KeyOnVoiceRaw(/* 0x0(sp) */ int core, /* 0x4(sp) */ int voice, /* 0x8(sp) */ bool reverb) {
    gAwaitingKeyOn[core] |= 1 << voice;
    if (reverb) {
        gReverbVoices[core] |= 1 << voice;
    } else {
        gReverbVoices[core] &= ~(1 << voice);
    }
}

/* 00037684 00037760 */ void snd_KeyOffVoiceRaw(/* 0x0(sp) */ int core, /* 0x4(sp) */ int voice) {
    gAwaitingKeyOn[core] &= ~(1 << voice);
    gAwaitingKeyOff[core] |= 1 << voice;
}
