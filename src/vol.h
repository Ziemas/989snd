#ifndef VOL_H_
#define VOL_H_

#include "types.h"

void snd_InitDuckers();

void snd_SetMasterVolume(SInt32 which, SInt32 vol);
void snd_SetMasterVolumeDucker(SInt32 which, DuckerDefPtr state);

void snd_SetPlaybackMode(SInt32 mode);

#endif // VOL_H_
