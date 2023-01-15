#ifndef VALLOC_H_
#define VALLOC_H_

#include "types.h"

extern UInt32 gVAllocOwnerID;
extern UInt32 gAwaitingKeyOn[2];
extern UInt32 gAwaitingKeyOff[2];

void snd_InitVoiceAllocator();
void snd_CleanupVoiceAllocator();

void snd_StopPendingVoices();

#endif // VALLOC_H_
