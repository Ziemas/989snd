#ifndef SNDHAND_H_
#define SNDHAND_H_

#include "types.h"

enum SND_TYPE {
    SOUND_MIDI = 1,
    SOUND_AME = 2,
    SOUND_VAG = 4,
    SOUND_BLOCK = 5,
};

enum SFX_FLAG {
    SFX_INSTLIMIT = 0x8,
    SFX_INSTLIMIT_VOL = 0x10,
    SFX_INSTLIMIT_TICK = 0x20,
};

#define NUM_BLOCK_HANDLER (64)
#define NUM_MIDI_HANDLER (32)
#define NUM_AME_HANDLER (4)

#define SND_GET_TYPE(a) (((a) >> 24) & 0x1f)
#define SND_GET_INDEX(a) (((a) >> 16) & 0xff)

void snd_InitSoundHandlers();

void snd_StopHandlerPtr(GSoundHandlerPtr snd, SInt32 and_child, SInt32 silence, bool vlimit_stop);

GSoundHandlerPtr snd_CheckHandlerStillActive(UInt32 handle);

void snd_FreeEffectChain(EffectChainPtr effect);

#endif // SNDHAND_H_
