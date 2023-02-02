#ifndef INIT_H_
#define INIT_H_

#include "types.h"
#include <stdbool.h>

extern UInt32 gMicsPerTick;
extern bool gPrefs_ShowMissingSounds;
extern bool gPrefs_Silent;

SInt32 snd_SystemRunning();
SInt32 snd_StartSoundSystemEx(UInt32 flags);

SInt32 snd_GetTickRate();
SInt32 snd_InstallSoundTick();

SInt32 snd_StopSoundSystem();
SInt32 snd_TimerLLIRQLoop();

#endif // INIT_H_
