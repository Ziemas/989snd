#ifndef INIT_H_
#define INIT_H_

#include "types.h"
#include <stdbool.h>

extern bool gPrefs_ShowMissingSounds;
extern bool gPrefs_Silent;

SInt32 snd_GetTickRate();
SInt32 snd_InstallSoundTick();

SInt32 snd_TimerLLIRQLoop();

#endif // INIT_H_
