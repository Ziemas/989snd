#ifndef SNDHAND_H_
#define SNDHAND_H_

#include "types.h"

enum HND_TYPE {
    HANDLER_UNK = 0,
    HANDLER_MIDI = 1,
    HANDLER_AME = 2,
    HANDLER_AME_MIDI = 3,
    HANDLER_VAG = 4,
    HANDLER_BLOCK = 5,
};

enum HND_FLAG {
    HND_PAUSED = 2,
    HND_UNK4 = 4,
};

enum SFX_FLAG {
    SFX_INSTLIMIT = 0x8,
    SFX_INSTLIMIT_VOL = 0x10,
    SFX_INSTLIMIT_TICK = 0x20,
};

enum NUM_HANDLERS {
    NUM_EFFECTS = 64,
    NUM_BLOCK_HANDLER = 64,
    NUM_MIDI_HANDLER = 32,
    NUM_AME_HANDLER = 4
};

#define HND_GET_ACTIVE(a) ((a)&0x80000000)
#define HND_GET_TYPE(a) (((a) >> 24) & 0x1f)
#define HND_GET_INDEX(a) (((a) >> 16) & 0xff)

void snd_InitSoundHandlers();

SInt32 snd_GetNextHandlerVoice(GSoundHandlerPtr snd, SInt32 start_v);
void snd_UpdateHandlers();

void snd_StopHandlerPtr(GSoundHandlerPtr snd, SInt32 and_child, SInt32 silence, bool vlimit_stop);
void snd_StopAllHandlersForSound(SoundPtr snd, SInt32 silence, bool vlimit_stop);

BlockSoundHandlerPtr snd_GetFreeBlockSoundHandler(SFX2 *sfx, SInt32 vol, bool parent);
MIDIHandlerPtr snd_GetFreeMIDIHandler();
AMEHandlerPtr snd_GetFreeAMEHandler();

void snd_ActivateHandler(GSoundHandlerPtr snd);
void snd_DeactivateHandler(GSoundHandlerPtr snd, SInt32 and_child);

SInt32 snd_UpdateEffect(EffectChainPtr effect, GSoundHandlerPtr owner);

GSoundHandlerPtr snd_CheckHandlerStillActive(UInt32 handle);

void snd_FreeEffectChain(EffectChainPtr effect);
EffectChainPtr snd_FindEffect(GSoundHandlerPtr handler, SInt32 type_flag);

void snd_RemoveEffectFromHandler(GSoundHandlerPtr handler, EffectChainPtr effect);
void snd_PauseHandlerPtr(GSoundHandlerPtr snd, SInt32 and_child);
void snd_PauseAllSoundsInGroup(UInt32 groups);
void snd_ContinueHandlerPtr(GSoundHandlerPtr snd, SInt32 and_child);
void snd_ContinueAllSoundsInGroup(UInt32 groups);
void snd_StopAllSoundsInGroup(UInt32 groups);
void snd_StopAllSounds();

#endif // SNDHAND_H_
