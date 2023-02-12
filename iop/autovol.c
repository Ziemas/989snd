#include "autovol.h"
#include "playsnd.h"
#include "sndhand.h"
#include "stick.h"
#include "types.h"
#include "util.h"

SInt32 snd_AutoVolHandler(BasicEffectPtr effect, GSoundHandlerPtr handler);

/* 00008738 00008c00 */ void snd_AutoVol(/* 0x0(sp) */ UInt32 handle, /* 0x4(sp) */ SInt32 vol, /* 0x8(sp) */ SInt32 delta_time, /* 0xc(sp) */ SInt32 delta_from) {
    /* -0x20(sp) */ SInt32 wouldhavechanged;
    /* -0x1c(sp) */ SInt32 volchange;
    /* -0x18(sp) */ SInt32 stopit = 0;
    /* -0x14(sp) */ GSoundHandlerPtr handler;
    /* -0x10(sp) */ BasicEffectPtr effect;
    /* -0xc(sp) */ SInt32 new_effect = 0;
    snd_LockMasterTick(202);
    handler = snd_CheckHandlerStillActive(handle);
    if (handler == NULL) {
        snd_UnlockMasterTick();
        return;
    }

    if (vol == -4) {
        vol = 0;
        stopit = 1;
    } else {
        vol = (handler->Original_Vol * vol) >> 10;
        if (vol > 127) {
            vol = 127;
        }
    }

    if (delta_time == 0) {
        snd_RemoveEffect(handler, 2);
        if (stopit == 1) {
            snd_StopSound(handle);
        } else {
            snd_SetSoundVolPan(handle, -vol, -2);
        }

        snd_UnlockMasterTick();
        return;
    }

    volchange = vol - handler->Current_Vol;
    if (volchange == 0) {
        snd_RemoveEffect(handler, 2);
        snd_UnlockMasterTick();
        return;
    }

    effect = snd_FindEffect(handler, 2);
    if (effect) {
        effect->ec.Flags |= 0x80000000;
        if (delta_from == 3) {
            wouldhavechanged = vol - effect->destination;
            if (wouldhavechanged == 0) {
                snd_RemoveEffect(handler, 2);
                snd_SetSoundVolPan(handle, -vol, -2);
                snd_UnlockMasterTick();
                return;
            }

            delta_time = abs(volchange * delta_time / wouldhavechanged);
            if (delta_time <= 0) {
                delta_time = 1;
            }
        }
    } else {
        effect = snd_GetFreeBasicEffect();
        if (effect == NULL) {
            snd_SetSoundVolPan(handle, -vol, -2);
            snd_UnlockMasterTick();
            return;
        }
        effect->ec.Flags |= 2;
        effect->ec.proc = snd_AutoVolHandler;
        new_effect = 1;
    }

    if (abs(volchange) >= delta_time) {
        effect->delta_value = 32 * volchange / delta_time;
        effect->ec.delta_type = 0;
        effect->ec.delta_counter = 0;
    } else {
        effect->delta_time = abs(delta_time / volchange);
        effect->ec.delta_type = 1;
        effect->ec.delta_counter = effect->delta_time;
        if (vol >= handler->Current_Vol) {
            effect->delta_value = 1;
        } else {
            effect->delta_value = -1;
        }
    }

    if (stopit == 1) {
        effect->destination = -4;
    } else {
        effect->destination = vol;
    }

    if (new_effect) {
        snd_AddEffectToHandler(handler, &effect->ec);
    } else {
        effect->ec.Flags &= ~0x80000000;
    }

    snd_UnlockMasterTick();
}

/* 00008c00 00008e24 */ SInt32 snd_AutoVolHandler(/* 0x0(sp) */ BasicEffectPtr effect, /* 0x4(sp) */ GSoundHandlerPtr handler) {
    /* -0x10(sp) */ SInt16 newMvol;
    /* -0xe(sp) */ SInt16 stopit = 0;

    if (effect->destination == -4) {
        effect->destination = 0;
        stopit = 1;
    }

    if (abs(effect->delta_value) < 2) {
        newMvol = handler->Current_Vol + effect->delta_value;
    } else {
        newMvol = (32 * handler->Current_Vol + effect->delta_value) >> 5;
    }

    if ((effect->delta_value > 0 && effect->destination < newMvol) ||
        (effect->delta_value < 0 && effect->destination >= newMvol)) {
        newMvol = effect->destination;
    }

    snd_SetSoundVolPan(handler->OwnerID, -newMvol, -2);
    if (newMvol == effect->destination) {
        snd_RemoveEffectFromHandler(handler, &effect->ec);
        return stopit;
    }

    if (effect->ec.delta_type == 1) {
        effect->ec.delta_counter = effect->delta_time;
    }

    if (stopit) {
        effect->destination = -4;
    }

    return 0;
}
