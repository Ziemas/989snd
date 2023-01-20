#ifndef INTR_CODE_H_
#define INTR_CODE_H_

#include "types.h"

extern bool gKillTimer;
extern SInt32 gCdBusy;
extern SInt32 gCdBusyTime;

SInt32 snd_TimerLLTick(struct timercommon *common);

#endif // INTR_CODE_H_
