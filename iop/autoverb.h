#ifndef AUTOVERB_H_
#define AUTOVERB_H_

#include "types.h"

void snd_AutoReverb(SInt32 core, SInt32 depth, SInt32 delta_time, SInt32 channel_flags);

void snd_KillAutoVerbForCore(SInt32 core);
void snd_AutoVerbHandler();

#endif // AUTOVERB_H_
