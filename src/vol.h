#ifndef VOL_H_
#define VOL_H_

#include "types.h"

enum {
    PLAYBACK_MODE_MONO = 1,

    VOL_NUM_DUCKERS = 4,
    VOL_NUM_GROUPS = 15,
    VOL_GROUP_MASTER = 16,
};

void snd_InitDuckers();

SInt16 snd_MIDITo360Pan(SInt8 pan);
void snd_CheckDuckers();

void snd_SetMasterVolume(SInt32 which, SInt32 vol);
SInt32 snd_GetMasterVolume(SInt32 which);
void snd_SetMasterVolumeDucker(SInt32 which, DuckerDefPtr state);

SInt16 snd_AdjustVolToGroup(SInt16 vol, SInt32 group);
void snd_AdjustAllChannelsMasterVolume(UInt32 flags);

void snd_SetPlaybackMode(SInt32 mode);
SInt32 snd_GetPlaybackMode();

void snd_MakeVolumesB(SInt32 bank_vol, SInt32 snd_vol,
                      SInt32 snd_pan, SInt32 prog_vol,
                      SInt32 prog_pan, SInt32 tone_vol,
                      SInt32 tone_pan, SpuVolume *spu_vol);
#endif // VOL_H_
