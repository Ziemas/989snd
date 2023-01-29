#ifndef REVERB_H_
#define REVERB_H_

#include "types.h"

extern SInt32 gReverbMode;

void snd_CheckVerb();

void snd_PreAllocReverbWorkArea(UInt32 core, SInt32 type);
void snd_PreAllocReverbWorkArea_L(SInt32 core, SInt32 type);
void snd_SetReverbMode(SInt32 reverb_mode);
void snd_SetReverbEx(UInt32 core, SInt32 type, SInt32 depth, SInt32 delay, SInt32 feedback);
void snd_SetReverbType(SInt32 core, SInt32 type);
SInt32 snd_SetReverbType_L(SInt32 core, SInt32 type, SInt32 delay, SInt32 feedback);
void snd_SetReverbDepth(SInt32 core, SInt32 left, SInt32 right);
void snd_SetReverbDepth_L(SInt32 core, SInt32 left, SInt32 right);

#endif // REVERB_H_
