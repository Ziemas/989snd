#include "sndhand.h"
#include "989snd.h"
#include "intrman.h"
#include "stdio.h"
#include "stick.h"
#include "stream.h"
#include "types.h"
/* data 1c1c */ GSoundHandlerPtr gActiveSoundListHead = NULL;
/* data 1c20 */ GSoundHandlerPtr gActiveSoundListTail = NULL;
/* bss 9d0 */ struct BlockSoundHandler gBlockSoundHandler[64];
/* bss 52d0 */ struct BasicEffect gBasicEffect[64];
/* bss 68e0 */ struct MIDIHandler gMIDIHandler[32];
/* bss 59d0 */ struct AMEHandler gAMEHandler[4];

/* 0001d778 0001d87c */ void snd_InitHandlers(/* 0x0(sp) */ GSoundHandlerPtr handlers, /* 0x4(sp) */ SInt32 count, /* 0x8(sp) */ UInt32 type, /* 0xc(sp) */ SInt32 size) {
    /* -0x10(sp) */ int x;
    for (x = 0; x < count; x++) {
        handlers->OwnerID = 1;
        handlers->OwnerID |= (x << 16) | (type << 24);
        handlers->Sound = NULL;
        handlers->Effects = NULL;
        handlers->flags = 0;
        handlers->VolGroup = 0;
        handlers->Voices.core[0] = 0;
        handlers->Voices.core[1] = 0;
        handlers = (GSoundHandlerPtr)((UInt8 *)handlers + size);
    }
}

/* 0001d87c 0001d990 */ void snd_InitSoundHandlers() {
    /* -0x10(sp) */ SInt32 x;
    snd_InitHandlers(&gBlockSoundHandler[0].SH, NUM_BLOCK_HANDLER, SOUND_BLOCK, sizeof(struct BlockSoundHandler));
    snd_InitHandlers(&gMIDIHandler[0].SH, NUM_MIDI_HANDLER, SOUND_MIDI, sizeof(struct MIDIHandler));
    snd_InitHandlers(&gAMEHandler[0].SH, NUM_AME_HANDLER, SOUND_AME, sizeof(struct AMEHandler));
    for (x = 0; x < 64; x++) {
        gBasicEffect[x].ec.Flags = 0;
        gBasicEffect[x].ec.next = NULL;
    }
    gActiveSoundListTail = NULL;
    gActiveSoundListHead = NULL;
}

/* 0001d990 0001db34 */ GSoundHandlerPtr snd_FindFreeHandler(/* 0x0(sp) */ GSoundHandlerPtr handlers, /* 0x4(sp) */ SInt32 count, /* 0x8(sp) */ UInt32 type, /* 0xc(sp) */ SInt32 size) {
    /* -0x18(sp) */ SInt32 x;
    /* -0x14(sp) */ SInt32 owner_num;
    /* -0x10(sp) */ SInt32 intr_state;
    /* -0xc(sp) */ SInt32 dis;
    dis = CpuSuspendIntr(&intr_state);
    for (x = 0; x < count; x++) {
        if (handlers->Sound == NULL) {
            handlers->OwnerID = (handlers->OwnerID + 1) | (x << 16) | (type << 24);
            handlers->next = NULL;
            handlers->prev = NULL;
            handlers->parent = NULL;
            handlers->first_child = NULL;
            handlers->siblings = NULL;
            handlers->flags = 0;
            if (!dis) {
                CpuResumeIntr(intr_state);
            }
            return handlers;
        }

        handlers = (GSoundHandlerPtr)((UInt8 *)handlers + size);
    }

    if (!dis) {
        CpuResumeIntr(intr_state);
    }

    return NULL;
}

// TODO this really needs cleaning
/* 0001db34 0001de08 */ bool snd_CheckInstanceLimit(/* 0x0(sp) */ SFX2 *sfx, /* 0x4(sp) */ SInt32 vol, /* 0x8(sp) */ bool parent, /* 0xc(sp) */ BlockSoundHandlerPtr *weakest_holder) {
    /* -0x18(sp) */ SInt32 inst = 0;
    /* -0x14(sp) */ SInt32 type = 0;
    /* -0x10(sp) */ BlockSoundHandlerPtr weakest = NULL;
    /* -0xc(sp) */ BlockSoundHandlerPtr walk = gActiveSoundListHead;
    if (!sfx->InstanceLimit) {
        return false;
    }

    if (weakest_holder != NULL) {
        *weakest_holder = NULL;
    }

    while (walk != NULL) {
        if (SND_GET_TYPE(walk->SH.OwnerID) == SOUND_BLOCK &&
            ((parent && walk->orig_sound == sfx) || (SFX2Ptr)walk->SH.Sound == sfx)) {
            inst++;
            if (weakest == NULL ||
                ((sfx->Flags & SFX_INSTLIMIT_VOL) != 0 && walk->App_Vol < weakest->App_Vol) ||
                ((sfx->Flags & SFX_INSTLIMIT_TICK) != 0 && walk->start_tick < weakest->start_tick)) {
                weakest = walk;
            }
        }
        walk = (BlockSoundHandlerPtr)walk->SH.next;
    }

    if (inst >= sfx->InstanceLimit) {
        if (weakest == NULL ||
            (((sfx->Flags & SFX_INSTLIMIT_VOL) == 0 || weakest->App_Vol >= vol) &&
             (sfx->Flags & SFX_INSTLIMIT_TICK) == 0)) {
            return 0;
        }

        *weakest_holder = weakest;
    }

    return true;
}

/* 0001de08 0001deec */ BlockSoundHandlerPtr snd_GetFreeBlockSoundHandler(/* 0x0(sp) */ SFX2 *sfx, /* 0x4(sp) */ SInt32 vol, /* 0x8(sp) */ bool parent) {
    /* -0x10(sp) */ BlockSoundHandlerPtr weakest;
    if (sfx != NULL && (sfx->Flags & SFX_INSTLIMIT) != 0) {
        if (!snd_CheckInstanceLimit(sfx, vol, parent, &weakest)) {
            return NULL;
        }

        if (weakest != NULL) {
            snd_StopHandlerPtr(&weakest->SH, 1, 0, 1);
        }
    }
    return (BlockSoundHandlerPtr)snd_FindFreeHandler(&gBlockSoundHandler[0].SH, NUM_BLOCK_HANDLER, SOUND_BLOCK, sizeof(struct BlockSoundHandler));
}

/* 0001deec 0001df40 */ MIDIHandlerPtr snd_GetFreeMIDIHandler() {

    return (MIDIHandlerPtr)snd_FindFreeHandler(&gMIDIHandler[0].SH, NUM_MIDI_HANDLER, SOUND_MIDI, sizeof(struct MIDIHandler));
}

/* 0001df40 0001df94 */ AMEHandlerPtr snd_GetFreeAMEHandler() {
    return (AMEHandlerPtr)snd_FindFreeHandler(&gAMEHandler[0].SH, NUM_AME_HANDLER, SOUND_AME, sizeof(struct AMEHandler));
}

/* 0001df94 0001e09c */ void snd_ActivateHandler(/* 0x0(sp) */ GSoundHandlerPtr snd) {
    /* -0x10(sp) */ SInt32 intr_state;
    /* -0xc(sp) */ SInt32 dis;
    dis = CpuSuspendIntr(&intr_state);
    if (gActiveSoundListTail != NULL) {
        if (gActiveSoundListTail != snd) {
            gActiveSoundListTail->next = snd;
            snd->prev = gActiveSoundListTail;
            gActiveSoundListTail = snd;
        }
    } else {
        gActiveSoundListHead = snd;
        gActiveSoundListTail = snd;
    }

    if (!dis) {
        CpuResumeIntr(intr_state);
    }
}

/* 0001e09c 0001e574 */ void snd_DeactivateHandler(/* 0x0(sp) */ GSoundHandlerPtr snd, /* 0x4(sp) */ SInt32 and_child) {
    /* -0x18(sp) */ GSoundHandlerPtr walk;
    /* -0x14(sp) */ SInt32 intr_state;
    /* -0x10(sp) */ SInt32 dis;
    if (!and_child && snd->parent == NULL && snd->first_child != NULL) {
        snd_ShowError(120, 0, 0, 0, 0);
        and_child = 1;
    }

    if (and_child) {
        while (snd->first_child) {
            snd_DeactivateHandler(snd->first_child, and_child);
        }
    }

    dis = CpuSuspendIntr(&intr_state);
    if (snd->Effects != NULL) {
        snd_FreeEffectChain(snd->Effects);
        snd->Effects = NULL;
    }

    if (snd->prev != NULL) {
        if (snd->prev->next == snd) {
            snd->prev->next = snd->next;
        } else {
            printf("list problem 1!\n");
        }
    } else {
        gActiveSoundListHead = snd->next;
    }

    if (snd->next != NULL) {
        if (snd->next->prev == snd) {
            snd->next->prev = snd->prev;
        } else {
            printf("list problem 2!\n");
        }
    } else {
        gActiveSoundListTail = snd->prev;
    }

    if (snd->parent == NULL) {
        snd->Sound = NULL;
        snd->OwnerID &= ~0x80000000;

        if (!dis) {
            CpuResumeIntr(intr_state);
        }

        return;
    }

    // TODO
    // TODO
    // TODO
    // FIXME
    // FIXME
    // FIXME
    // FIXME

    if (!dis) {
        CpuResumeIntr(intr_state);
    }
}

/* 0001e574 0001e6e8 */ void snd_AttachSoundToHandlersChildList(/* 0x0(sp) */ GSoundHandlerPtr handler, /* 0x4(sp) */ UInt32 id) {
    /* -0x10(sp) */ GSoundHandlerPtr child_handle;
    /* -0xc(sp) */ GSoundHandlerPtr walk;
    snd_LockMasterTick(74);
    child_handle = snd_CheckHandlerStillActive(id);
    if (child_handle != NULL) {
        if (handler == child_handle || child_handle->parent != NULL) {
            snd_ShowError(117, 0, 0, 0, 0);
            snd_UnlockMasterTick();
            return;
        }

        child_handle->parent = handler;
        if (handler->first_child != NULL) {
            walk = handler->first_child;
            while (walk->siblings != NULL) {
                walk = walk->siblings;
            }
            walk->siblings = child_handle;
        } else {
            handler->first_child = child_handle;
        }
    }

    snd_UnlockMasterTick();
}

/* 0001e6e8 0001eaec */ GSoundHandlerPtr snd_CheckHandlerStillActive(/* 0x0(sp) */ UInt32 handle) {
    /* -0x10(sp) */ SInt32 handler_index = SND_GET_INDEX(handle);
    /* -0xc(sp) */ SInt32 handler_type = SND_GET_TYPE(handle);

    if (!handle) {
        return 0;
    }

    if (handle == -1) {
        snd_ShowError(100, 0, 0, 0, 0);
        return NULL;
    }

    switch (handler_type) {
    case SOUND_MIDI:
        if (handler_index >= NUM_MIDI_HANDLER) {
            snd_ShowError(101, handle, 0, 0, 0);
            return NULL;
        }
        if (gMIDIHandler[handler_index].SH.OwnerID == handle) {
            return (GSoundHandlerPtr)&gMIDIHandler[handler_index];
        }
        break;
    case SOUND_AME:
        if (handler_index >= NUM_AME_HANDLER) {
            snd_ShowError(101, handle, 0, 0, 0);
            return NULL;
        }
        if (gAMEHandler[handler_index].SH.OwnerID == handle) {
            return (GSoundHandlerPtr)&gAMEHandler[handler_index];
        }
        break;
    case SOUND_VAG:
        if (handler_index >= gNumVAGStreams) {
            snd_ShowError(101, handle, 0, 0, 0);
            return NULL;
        }
        if (gVAGStreamHandler[handler_index].SH.OwnerID == handle) {
            return (GSoundHandlerPtr)&gVAGStreamHandler[handler_index];
        }
        break;
    case SOUND_BLOCK:
        if (handler_index >= NUM_BLOCK_HANDLER) {
            snd_ShowError(101, handle, 0, 0, 0);
            return NULL;
        }
        if (gBlockSoundHandler[handler_index].SH.OwnerID == handle) {
            return (GSoundHandlerPtr)&gBlockSoundHandler[handler_index];
        }
        break;
    }

    return NULL;
}

/* 0001eaec 0001eca0 */ void snd_StopHandler(/* 0x0(sp) */ UInt32 handle, /* 0x4(sp) */ SInt32 and_child, /* 0x8(sp) */ SInt32 silence, /* 0xc(sp) */ bool vlimit_stop) {
    /* -0x18(sp) */ SInt32 handler_index = SND_GET_INDEX(handle);
    /* -0x14(sp) */ SInt32 handler_type = SND_GET_TYPE(handle);
    /* -0x10(sp) */ GSoundHandlerPtr snd;
    switch (handler_type) {
    case SOUND_MIDI:
        snd_StopHandlerPtr(&gMIDIHandler[handler_index].SH, and_child, silence, vlimit_stop);
        break;
    case SOUND_AME:
        snd_StopHandlerPtr(&gAMEHandler[handler_index].SH, and_child, silence, vlimit_stop);
        break;
    case SOUND_VAG:
        snd_StopHandlerPtr(&gVAGStreamHandler[handler_index].SH, and_child, silence, vlimit_stop);
        break;
    case SOUND_BLOCK:
        snd_StopHandlerPtr(&gBlockSoundHandler[handler_index].SH, and_child, silence, vlimit_stop);
        break;
    }
}

/* 0001eca0 0001ed78 */ void snd_PauseHandlerPtr(/* 0x0(sp) */ GSoundHandlerPtr snd, /* 0x4(sp) */ SInt32 and_child) {
    /* -0x10(sp) */ GSoundHandlerPtr walk;
}

/* 0001ed78 0001ee50 */ void snd_ContinueHandlerPtr(/* 0x0(sp) */ GSoundHandlerPtr snd, /* 0x4(sp) */ SInt32 and_child) {
    /* -0x10(sp) */ GSoundHandlerPtr walk;
}

/* 0001ee50 0001ef74 */ void snd_PauseAllSoundsInGroup(/* 0x0(sp) */ UInt32 groups) {
    /* -0x10(sp) */ GSoundHandlerPtr walk;
    /* -0xc(sp) */ SInt32 type;
}

/* 0001ef74 0001f050 */ void snd_StopAllSoundsInGroup(/* 0x0(sp) */ UInt32 groups) {
    /* -0x10(sp) */ GSoundHandlerPtr walk;
}

/* 0001f050 0001f174 */ void snd_ContinueAllSoundsInGroup(/* 0x0(sp) */ UInt32 groups) {
    /* -0x10(sp) */ GSoundHandlerPtr walk;
    /* -0xc(sp) */ SInt32 type;
}

/* 0001f174 0001f2d8 */ void snd_StopAllSounds() {
    /* -0x18(sp) */ GSoundHandlerPtr walk;
    /* -0x14(sp) */ struct VoiceFlags voices;
}

/* 0001f2d8 0001f608 */ void snd_StopHandlerPtr(/* 0x0(sp) */ GSoundHandlerPtr snd, /* 0x4(sp) */ SInt32 and_child, /* 0x8(sp) */ SInt32 silence, /* 0xc(sp) */ bool vlimit_stop) {
    /* -0x18(sp) */ SInt32 do_voice_and_deactivate;
    /* -0x14(sp) */ bool kill_block_sound;
    /* -0x10(sp) */ GSoundHandlerPtr walk;
    /* -0xc(sp) */ MultiMIDIBlockHeaderPtr ame_master;
}

/* 0001f608 0001f6bc */ void snd_StopAllHandlersForSound(/* 0x0(sp) */ SoundPtr snd, /* 0x4(sp) */ SInt32 silence, /* 0x8(sp) */ bool vlimit_stop) {
    /* -0x10(sp) */ GSoundHandlerPtr walk;
}

/* 0001f6bc 0001f7a4 */ bool snd_KillChildrenWithSound(/* 0x0(sp) */ GSoundHandlerPtr handler, /* 0x4(sp) */ void *sfx) {
    /* -0x10(sp) */ GSoundHandlerPtr walk;
}

/* 0001f7a4 0001f8c8 */ SInt32 snd_GetNextHandlerVoice(/* 0x0(sp) */ GSoundHandlerPtr snd, /* 0x4(sp) */ SInt32 start_v) {
    /* -0x18(sp) */ SInt32 x;
    /* -0x14(sp) */ SInt32 core;
    /* -0x10(sp) */ SInt32 c_v;
}

/* 0001f8c8 0001fae0 */ void snd_UpdateHandlers() {
    /* -0x18(sp) */ GSoundHandlerPtr walk;
    /* -0x14(sp) */ SInt32 type;
    /* -0x10(sp) */ SInt32 stop_current_handler;
}

/* 0001fae0 0001fc08 */ SInt32 snd_UpdateEffect(/* 0x0(sp) */ EffectChainPtr effect, /* 0x4(sp) */ GSoundHandlerPtr owner) {}
/* 0001fc08 0001fc6c */ void snd_FreeEffectChain(/* 0x0(sp) */ EffectChainPtr effect) {}
/* 0001fc6c 0001fcd0 */ void snd_RemoveEffect(/* 0x0(sp) */ GSoundHandlerPtr handler, /* 0x4(sp) */ SInt32 type_flag) {
    /* -0x10(sp) */ EffectChainPtr found;
}

/* 0001fcd0 0001fd7c */ EffectChainPtr snd_FindEffect(/* 0x0(sp) */ GSoundHandlerPtr handler, /* 0x4(sp) */ SInt32 type_flag) {
    /* -0x10(sp) */ EffectChainPtr walk;
}

/* 0001fd7c 0001fea0 */ BasicEffectPtr snd_GetFreeBasicEffect() {
    /* -0x10(sp) */ SInt32 x;
}

/* 0001fea0 0001ff58 */ void snd_AddEffectToHandler(/* 0x0(sp) */ GSoundHandlerPtr handler, /* 0x4(sp) */ EffectChainPtr effect) {
    /* -0x10(sp) */ EffectChainPtr walk;
}

/* 0001ff58 000200a8 */ void snd_RemoveEffectFromHandler(/* 0x0(sp) */ GSoundHandlerPtr handler, /* 0x4(sp) */ EffectChainPtr effect) {
    /* -0x18(sp) */ SInt32 intr_state;
    /* -0x14(sp) */ SInt32 res;
    /* -0x10(sp) */ EffectChainPtr walk;
}
