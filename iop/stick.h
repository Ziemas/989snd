#ifndef STICK_H_
#define STICK_H_

#include "types.h"

extern SInt32 gLockMasterTick;
extern UInt32 gMasterTickOwner[32];
extern SInt8 gAutoFreeState[48];

SInt32 snd_SoundTickHandler();

void snd_LockMasterTick(UInt32 ownerID);
void snd_UnlockMasterTick();

UInt32 snd_GetTick();

#endif // STICK_H_
