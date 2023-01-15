#ifndef REVERB_H_
#define REVERB_H_

#include "types.h"

void snd_SetReverbType(SInt32 core, SInt32 type);
void snd_SetReverbDepth(SInt32 core, SInt32 left, SInt32 right);

#endif // REVERB_H_
