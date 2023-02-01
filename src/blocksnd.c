#include "blocksnd.h"
#include "989snd.h"
#include "LFO.h"
#include "intrman.h"
#include "libsd.h"
#include "my_rand.h"
#include "sndhand.h"
#include "stick.h"
#include "types.h"
#include "util.h"
#include "valloc.h"
#include "vol.h"

/* data 1208 */ SInt8 gBlockGlobalReg[32] = {};
/* data 1228 */ GrainHandler gGrainHandler[] = {
    snd_SFX_GRAIN_TYPE_NULL,
    snd_SFX_GRAIN_TYPE_TONE,
    snd_SFX_GRAIN_TYPE_XREF_ID,
    snd_SFX_GRAIN_TYPE_XREF_NUM,
    snd_SFX_GRAIN_TYPE_LFO_SETTINGS,
    snd_SFX_GRAIN_TYPE_STARTCHILDSOUND,
    snd_SFX_GRAIN_TYPE_STOPCHILDSOUND,
    snd_SFX_GRAIN_TYPE_PLUGIN_MESSAGE,
    snd_SFX_GRAIN_TYPE_BRANCH,
    snd_SFX_GRAIN_TYPE_TONE,
    snd_SFX_UNKNOWN_GRAIN_TYPE,
    snd_SFX_UNKNOWN_GRAIN_TYPE,
    snd_SFX_UNKNOWN_GRAIN_TYPE,
    snd_SFX_UNKNOWN_GRAIN_TYPE,
    snd_SFX_UNKNOWN_GRAIN_TYPE,
    snd_SFX_UNKNOWN_GRAIN_TYPE,
    snd_SFX_UNKNOWN_GRAIN_TYPE,
    snd_SFX_UNKNOWN_GRAIN_TYPE,
    snd_SFX_UNKNOWN_GRAIN_TYPE,
    snd_SFX_UNKNOWN_GRAIN_TYPE,
    snd_SFX_GRAIN_TYPE_CONTROL_NULL,
    snd_SFX_GRAIN_TYPE_LOOP_START,
    snd_SFX_GRAIN_TYPE_LOOP_END,
    snd_SFX_GRAIN_TYPE_LOOP_CONTINUE,
    snd_SFX_GRAIN_TYPE_STOP,
    snd_SFX_GRAIN_TYPE_RAND_PLAY,
    snd_SFX_GRAIN_TYPE_RAND_DELAY,
    snd_SFX_GRAIN_TYPE_RAND_PB,
    snd_SFX_GRAIN_TYPE_PB,
    snd_SFX_GRAIN_TYPE_ADD_PB,
    snd_SFX_GRAIN_TYPE_SET_REGISTER,
    snd_SFX_GRAIN_TYPE_SET_REGISTER_RAND,
    snd_SFX_GRAIN_TYPE_INC_REGISTER,
    snd_SFX_GRAIN_TYPE_DEC_REGISTER,
    snd_SFX_GRAIN_TYPE_TEST_REGISTER,
    snd_SFX_GRAIN_TYPE_MARKER,
    snd_SFX_GRAIN_TYPE_GOTO_MARKER,
    snd_SFX_GRAIN_TYPE_GOTO_RANDOM_MARKER,
    snd_SFX_GRAIN_TYPE_WAIT_FOR_ALL_VOICES,
    snd_SFX_GRAIN_TYPE_PLAY_CYCLE,
    snd_SFX_GRAIN_TYPE_ADD_REGISTER,
    snd_SFX_GRAIN_TYPE_KEY_OFF_VOICES,
    snd_SFX_GRAIN_TYPE_KILL_VOICES,
    snd_SFX_GRAIN_TYPE_ON_STOP_MARKER,
    snd_SFX_GRAIN_TYPE_COPY_REGISTER,
};

/* 00008e24 00008ebc */ UInt32 snd_PlaySFX(/* 0x0(sp) */ SFXBlock2Ptr block, /* 0x4(sp) */ SInt32 sound, /* 0x8(sp) */ SInt32 vol, /* 0xc(sp) */ SInt32 pan, /* 0x10(sp) */ int pitch_mod, /* 0x14(sp) */ int bend) {
    /* -0x10(sp) */ SInt16 _pitch_mod;
    /* -0xe(sp) */ SInt16 _bend;

    return snd_PlaySFX_EX(block, sound, vol, pan, pitch_mod, bend, -1, -1);
}

/* 00008ebc 000096f4 */ UInt32 snd_PlaySFXWithStruct(/* 0x0(sp) */ SFXBlock2Ptr block, /* 0x4(sp) */ SInt32 sound, /* -0x30(sp) */ SInt16 sfx_vol, /* -0x2e(sp) */ SInt16 sfx_pan, /* 0x10(sp) */ SndPlayParamsPtr params) {
    /* -0x2c(sp) */ SInt32 play_vol;
    /* -0x28(sp) */ SFX2 *sfx;
    /* -0x24(sp) */ BlockSoundHandlerPtr hand;
    /* -0x20(sp) */ SInt32 stop_sound;
    /* -0x1c(sp) */ SInt32 i;
    /* -0x18(sp) */ SInt32 vol;
    /* -0x14(sp) */ SInt32 pan;
    /* -0x10(sp) */ SInt16 pitch_mod;
    /* -0xe(sp) */ SInt16 bend;

    if (sound < 0 || sound >= block->NumSounds) {
        return 0;
    }

    sfx = &block->FirstSound[sound];
    sfx->Flags |= 0x4000;

    if (sfx->NumGrains == 0) {
        return 0;
    }

    if (sfx_vol == -1) {
        sfx_vol = sfx->Vol;
    }
    if (sfx_pan == -1) {
        sfx_pan = sfx->Pan;
    }

    if ((params->mask & 4) != 0) {
        vol = params->vol;
    } else {
        vol = 0x400;
    }

    if ((params->mask & 8) != 0) {
        pan = params->pan;
    } else {
        pan = -1;
    }

    if ((params->mask & 0x10) != 0) {
        pitch_mod = params->pitch_mod;
    } else {

        pitch_mod = 0;
    }
    if ((params->mask & 0x20) != 0) {
        bend = params->pitch_bend;
    } else {
        bend = 0;
    }

    if (vol == 0x7fffffff) {
        vol = 0x400;
    }

    play_vol = (sfx_vol * vol) >> 10;

    if (play_vol > 127) {
        play_vol = 127;
    }

    if (pan == -1 || pan == -2) {
        pan = sfx_pan;
    }

    if ((sfx->Flags & 2) != 0) {
        snd_StopAllHandlersForSound((SoundPtr)sfx, 0, 1);
    }

    snd_LockMasterTick(26);
    hand = snd_GetFreeBlockSoundHandler(sfx, vol, (params->mask & 0x80000000) == 0);
    if (hand == NULL) {
        snd_UnlockMasterTick();
        return 0;
    }

    hand->SH.Sound = (SoundPtr)sfx;
    hand->orig_sound = sfx;
    snd_RemoveLFOsForHandler(hand);
    snd_UnlockMasterTick();
    hand->SH.Effects = NULL;
    hand->SH.Original_Vol = sfx_vol;
    hand->SH.Original_Pan = sfx_pan;
    hand->SH.Current_Vol = play_vol;
    hand->SH.Current_Pan = pan;
    hand->SH.Current_PM = pitch_mod;
    hand->SH.flags = 0;
    hand->SH.VolGroup = sfx->VolGroup;
    hand->SH.Voices.core[0] = 0;
    hand->SH.Voices.core[1] = 0;
    hand->Current_Note = 60;
    hand->Current_Fine = 0;
    hand->Current_PB = bend;
    hand->LFO_Vol = 0;
    hand->App_Vol = vol;
    hand->LFO_Pan = 0;
    hand->App_Pan = pan;
    hand->LFO_PB = 0;
    hand->App_PB = bend;
    hand->LFO_PM = 0;
    hand->App_PM = pitch_mod;
    hand->CountDown = sfx->FirstGrain[0].Delay;
    hand->NextGrain = 0;

    if ((params->mask & 0x40) != 0) {
        hand->Registers[0] = params->reg[0];
        hand->Registers[1] = params->reg[1];
        hand->Registers[2] = params->reg[2];
        hand->Registers[3] = params->reg[3];
    } else {
        hand->Registers[0] = 0;
        hand->Registers[1] = 0;
        hand->Registers[2] = 0;
        hand->Registers[3] = 0;
    }

    hand->sk_doing_skipping_play = 0;
    hand->sk_grains_to_play = 0;
    hand->sk_grains_to_skip = 0;
    hand->block = block;
    for (i = 0; i < 4; i++) {
        hand->lfo[i].type = 0;
        hand->lfo[i].next_step = 0;
        hand->lfo[i].target = 0;
        hand->lfo[i].setup_flags = 0;
        hand->lfo[i].running_flags = 0;
        hand->lfo[i].handler = hand;
    }

    stop_sound = 0;
    while (stop_sound == 0 && hand->NextGrain != -1 && hand->CountDown == 0) {
        stop_sound = snd_DoGrain(hand);
    }

    if (stop_sound != 0 ||
        (hand->CountDown == 0 &&
         hand->SH.Voices.core[0] == 0 &&
         hand->SH.Voices.core[1] == 0 &&
         hand->SH.first_child == NULL)) {
        snd_StopHandlerPtr(&hand->SH, 1, 0, 0);
        hand->SH.Sound = NULL;
        hand = NULL;
    }

    if (hand == NULL) {
        return 0;
    }

    snd_ActivateHandler(&hand->SH);
    return hand->SH.OwnerID;
}

/* 000096f4 000097bc */ UInt32 snd_PlaySFX_EX(/* 0x0(sp) */ SFXBlock2Ptr block, /* 0x4(sp) */ SInt32 sound, /* 0x8(sp) */ SInt32 vol, /* 0xc(sp) */ SInt32 pan, /* 0x10(sp) */ int pitch_mod, /* 0x14(sp) */ int bend, /* 0x18(sp) */ int sfx_vol, /* 0x1c(sp) */ int sfx_pan) {
    /* -0x40(sp) */ SInt16 _pitch_mod;
    /* -0x3e(sp) */ SInt16 _bend;
    /* -0x3c(sp) */ SInt16 _sfx_vol;
    /* -0x3a(sp) */ SInt16 _sfx_pan;
    /* -0x38(sp) */ struct SndPlayParams params;

    params.mask = 0x3c;
    params.vol = vol;
    params.pan = pan;
    params.pitch_mod = pitch_mod;
    params.pitch_bend = bend;
    return snd_PlaySFXWithStruct(block, sound, sfx_vol, sfx_pan, &params);
}

/* 000097bc 00009a98 */ UInt32 snd_PlaySFXGrain(/* 0x0(sp) */ SFXBlock2Ptr block, /* 0x4(sp) */ SInt32 sindex, /* 0x8(sp) */ SInt32 gindex) {
    /* -0x10(sp) */ SFX2 *sfx;
    /* -0xc(sp) */ BlockSoundHandlerPtr hand;
    UNIMPLEMENTED();
}

/* 00009a98 00009b20 */ SInt8 snd_GetSFXGlobalReg(/* 0x0(sp) */ SInt32 which) {
    UNIMPLEMENTED();
}
/* 00009b20 00009ba4 */ void snd_SetSFXGlobalReg(/* 0x0(sp) */ SInt32 which, /* -0x10(sp) */ SInt8 val) {

    UNIMPLEMENTED();
}
/* 00009ba4 00009c88 */ SInt8 snd_GetSFXSoundReg(/* 0x0(sp) */ UInt32 handle, /* 0x4(sp) */ SInt32 which) {
    /* -0x10(sp) */ BlockSoundHandlerPtr hand;
    /* -0xc(sp) */ SInt8 val;
    UNIMPLEMENTED();
}

/* 00009c88 00009d58 */ void snd_SetSFXSoundReg(/* 0x0(sp) */ UInt32 handle, /* 0x4(sp) */ SInt32 which, /* -0x10(sp) */ SInt8 val) {
    /* -0xc(sp) */ BlockSoundHandlerPtr hand;
    UNIMPLEMENTED();
}

/* 00009d58 00009e34 */ void snd_SetAllSFXSoundReg(/* 0x0(sp) */ UInt32 handle, /* 0x4(sp) */ SInt8 *vals) {
    /* -0x10(sp) */ BlockSoundHandlerPtr hand;
    /* -0xc(sp) */ SInt32 i;
    UNIMPLEMENTED();
}

/* 00009e34 00009f78 */ SInt32 snd_ProcessBlockSoundTick(/* 0x0(sp) */ BlockSoundHandlerPtr handler) {
    /* -0x10(sp) */ SInt32 stop_sound = 0;
    if (handler->NextGrain == -1) {
        if (handler->SH.first_child == NULL &&
            handler->SH.Voices.core[0] == 0 &&
            handler->SH.Voices.core[1] == 0) {
            stop_sound = 1;
        }
    } else {
        handler->CountDown--;
        while (!stop_sound && handler->NextGrain != -1 && handler->CountDown <= 0) {
            stop_sound = snd_DoGrain(handler);
        }
    }
    return stop_sound;
}

/* 00009f78 0000a140 */ bool snd_DoBlockSoundStop(/* 0x0(sp) */ BlockSoundHandlerPtr handler, /* 0x4(sp) */ SInt32 silence, /* 0x8(sp) */ bool vlimit_stop) {
    /* -0x10(sp) */ SInt32 stop_sound;
    /* -0xc(sp) */ SInt32 stop_index;
    UNIMPLEMENTED();
}

/* 0000a140 0000a23c */ SInt32 snd_GetOnStopGrainIndex(/* 0x0(sp) */ BlockSoundHandlerPtr handler) {
    /* -0x10(sp) */ SInt32 i;
    /* -0xc(sp) */ SFX2Ptr sfx;
    UNIMPLEMENTED();
}

/* 0000a23c 0000a274 */ SInt32 snd_SFX_GRAIN_TYPE_NULL(/* 0x0(sp) */ BlockSoundHandlerPtr handler, /* 0x4(sp) */ SFX2Ptr sfx, /* 0x8(sp) */ SFXGrain2Ptr grain) {
    UNIMPLEMENTED();
}

/* 0000a274 0000ae90 */ SInt32 snd_SFX_GRAIN_TYPE_TONE(/* 0x0(sp) */ BlockSoundHandlerPtr handler, /* 0x4(sp) */ SFX2Ptr sfx, /* 0x8(sp) */ SFXGrain2Ptr grain) {
    /* -0x20(sp) */ SInt32 voice;
    /* -0x1c(sp) */ SInt32 g_pan;
    /* -0x18(sp) */ SInt32 priority;
    /* -0x14(sp) */ TonePtr tone;
    /* -0x10(sp) */ SInt32 core;
    /* -0xc(sp) */ SInt32 c_v;

    if ((handler->SH.flags & 0x20) != 0) {
        return 0;
    }

    tone = handler->block->GrainData + (grain->OpcodeData.Opcode & 0xFFFFFF);
    handler->SH.Current_Vol = ((handler->App_Vol * handler->SH.Original_Vol) >> 10) + handler->LFO_Vol;
    if (handler->SH.Current_Vol > 127) {
        handler->SH.Original_Vol = 127;
    }
    if (handler->SH.Current_Vol < 0) {
        handler->SH.Original_Vol = 0;
    }

    handler->SH.Current_Pan = handler->App_Pan + handler->LFO_Pan;
    while (handler->SH.Current_Pan >= 360) {
        handler->SH.Current_Pan -= 360;
    }
    while (handler->SH.Current_Pan < 0) {
        handler->SH.Current_Pan += 360;
    }

    priority = snd_ScalePriorityForVolume(handler->App_Vol, tone);
    snd_LockVoiceAllocatorEx(true, 26);
    if ((tone->Flags & 8) != 0) {
        voice = snd_AllocateNoiseVoice(sfx->VolGroup, priority);
    } else {
        voice = snd_AllocateVoice(sfx->VolGroup, priority);
    }

    if (voice != -1) {
        core = voice / 24;
        c_v = voice % 24;
        handler->SH.Voices.core[core] |= 1 << c_v;
        gChannelStatus[voice].Owner = &handler->SH;
        gChannelStatus[voice].OwnerProc = snd_SFXOwnerProc;
        gChannelStatus[voice].Tone = tone;

        if (tone->Vol >= 0) {
            gChannelStatus[voice].OwnerData.BlockData.g_vol = tone->Vol;
        } else if (tone->Vol < -4) {
            if (tone->Vol == -5) {
                gChannelStatus[voice].OwnerData.BlockData.g_vol = snd_RandomUInt16() & 0x7F;
            } else {
                gChannelStatus[voice].OwnerData.BlockData.g_vol = gBlockGlobalReg[-tone->Vol - 6];
            }
        } else {
            gChannelStatus[voice].OwnerData.BlockData.g_vol = handler->Registers[-tone->Vol - 1];
        }

        if (gChannelStatus[voice].OwnerData.BlockData.g_vol < 0) {
            gChannelStatus[voice].OwnerData.BlockData.g_vol = 0;
        }

        if (tone->Pan >= 0) {
            gChannelStatus[voice].OwnerData.BlockData.g_pan = tone->Pan;
        } else if (tone->Pan < -4) {
            if (tone->Pan == -5) {
                gChannelStatus[voice].OwnerData.BlockData.g_pan = snd_RandomUInt16() % 360u;
            } else {
                gChannelStatus[voice].OwnerData.BlockData.g_pan = 360 * gBlockGlobalReg[-tone->Pan - 6] / 127;
            }
        } else {
            gChannelStatus[voice].OwnerData.BlockData.g_pan = 360 * handler->Registers[-tone->Pan - 1] / 127;
        }

        while (gChannelStatus[voice].OwnerData.BlockData.g_pan < 0) {
            gChannelStatus[voice].OwnerData.BlockData.g_pan += 360;
        }
        while (gChannelStatus[voice].OwnerData.BlockData.g_pan >= 360) {
            gChannelStatus[voice].OwnerData.BlockData.g_pan -= 360;
        }

        gChannelStatus[voice].StartNote = handler->Current_Note;
        gChannelStatus[voice].StartFine = handler->Current_Fine;
        gChannelStatus[voice].Current_PB = handler->Current_PB;
        gChannelStatus[voice].Current_PM = handler->SH.Current_PM;
        gChannelStatus[voice].Priority = priority;
        gChannelStatus[voice].VolGroup = sfx->VolGroup;

        // why not just sfx->flags??
        if ((((SFX2Ptr)handler->SH.Sound)->Flags & 4) != 0) {
            g_pan = snd_CollapsePan(gChannelStatus[voice].OwnerData.BlockData.g_pan, handler->App_Vol, sfx);
        } else {
            g_pan = gChannelStatus[voice].OwnerData.BlockData.g_pan;
        }
        gChannelStatus[voice].Volume.right = 0;
        gChannelStatus[voice].Volume.left = 0;
        snd_MakeVolumes(127,
                        0,
                        handler->SH.Current_Vol,
                        handler->SH.Current_Pan,
                        gChannelStatus[voice].OwnerData.BlockData.g_vol,
                        g_pan,
                        &gChannelStatus[voice].Volume);
        snd_StartVAGVoice(voice, (tone->Flags >> 3) & 1);
    }

    snd_UnlockVoiceAllocator();

    return 0;
}

/* 0000ae90 0000aec8 */ SInt32 snd_SFX_GRAIN_TYPE_XREF_ID(/* 0x0(sp) */ BlockSoundHandlerPtr handler, /* 0x4(sp) */ SFX2Ptr sfx, /* 0x8(sp) */ SFXGrain2Ptr grain) {
    UNIMPLEMENTED();
}
/* 0000aec8 0000af00 */ SInt32 snd_SFX_GRAIN_TYPE_XREF_NUM(/* 0x0(sp) */ BlockSoundHandlerPtr handler, /* 0x4(sp) */ SFX2Ptr sfx, /* 0x8(sp) */ SFXGrain2Ptr grain) {
    UNIMPLEMENTED();
}
/* 0000af00 0000b18c */ SInt32 snd_SFX_GRAIN_TYPE_LFO_SETTINGS(/* 0x0(sp) */ BlockSoundHandlerPtr handler, /* 0x4(sp) */ SFX2Ptr sfx, /* 0x8(sp) */ SFXGrain2Ptr grain) {
    /* -0x18(sp) */ struct LFOTracker *lfo;
    /* -0x14(sp) */ SInt32 index;
    /* -0x10(sp) */ LFOParams *lfop;
    lfop = handler->block->GrainData + (grain->OpcodeData.Opcode &= 0xFFFFFF);
    index = lfop->which_lfo;
    lfo = &handler->lfo[index];

    lfo->target = lfop->target;
    if (lfo->target == 0) {
        snd_RemoveLFO(lfo);
        lfo->type = 0;
        return 0;
    }

    lfo->type = lfop->shape;
    lfo->target = lfop->target;
    lfo->target_extra = lfop->target_extra;
    lfo->setup_flags = lfop->flags;
    lfo->depth = lfop->depth;
    lfo->orig_depth = lfop->depth;
    lfo->step_size = lfop->step_size;
    lfo->orig_step_size = lfop->step_size;
    lfo->state_hold1 = 0;
    lfo->last_lfo = 0;

    if (lfo->type == 2) {
        lfo->state_hold1 = lfop->duty_cycle;
    }

    lfo->state_hold2 = 0;

    if ((lfo->setup_flags & 2) != 0) {
        lfo->next_step = (snd_RandomUInt16() & 0x7FF) << 16;
    } else {
        lfo->next_step = lfop->start_offset << 16;
    }

    snd_InitLFO(lfo);
    return 0;
}

/* 0000b18c 0000b1c4 */ SInt32 snd_SFX_GRAIN_TYPE_CONTROL_NULL(/* 0x0(sp) */ BlockSoundHandlerPtr handler, /* 0x4(sp) */ SFX2Ptr sfx, /* 0x8(sp) */ SFXGrain2Ptr grain) {
    UNIMPLEMENTED();
}
/* 0000b1c4 0000b1fc */ SInt32 snd_SFX_GRAIN_TYPE_LOOP_START(/* 0x0(sp) */ BlockSoundHandlerPtr handler, /* 0x4(sp) */ SFX2Ptr sfx, /* 0x8(sp) */ SFXGrain2Ptr grain) {
    UNIMPLEMENTED();
}
/* 0000b1fc 0000b328 */ SInt32 snd_SFX_GRAIN_TYPE_LOOP_END(/* 0x0(sp) */ BlockSoundHandlerPtr handler, /* 0x4(sp) */ SFX2Ptr sfx, /* 0x8(sp) */ SFXGrain2Ptr grain) {
    /* -0x10(sp) */ SInt32 found;
    /* -0xc(sp) */ SInt32 i;
    UNIMPLEMENTED();
}

/* 0000b328 0000b45c */ SInt32 snd_SFX_GRAIN_TYPE_LOOP_CONTINUE(/* 0x0(sp) */ BlockSoundHandlerPtr handler, /* 0x4(sp) */ SFX2Ptr sfx, /* 0x8(sp) */ SFXGrain2Ptr grain) {
    /* -0x10(sp) */ SInt32 found;
    /* -0xc(sp) */ SInt32 i;
    UNIMPLEMENTED();
}

/* 0000b45c 0000b494 */ SInt32 snd_SFX_GRAIN_TYPE_STOP(/* 0x0(sp) */ BlockSoundHandlerPtr handler, /* 0x4(sp) */ SFX2Ptr sfx, /* 0x8(sp) */ SFXGrain2Ptr grain) {
    UNIMPLEMENTED();
}
/* 0000b494 0000b558 */ SInt32 snd_GetSoundIndexInBlock(/* 0x0(sp) */ SFXBlock2Ptr block, /* 0x4(sp) */ SFX2 *sfx) {
    /* -0x10(sp) */ int i;
    UNIMPLEMENTED();
}

/* 0000b558 0000b73c */ SInt32 snd_SFX_GRAIN_TYPE_RAND_PLAY(/* 0x0(sp) */ BlockSoundHandlerPtr handler, /* 0x4(sp) */ SFX2Ptr sfx, /* 0x8(sp) */ SFXGrain2Ptr grain) {
    /* -0x10(sp) */ SInt32 work32;
    if (handler->sk_doing_skipping_play) {
        snd_ShowError(10, handler->block->BlockID, snd_GetSoundIndexInBlock(handler->block, sfx), handler->NextGrain + 1, 0);
        return -1;
    }

    work32 = snd_RandomUInt16() % GRAIN_ARG(0);
    if (work32 == GRAIN_ARG(2)) {
        work32 += 1;
        if (work32 == GRAIN_ARG(0)) {
            work32 = 0;
        }
    }

    GRAIN_ARG(2) = work32;
    handler->NextGrain += GRAIN_ARG(1) * work32;
    handler->sk_grains_to_play = GRAIN_ARG(1) + 1;
    handler->sk_grains_to_skip = (GRAIN_ARG(0) - 1 - work32) * GRAIN_ARG(1);
    handler->sk_doing_skipping_play = 1;
    return 0;
}

/* 0000b73c 0000b7b8 */ SInt32 snd_SFX_GRAIN_TYPE_RAND_DELAY(/* 0x0(sp) */ BlockSoundHandlerPtr handler, /* 0x4(sp) */ SFX2Ptr sfx, /* 0x8(sp) */ SFXGrain2Ptr grain) {
    UNIMPLEMENTED();
}
/* 0000b7b8 0000b8c8 */ SInt32 snd_SFX_GRAIN_TYPE_RAND_PB(/* 0x0(sp) */ BlockSoundHandlerPtr handler, /* 0x4(sp) */ SFX2Ptr sfx, /* 0x8(sp) */ SFXGrain2Ptr grain) {
    /* -0x10(sp) */ SInt32 work32;
    work32 = snd_RandomUInt16();
    snd_SetSFXPitchbend(handler->SH.OwnerID, GRAIN_ARG(0) * ((int)(0xFFFF * (work32 % 0x7FFF)) / 0x7FFF - 0x8000) / 100);
    return 0;
}

/* 0000b8c8 0000b9b8 */ SInt32 snd_SFX_GRAIN_TYPE_PB(/* 0x0(sp) */ BlockSoundHandlerPtr handler, /* 0x4(sp) */ SFX2Ptr sfx, /* 0x8(sp) */ SFXGrain2Ptr grain) {
    /* -0x10(sp) */ SInt32 work32;
    UNIMPLEMENTED();
}

/* 0000b9b8 0000ba98 */ SInt32 snd_SFX_GRAIN_TYPE_ADD_PB(/* 0x0(sp) */ BlockSoundHandlerPtr handler, /* 0x4(sp) */ SFX2Ptr sfx, /* 0x8(sp) */ SFXGrain2Ptr grain) {
    /* -0x10(sp) */ SInt32 work32;
    UNIMPLEMENTED();
}

/* 0000ba98 0000bb5c */ SInt32 snd_SFX_GRAIN_TYPE_SET_REGISTER(/* 0x0(sp) */ BlockSoundHandlerPtr handler, /* 0x4(sp) */ SFX2Ptr sfx, /* 0x8(sp) */ SFXGrain2Ptr grain) {
    if (GRAIN_ARG(0) < 0) {
        gBlockGlobalReg[-GRAIN_ARG(0) - 1] = GRAIN_ARG(1);
    } else {
        handler->Registers[GRAIN_ARG(0)] = GRAIN_ARG(1);
    }

    return 0;
}

/* 0000bb5c 0000bc84 */ SInt32 snd_SFX_GRAIN_TYPE_SET_REGISTER_RAND(/* 0x0(sp) */ BlockSoundHandlerPtr handler, /* 0x4(sp) */ SFX2Ptr sfx, /* 0x8(sp) */ SFXGrain2Ptr grain) {
    /* -0x10(sp) */ SInt32 work32;
    work32 = GRAIN_ARG(2) - GRAIN_ARG(1) + 1;
    work32 = snd_RandomUInt16() % work32 + GRAIN_ARG(1);

    if (GRAIN_ARG(0) < 0) {
        gBlockGlobalReg[-GRAIN_ARG(0) - 1] = work32;
    } else {
        handler->Registers[GRAIN_ARG(0)] = work32;
    }

    return 0;
}

/* 0000bc84 0000bdd0 */ SInt32 snd_SFX_GRAIN_TYPE_INC_REGISTER(/* 0x0(sp) */ BlockSoundHandlerPtr handler, /* 0x4(sp) */ SFX2Ptr sfx, /* 0x8(sp) */ SFXGrain2Ptr grain) {
    /* -0x10(sp) */ SInt32 work32;
    UNIMPLEMENTED();
}

/* 0000bdd0 0000bf1c */ SInt32 snd_SFX_GRAIN_TYPE_DEC_REGISTER(/* 0x0(sp) */ BlockSoundHandlerPtr handler, /* 0x4(sp) */ SFX2Ptr sfx, /* 0x8(sp) */ SFXGrain2Ptr grain) {
    /* -0x10(sp) */ SInt32 work32;
    if (GRAIN_ARG(0) < 0) {
        work32 = gBlockGlobalReg[-GRAIN_ARG(0) - 1] - 1;
        if (work32 < -128) {
            work32 = 128;
        }
        gBlockGlobalReg[-GRAIN_ARG(0) - 1] = work32;
    } else {
        work32 = handler->Registers[GRAIN_ARG(0)] - 1;
        if (work32 < -128) {
            work32 = 128;
        }
        handler->Registers[GRAIN_ARG(0)] = work32;
    }

    return 0;
}

/* 0000bf1c 0000c0e0 */ SInt32 snd_SFX_GRAIN_TYPE_TEST_REGISTER(/* 0x0(sp) */ BlockSoundHandlerPtr handler, /* 0x4(sp) */ SFX2Ptr sfx, /* 0x8(sp) */ SFXGrain2Ptr grain) {
    /* -0x10(sp) */ SInt8 source;
    if (GRAIN_ARG(0) < 0) {
        source = gBlockGlobalReg[-GRAIN_ARG(0) - 1];
    } else {
        source = handler->Registers[GRAIN_ARG(0)];
    }

    if (GRAIN_ARG(1) == 0) {
        if (source >= GRAIN_ARG(2)) {
            handler->NextGrain++;
        }
    } else if (GRAIN_ARG(1) == 1) {
        if (source != GRAIN_ARG(2)) {
            handler->NextGrain++;
        }
    } else if (GRAIN_ARG(1) >= 2) {
        if (GRAIN_ARG(2) >= source) {
            handler->NextGrain++;
        }
    }

    return 0;
}

/* 0000c0e0 0000c118 */ SInt32 snd_SFX_GRAIN_TYPE_MARKER(/* 0x0(sp) */ BlockSoundHandlerPtr handler, /* 0x4(sp) */ SFX2Ptr sfx, /* 0x8(sp) */ SFXGrain2Ptr grain) {
    return 0;
}

/* 0000c118 0000c284 */ SInt32 snd_SFX_GRAIN_TYPE_GOTO_MARKER(/* 0x0(sp) */ BlockSoundHandlerPtr handler, /* 0x4(sp) */ SFX2Ptr sfx, /* 0x8(sp) */ SFXGrain2Ptr grain) {
    /* -0x18(sp) */ SInt32 work32 = GRAIN_ARG(0);
    /* -0x14(sp) */ SInt32 i;
    /* -0x10(sp) */ SInt32 found = 0;
    for (i = 0; i < sfx->NumGrains && !found; i++) {
        if (sfx->FirstGrain[i].OpcodeData.MicroOp.Type == 0x23 &&
            sfx->FirstGrain[i].OpcodeData.MicroOp.Arg[0] == work32) {
            handler->NextGrain = i - 1;
            found = 1;
        }
    }

    if (!found) {
        snd_ShowError(11, 0, 0, 0, 0);
        return -1;
    }

    return 0;
}

/* 0000c284 0000c448 */ SInt32 snd_SFX_GRAIN_TYPE_GOTO_RANDOM_MARKER(/* 0x0(sp) */ BlockSoundHandlerPtr handler, /* 0x4(sp) */ SFX2Ptr sfx, /* 0x8(sp) */ SFXGrain2Ptr grain) {
    /* -0x18(sp) */ SInt32 work32;
    /* -0x14(sp) */ SInt32 i;
    /* -0x10(sp) */ SInt32 found;
    UNIMPLEMENTED();
}

/* 0000c448 0000c4e4 */ SInt32 snd_SFX_GRAIN_TYPE_WAIT_FOR_ALL_VOICES(/* 0x0(sp) */ BlockSoundHandlerPtr handler, /* 0x4(sp) */ SFX2Ptr sfx, /* 0x8(sp) */ SFXGrain2Ptr grain) {
    UNIMPLEMENTED();
}

/* 0000c4e4 0000c664 */ SInt32 snd_SFX_GRAIN_TYPE_PLAY_CYCLE(/* 0x0(sp) */ BlockSoundHandlerPtr handler, /* 0x4(sp) */ SFX2Ptr sfx, /* 0x8(sp) */ SFXGrain2Ptr grain) {
    /* -0x10(sp) */ SInt32 work32;
    if (handler->sk_doing_skipping_play) {
        snd_ShowError(13, 0, 0, 0, 0);
        return -1;
    }
    work32 = GRAIN_ARG(2);
    GRAIN_ARG(2) += 1;
    if (GRAIN_ARG(2) == GRAIN_ARG(1)) {
        GRAIN_ARG(2) = 0;
    }
    handler->NextGrain += GRAIN_ARG(1) * work32;
    handler->sk_grains_to_play = GRAIN_ARG(1) + 1;
    handler->sk_grains_to_skip = (GRAIN_ARG(0) - 1 - work32) * GRAIN_ARG(1);
    handler->sk_doing_skipping_play = 1;
    return 0;
}

/* 0000c664 0000c800 */ SInt32 snd_SFX_GRAIN_TYPE_ADD_REGISTER(/* 0x0(sp) */ BlockSoundHandlerPtr handler, /* 0x4(sp) */ SFX2Ptr sfx, /* 0x8(sp) */ SFXGrain2Ptr grain) {
    /* -0x10(sp) */ SInt32 work32;
    UNIMPLEMENTED();
}

/* 0000c800 0000c870 */ SInt32 snd_SFX_GRAIN_TYPE_KEY_OFF_VOICES(/* 0x0(sp) */ BlockSoundHandlerPtr handler, /* 0x4(sp) */ SFX2Ptr sfx, /* 0x8(sp) */ SFXGrain2Ptr grain) {
    UNIMPLEMENTED();
}
/* 0000c870 0000c8e0 */ SInt32 snd_SFX_GRAIN_TYPE_KILL_VOICES(/* 0x0(sp) */ BlockSoundHandlerPtr handler, /* 0x4(sp) */ SFX2Ptr sfx, /* 0x8(sp) */ SFXGrain2Ptr grain) {
    UNIMPLEMENTED();
}
/* 0000c8e0 0000c934 */ SInt32 snd_SFX_GRAIN_TYPE_ON_STOP_MARKER(/* 0x0(sp) */ BlockSoundHandlerPtr handler, /* 0x4(sp) */ SFX2Ptr sfx, /* 0x8(sp) */ SFXGrain2Ptr grain) {
    UNIMPLEMENTED();
}
/* 0000c934 0000ca64 */ SInt32 snd_SFX_GRAIN_TYPE_COPY_REGISTER(/* 0x0(sp) */ BlockSoundHandlerPtr handler, /* 0x4(sp) */ SFX2Ptr sfx, /* 0x8(sp) */ SFXGrain2Ptr grain) {
    /* -0x10(sp) */ SInt8 work;
    UNIMPLEMENTED();
}

/* 0000ca64 0000cb1c */ SInt32 snd_SFX_GRAIN_TYPE_PLUGIN_MESSAGE(/* 0x0(sp) */ BlockSoundHandlerPtr handler, /* 0x4(sp) */ SFX2Ptr sfx, /* 0x8(sp) */ SFXGrain2Ptr grain) {
    /* -0x10(sp) */ PluginParams *pp;
    UNIMPLEMENTED();
}

/* 0000cb1c 0000cec8 */ SInt32 snd_SFX_GRAIN_TYPE_BRANCH(/* 0x0(sp) */ BlockSoundHandlerPtr handler, /* 0x4(sp) */ SFX2Ptr sfx, /* 0x8(sp) */ SFXGrain2Ptr grain) {
    /* -0x20(sp) */ SInt32 index;
    /* -0x1c(sp) */ SFXBlock2Ptr block;
    /* -0x18(sp) */ SFX2 *new_sfx;
    /* -0x14(sp) */ BlockSoundHandlerPtr weakest;
    /* -0x10(sp) */ PlaySoundParams *psp;
    UNIMPLEMENTED();
}

/* 0000cec8 0000d548 */ SInt32 snd_SFX_GRAIN_TYPE_STARTCHILDSOUND(/* 0x0(sp) */ BlockSoundHandlerPtr handler, /* 0x4(sp) */ SFX2Ptr sfx, /* 0x8(sp) */ SFXGrain2Ptr grain) {
    /* -0x58(sp) */ UInt32 sid;
    /* -0x54(sp) */ SInt32 reg;
    /* -0x50(sp) */ SInt32 vol;
    /* -0x4c(sp) */ SInt32 pan;
    /* -0x48(sp) */ SInt32 index;
    /* -0x44(sp) */ SFXBlock2Ptr block;
    /* -0x40(sp) */ struct SndPlayParams params;
    /* -0x10(sp) */ PlaySoundParams *psp;
    UNIMPLEMENTED();
}

/* 0000d548 0000d740 */ SInt32 snd_SFX_GRAIN_TYPE_STOPCHILDSOUND(/* 0x0(sp) */ BlockSoundHandlerPtr handler, /* 0x4(sp) */ SFX2Ptr sfx, /* 0x8(sp) */ SFXGrain2Ptr grain) {
    /* -0x18(sp) */ SFX2Ptr sts;
    /* -0x14(sp) */ SInt32 index;
    /* -0x10(sp) */ SFXBlock2Ptr block;
    /* -0xc(sp) */ PlaySoundParams *psp;
    UNIMPLEMENTED();
}

/* 0000d740 0000d79c */ SInt32 snd_SFX_UNKNOWN_GRAIN_TYPE(/* 0x0(sp) */ BlockSoundHandlerPtr handler, /* 0x4(sp) */ SFX2Ptr sfx, /* 0x8(sp) */ SFXGrain2Ptr grain) {
    UNIMPLEMENTED();
}

/* 0000d79c 0000da2c */ SInt32 snd_DoGrain(/* 0x0(sp) */ BlockSoundHandlerPtr handler) {
    /* -0x18(sp) */ SFX2Ptr sfx;
    /* -0x14(sp) */ SFXGrain2Ptr grain;
    /* -0x10(sp) */ SInt32 stop_sound = 0;
    /* -0xc(sp) */ SInt32 ret;
    sfx = (SFX2Ptr)handler->SH.Sound;
    grain = &sfx->FirstGrain[handler->NextGrain];
    ret = gGrainHandler[grain->OpcodeData.MicroOp.Type](handler, sfx, grain);
    if (ret < 0) {
        return 1;
    }

    if (handler->sk_doing_skipping_play) {
        handler->sk_grains_to_play--;
        if (handler->sk_grains_to_play == 0) {
            handler->NextGrain += handler->sk_grains_to_skip;
            handler->sk_doing_skipping_play = 0;
        }
    }

    handler->NextGrain++;
    if (handler->NextGrain >= sfx->NumGrains) {
        if (handler->SH.Voices.core[0] ||
            handler->SH.Voices.core[1] ||
            (handler->SH.first_child != NULL && handler->SH.parent == NULL)) {
            handler->NextGrain = -1;
        } else {
            stop_sound = 1;
        }
    } else {
        handler->CountDown = grain->Delay + ret;
    }

    return stop_sound;
}

/* 0000da2c 0000e18c */ void snd_SetSFXVolPan(/* 0x0(sp) */ UInt32 handle, /* 0x4(sp) */ SInt32 vol, /* 0x8(sp) */ SInt32 pan, /* 0xc(sp) */ SInt32 cause) {
    /* -0x38(sp) */ BlockSoundHandlerPtr hand;
    /* -0x34(sp) */ SInt32 uses_voice = -1;
    /* -0x30(sp) */ SInt32 own_the_allocator;
    /* -0x2c(sp) */ SFX2Ptr sfx;
    /* -0x28(sp) */ SInt32 new_vol;
    /* -0x24(sp) */ SInt32 new_pan;
    /* -0x20(sp) */ GSoundHandlerPtr child_walk;
    /* -0x1c(sp) */ SInt32 child_vol;
    /* -0x18(sp) */ SInt32 intr_state;
    /* -0x14(sp) */ SInt32 dis;
    /* -0x10(sp) */ SpuVolume spu_vol;
    /* -0xc(sp) */ SInt32 g_pan;

    hand = snd_CheckHandlerStillActive(handle);
    if (hand == NULL) {
        return;
    }

    sfx = hand->SH.Sound;
    if (vol >= 0) {
        if (vol != 0x7FFFFFFF) {
            hand->App_Vol = vol;
        }
    } else {
        hand->App_Vol = -1024 * vol / 127;
    }

    if (pan == -1) {
        hand->App_Pan = sfx->Pan;
    } else if (pan != -2) {
        hand->App_Pan = pan;
    }

    new_vol = ((hand->App_Vol * hand->SH.Original_Vol) >> 10) + hand->LFO_Vol;
    if (new_vol > 127) {
        new_vol = 127;
    }
    if (new_vol < 0) {
        new_vol = 0;
    }

    new_pan = hand->App_Pan + hand->LFO_Pan;
    while (new_pan >= 360) {
        new_pan -= 360;
    }
    while (new_pan < 0) {
        new_pan += 360;
    }

    if (new_vol == hand->SH.Current_Vol && new_pan == hand->SH.Current_Pan) {
        return;
    }

    if (cause == 0 && hand->SH.first_child != NULL) {
        for (child_walk = hand->SH.first_child; child_walk != NULL; child_walk = child_walk->siblings) {
            snd_SetSFXVolPan(child_walk->OwnerID, hand->App_Vol * hand->SH.Original_Vol / 127, pan, 0);
        }
    }

    hand->SH.Current_Vol = new_vol;
    hand->SH.Current_Pan = new_pan;
    own_the_allocator = snd_LockVoiceAllocatorEx(true, 27);

    dis = CpuSuspendIntr(&intr_state);

    while (true) {
        uses_voice = snd_GetNextHandlerVoice(&hand->SH, uses_voice + 1);
        if (uses_voice == -1) {
            break;
        }

        g_pan = 0;
        // why not just sfx->flags??
        if ((((SFX2Ptr)hand->SH.Sound)->Flags & 4) != 0) {
            g_pan = snd_CollapsePan(gChannelStatus[uses_voice].OwnerData.BlockData.g_pan, hand->App_Vol, sfx);
        } else {
            g_pan = gChannelStatus[uses_voice].OwnerData.BlockData.g_pan;
        }

        snd_MakeVolumes(127,
                        0,
                        hand->SH.Current_Vol,
                        hand->SH.Current_Pan,
                        gChannelStatus[uses_voice].OwnerData.BlockData.g_vol,
                        g_pan,
                        &gChannelStatus[uses_voice].Volume);

        if ((hand->SH.flags & HND_PAUSED) == 0) {
            spu_vol.left = snd_AdjustVolToGroup(gChannelStatus[uses_voice].Volume.left, hand->SH.VolGroup) >> 1;
            spu_vol.right = snd_AdjustVolToGroup(gChannelStatus[uses_voice].Volume.right, hand->SH.VolGroup) >> 1;

            sceSdSetParam(SD_VPARAM_VOLL | SD_VOICE(uses_voice / 24, uses_voice % 24), spu_vol.left);
            sceSdSetParam(SD_VPARAM_VOLR | SD_VOICE(uses_voice / 24, uses_voice % 24), spu_vol.right);
        }

        gChannelStatus[uses_voice].Priority = snd_ScalePriorityForVolume(hand->App_Vol, gChannelStatus[uses_voice].Tone);
    }

    if (!dis) {
        CpuResumeIntr(intr_state);
    }

    if (own_the_allocator) {
        snd_UnlockVoiceAllocator();
    }
}

/* 0000e18c 0000e240 */ void snd_SetSFXPitch(/* 0x0(sp) */ UInt32 handle, /* 0x4(sp) */ SInt32 pitch) {
    /* -0x10(sp) */ BlockSoundHandlerPtr hand;
    UNIMPLEMENTED();
}

/* 0000e240 0000e32c */ void snd_SetSFXPitchbend(/* 0x0(sp) */ UInt32 handle, /* -0x18(sp) */ SInt16 bend) {
    /* -0x14(sp) */ BlockSoundHandlerPtr hand;
    /* -0x10(sp) */ GSoundHandlerPtr child_walk;
    hand = snd_CheckHandlerStillActive(handle);
    if (hand == NULL) {
        return;
    }

    if (hand->SH.first_child != NULL) {
        for (child_walk = hand->SH.first_child; child_walk != NULL; child_walk = child_walk->siblings) {
            snd_SetSFXPitchbend(child_walk->OwnerID, bend);
        }
    }

    hand->App_PB = bend;
    snd_UpdateSFXPitch(hand);
}

/* 0000e32c 0000e418 */ void snd_SetSFXPitchModifier(/* 0x0(sp) */ UInt32 handle, /* -0x18(sp) */ SInt16 mod) {
    /* -0x14(sp) */ BlockSoundHandlerPtr hand;
    /* -0x10(sp) */ GSoundHandlerPtr child_walk;
    UNIMPLEMENTED();
}

/* 0000e418 0000e8d8 */ void snd_UpdateSFXPitch(/* 0x0(sp) */ BlockSoundHandlerPtr hand) {
    /* -0x28(sp) */ SInt32 own_the_allocator;
    /* -0x24(sp) */ SInt32 uses_voice = -1;
    /* -0x20(sp) */ SInt32 note;
    /* -0x1c(sp) */ SInt32 fine;
    /* -0x18(sp) */ SInt32 new_pb;
    /* -0x14(sp) */ SInt32 intr_state;
    /* -0x10(sp) */ SInt32 dis;
    /* -0xc(sp) */ SInt32 clock;
    hand->SH.Current_PM = hand->App_PM + hand->LFO_PM;
    new_pb = hand->App_PB + hand->LFO_PB;

    if (new_pb <= 32767) {
        if (new_pb >= -32768) {
            hand->Current_PB = new_pb;
        } else {
            hand->Current_PB = -32768;
        }
    } else {
        hand->Current_PB = 32767;
    }

    own_the_allocator = snd_LockVoiceAllocatorEx(true, 28);
    while (true) {
        uses_voice = snd_GetNextHandlerVoice(&hand->SH, uses_voice + 1);
        if (uses_voice == -1) {
            break;
        }

        gChannelStatus[uses_voice].Current_PB = hand->Current_PB;
        gChannelStatus[uses_voice].Current_PM = hand->SH.Current_PM;

        if ((hand->SH.flags & HND_PAUSED) != 0) {
            continue;
        }

        dis = CpuSuspendIntr(&intr_state);

        if ((gChannelStatus[uses_voice].Tone->Flags & 8) == 0) {
            snd_PitchBendTone(gChannelStatus[uses_voice].Tone,
                              hand->Current_PB,
                              hand->SH.Current_PM,
                              hand->Current_Note,
                              hand->Current_Fine,
                              &note, &fine);

            sceSdSetParam(SD_VPARAM_PITCH | SD_VOICE(uses_voice / 24, uses_voice % 24),
                          PS1Note2Pitch(gChannelStatus[uses_voice].Tone->CenterNote,
                                        gChannelStatus[uses_voice].Tone->CenterFine,
                                        note,
                                        fine));
        } else {
            clock = gChannelStatus[uses_voice].Tone->CenterNote + 32 * gChannelStatus[uses_voice].Current_PB / 0x7fff;
            if (clock < 0) {
                clock = 0;
            }
            if (clock > 63) {
                clock = 63;
            }
            sceSdSetCoreAttr(SD_CORE_NOISE_CLK | uses_voice / 24, clock);
        }

        if (!dis) {
            CpuResumeIntr(intr_state);
        }
    }

    if (own_the_allocator) {
        snd_UnlockVoiceAllocator();
    }
}

/* 0000e8d8 0000e944 */ SInt32 snd_DoesSFXLoop(/* 0x0(sp) */ SFXBlock2Ptr block, /* 0x4(sp) */ SInt32 sound) {
    UNIMPLEMENTED();
}
/* 0000e944 0000eb8c */ void snd_SFXOwnerProc(/* 0x0(sp) */ SInt32 voice, /* 0x4(sp) */ UInt32 owner, /* 0x8(sp) */ SInt32 flag) {
    /* -0x18(sp) */ BlockSoundHandlerPtr snd = (BlockSoundHandlerPtr)owner;
    /* -0x14(sp) */ SInt32 core = voice / 24;
    /* -0x10(sp) */ SInt32 c_v = voice % 24;
    /* -0xc(sp) */ BlockSoundHandlerPtr parent;
    snd->SH.Voices.core[core] &= ~(1 << c_v);
    if (snd->SH.Voices.core[0] == 0 &&
        snd->SH.Voices.core[1] == 0 &&
        snd->NextGrain == -1 &&
        (snd->SH.parent != NULL || snd->SH.first_child == NULL)) {
        snd_LockMasterTick(16);
        parent = snd->SH.parent;
        snd_RemoveLFOsForHandler(snd);
        snd_DeactivateHandler(&snd->SH, 0);

        if (parent != NULL &&
            parent->SH.first_child == NULL &&
            snd->SH.Voices.core[0] == 0 &&
            snd->SH.Voices.core[1] == 0 &&
            snd->NextGrain == -1) {
            snd_RemoveLFOsForHandler(parent);
            snd_DeactivateHandler(&parent->SH, 0);
        }

        snd_UnlockMasterTick();
    }
}

/* 0000eb8c 0000ed00 */ SInt32 snd_CollapsePan(/* 0x0(sp) */ SInt32 g_pan, /* 0x4(sp) */ SInt32 app_vol, /* 0x8(sp) */ SFX2 *sfx) {
    /* -0x18(sp) */ SInt32 w_vol = 0x400;
    /* -0x14(sp) */ SInt32 m_vol;
    /* -0x10(sp) */ SInt32 f_vol;

    // TODO figure this out better using the stack vars
    if (app_vol < 854) {
        if (app_vol <= 170) {
            return 0;
        } else if (g_pan < 181) {
            return (app_vol - 170) * g_pan / 684;
        } else {
            return 360 - (360 - g_pan) * (app_vol - 170) / 684;
        }
    }

    return g_pan;
}

/* 0000ed00 0000eed0 */ SInt8 snd_ScalePriorityForVolume(/* 0x0(sp) */ SInt32 vol, /* 0x4(sp) */ TonePtr tone) {
    /* -0x18(sp) */ SInt32 p = tone->Priority;
    /* -0x14(sp) */ SInt32 depth = (tone->Flags >> 1) & 3;
    /* -0x10(sp) */ SInt32 var;
    if (depth == 0) {
        return p;
    }

    if (depth == 1) {
        var = p / 5;
    } else if (depth == 2) {
        var = p / 2;
    } else {
        var = tone->Priority;
    }

    if (var == 0) {
        return p;
    }

    if (vol < 951) {
        p -= var - var * vol / 950;
    }

    return p;
}

/* 0000eed0 0000f0b0 */ void snd_ResetAllHandlersForSound(/* 0x0(sp) */ SFX2 *sfx) {
    /* -0x18(sp) */ GSoundHandlerPtr walk;
    /* -0x14(sp) */ BlockSoundHandlerPtr hand;
    /* -0x10(sp) */ int x;
    UNIMPLEMENTED();
}
