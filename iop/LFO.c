#include "LFO.h"
#include "blocksnd.h"
#include "my_rand.h"
#include "stick.h"
#include "types.h"
#include "util.h"

#include "lfo_sine.inc"
/* data 11e8 */ struct LFOTracker *gActiveLFOs = NULL;
/* data 11ec */ SInt32 gNumLFOsInUse = 0;

/* 00003e5c 00003e90 */ SInt32 snd_LFO_TYPE_OFF(/* 0x0(sp) */ struct LFOTracker *tracker, /* 0x4(sp) */ int step) {
    return 0;
}
/* 00003e90 00003ee8 */ SInt32 snd_LFO_TYPE_SINE(/* 0x0(sp) */ struct LFOTracker *tracker, /* 0x4(sp) */ int step) {
    return gLFO_sine[step];
}
/* 00003ee8 00003f44 */ SInt32 snd_LFO_TYPE_SQUARE(/* 0x0(sp) */ struct LFOTracker *tracker, /* 0x4(sp) */ int step) {
    if (step >= tracker->state_hold1) {
        return -32767;
    }

    return 32767;
}
/* 00003f44 00004058 */ SInt32 snd_LFO_TYPE_TRIANGLE(/* 0x0(sp) */ struct LFOTracker *tracker, /* 0x4(sp) */ int step) {
    /* -0x10(sp) */ SInt32 ret_val;
    if (step < 0x200) {
        ret_val = 0x7fff * step / 0x200;
    } else if (step >= 0x600) {
        ret_val = 0x7fff * (step - 0x600) / 0x200 - 0x7fff;
    } else {
        ret_val = 0x7fff - 0xfffe * (step - 0x200) / 0x400;
    }

    return ret_val;
}

/* 00004058 00004120 */ SInt32 snd_LFO_TYPE_SAW(/* 0x0(sp) */ struct LFOTracker *tracker, /* 0x4(sp) */ int step) {
    /* -0x10(sp) */ SInt32 ret_val;
    if (step >= 0x400) {
        ret_val = 0x7fff * (step - 0x400) / 0x400 - 0x7fff;
    } else {
        ret_val = 0x7fff * step / 1023;
    }

    return ret_val;
}

/* 00004120 00004254 */ SInt32 snd_LFO_TYPE_RAND(/* 0x0(sp) */ struct LFOTracker *tracker, /* 0x4(sp) */ int step) {
    if (step >= 0x400 && tracker->state_hold2 == 1) {
        tracker->state_hold2 = 0;
        tracker->state_hold1 = ((snd_RandomUInt16() & 0x7fff) - 0x3fff) * 2;
    } else if (step < 0x400 && tracker->state_hold2 == 0) {
        tracker->state_hold2 = 1;
        tracker->state_hold1 = -(snd_RandomUInt16() & 0x7fff) * (snd_RandomUInt16() & 1);
    }

    return tracker->state_hold1;
}

/* data 11f0 */ LFOFunction gLFOFuncs[] = {
    snd_LFO_TYPE_OFF,
    snd_LFO_TYPE_SINE,
    snd_LFO_TYPE_SQUARE,
    snd_LFO_TYPE_TRIANGLE,
    snd_LFO_TYPE_SAW,
    snd_LFO_TYPE_RAND,
};

/* 00004254 00004364 */ SInt32 snd_GetLFO(/* s1 17 */ struct LFOTracker *tracker, /* s0 16 */ SInt32 step_mult) {
    /* -0x18(sp) */ SInt32 ret_val;
    /* -0x14(sp) */ SInt32 step;
    step = tracker->next_step >> 16;
    tracker->next_step += tracker->step_size * step_mult;
    if (tracker->next_step > 0x7ffffff) {
        tracker->next_step -= 0x8000000;
    }

    ret_val = gLFOFuncs[tracker->type](tracker, step);
    if ((tracker->setup_flags & 1) != 0) {
        return -ret_val;
    }
    return ret_val;
}

/* 00004364 0000444c */ void snd_InitLFO(/* 0x0(sp) */ struct LFOTracker *tracker) {
    if (tracker->type == LFO_TYPE_RAND) {
        tracker->state_hold1 = -(snd_RandomUInt16() & 0x7fff) * (snd_RandomUInt16() & 1);
        tracker->state_hold2 = 1;
    }
    snd_CalcLFODepth(tracker);
    snd_LockMasterTick(256);
    if (snd_InsertLFO(tracker)) {
        snd_DoOneLFO(tracker);
    }
    snd_UnlockMasterTick();
}

/* 0000444c 000044e4 */ void snd_RemoveLFOsForHandler(/* 0x0(sp) */ BlockSoundHandlerPtr handler) {
    /* -0x10(sp) */ SInt32 i;
    for (i = 0; i < 4; i++) {
        snd_RemoveLFO(&handler->lfo[i]);
    }
}

/* 000044e4 00004628 */ void snd_RemoveLFO(/* 0x0(sp) */ struct LFOTracker *lfo) {
    /* -0x10(sp) */ struct LFOTracker *walk;
    lfo->running_flags &= ~1;

    if (gActiveLFOs == lfo) {
        gActiveLFOs = lfo->next;
        gNumLFOsInUse--;
    } else {
        walk = gActiveLFOs;
        while (walk != NULL && walk->next != lfo) {
            walk = walk->next;
        }

        if (walk != NULL) {
            walk->next = walk->next->next;
            gNumLFOsInUse--;
        }
    }
}

/* 00004628 00004774 */ bool snd_InsertLFO(/* 0x0(sp) */ struct LFOTracker *lfo) {
    /* -0x10(sp) */ struct LFOTracker *walk;
    if ((lfo->running_flags & 1) != 0) {
        return true;
    }

    if (gNumLFOsInUse == 10) {
        return false;
    }

    gNumLFOsInUse++;
    lfo->next = NULL;
    if (gActiveLFOs == NULL) {
        gActiveLFOs = lfo;
        return true;
    }

    walk = gActiveLFOs;
    while (walk->next != NULL) {
        walk = walk->next;
    }
    walk->next = lfo;
    return true;
}

#define UPDATE_VOL 1
#define UPDATE_PITCH 2

/* 00004774 00004b44 */ UInt32 snd_DoOneLFO(/* 0x0(sp) */ struct LFOTracker *tracker) {
    /* -0x20(sp) */ BlockSoundHandlerPtr handler = tracker->handler;
    /* -0x1c(sp) */ struct LFOTracker *target_tracker;
    /* -0x18(sp) */ SInt32 range;
    /* -0x14(sp) */ SInt32 lfo;
    /* -0x10(sp) */ SInt32 new_val;
    /* -0xc(sp) */ UInt32 ret = 0;

    switch (tracker->target) {
    case LFO_TARGET_VOL:
        range = snd_GetLFO(tracker, 2);
        new_val = (tracker->range * (range - 0x7fff)) >> 16;
        if (handler->LFO_Vol != new_val) {
            handler->LFO_Vol = new_val;
            ret = UPDATE_VOL;
        }
        break;
    case LFO_TARGET_PAN:
        range = snd_GetLFO(tracker, 2);
        new_val = (tracker->range * range) >> 15;
        if (handler->LFO_Pan != new_val) {
            handler->LFO_Pan = new_val;
            ret = UPDATE_VOL;
        }
        break;
    case LFO_TARGET_PM:
        range = snd_GetLFO(tracker, 2);
        new_val = (tracker->range * range) >> 15;
        if (handler->LFO_PM != new_val) {
            handler->LFO_PM = new_val;
            ret = UPDATE_PITCH;
        }
        break;
    case LFO_TARGET_PB:
        range = snd_GetLFO(tracker, 2);
        new_val = (tracker->range * range) >> 15;
        if (handler->LFO_PB != new_val) {
            handler->LFO_PB = new_val;
            ret = UPDATE_PITCH;
        }
        break;
    case LFO_TARGET_STEP:
        target_tracker = &handler->lfo[tracker->target_extra];
        range = snd_GetLFO(tracker, 2);
        // TODO uuuh
        target_tracker->step_size = target_tracker->orig_step_size +
                                    ((((((target_tracker->orig_step_size >> 8) * tracker->depth) >> 10) * range) >> 15) << 8);
        break;
    case LFO_TARGET_DEPTH:
        target_tracker = &handler->lfo[tracker->target_extra];
        range = snd_GetLFO(tracker, 2);
        // TODO uuuh
        target_tracker->depth = (target_tracker->orig_depth +
                                 ((((tracker->depth * target_tracker->orig_depth) >> 10) * range) >> 15));
        break;
    default:
        break;
    }

    return ret;
}

/* 00004b44 00004c44 */ void snd_HandleLFOs() {
    /* -0x10(sp) */ struct LFOTracker *tracker;
    /* -0xc(sp) */ UInt32 flags;

    if ((snd_GetTick() & 1) == 0) {
        for (tracker = gActiveLFOs; tracker != NULL; tracker = tracker->next) {
            flags = snd_DoOneLFO(tracker);
            if ((flags & UPDATE_VOL) != 0) {
                snd_SetSFXVolPan(tracker->handler->SH.OwnerID, 0x7FFFFFFF, -2, 1);
            }
            if ((flags & UPDATE_PITCH) != 0) {
                snd_UpdateSFXPitch(tracker->handler);
            }
        }
    }
}

/* 00004c44 00004dc8 */ void snd_CalcLFODepth(/* 0x0(sp) */ struct LFOTracker *lfo) {
    /* -0x10(sp) */ SFX2 *sfx;

    switch (lfo->target) {
    case LFO_TARGET_VOL:
        sfx = (SFX2Ptr)lfo->handler->SH.Sound;
        lfo->range = (sfx->Vol * lfo->depth) >> 10;
        break;
    case LFO_TARGET_PAN:
        lfo->range = (180 * lfo->depth) >> 10;
        break;
    case LFO_TARGET_PM:
        lfo->range = (0x17d0 * lfo->depth) >> 10;
        break;
    case LFO_TARGET_PB:
        lfo->range = (0x7fff * lfo->depth) >> 10;
        break;
    default:
        break;
    }

    lfo->last_lfo = 0;
}
