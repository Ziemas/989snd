#ifndef PLAYSND_H_
#define PLAYSND_H_

#include "types.h"

enum {
    PAN_DEFAULT = -1,

    SND_TYPE_UNK = 3,
    SND_TYPE_MIDI = 4,
    SND_TYPE_AME = 5,
};

enum PLAY_PARAM_FLAGS_BIT {
    PARAM_BANK_PTR = 0,
    PARAM_SOUND_INDEX = 1,
    PARAM_VOL = 2,
    PARAM_PAN = 3,
    PARAM_PITCH = 4,
    PARAM_BEND = 5,
};

UInt32 snd_PlaySoundVolPanPMPB(SoundBankPtr bank, SInt32 sound, SInt32 vol, SInt32 pan, int pitch_mod, int bend);

void snd_SetSoundPitchBend(UInt32 handle, SInt16 bend);
void snd_SetSoundPitchModifier(UInt32 handle, SInt16 mod);

void snd_StopSound(UInt32 handle);

SoundBankPtr snd_FindBankByName(char *name);
SInt32 snd_FindSoundByName(SFXBlock2Ptr block, char *name, SFXBlock2Ptr *found_block);

SInt32 snd_DEBUG_CheckSolo(SoundBankPtr bank, SInt32 sound);
#endif // PLAYSND_H_
