#ifndef VOL_H_
#define VOL_H_

#include "types.h"

void snd_InitDuckers();

void snd_SetMasterVolume(SInt32 which, SInt32 vol);
void snd_SetMasterVolumeDucker(SInt32 which, DuckerDefPtr state);

SInt16 snd_AdjustVolToGroup(SInt16 vol, SInt32 group);

void snd_SetPlaybackMode(SInt32 mode);

#endif // VOL_H_
