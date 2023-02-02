#ifndef INTR_CODE_H_
#define INTR_CODE_H_

#include "types.h"

extern SInt32 gTransfering;
extern SpuTransferCallbackProc gTransferDoneCallback;
extern SInt32 gSPURAMTransSema;
extern bool gKillTimer;
extern SInt32 gDMAInUse[2];
extern SInt32 gCdBusy;
extern SInt32 gCdBusyTime;

extern SInt32 gWaitingDMAComplete;

SInt32 snd_TimerLLTick(struct timercommon *common);
int snd_TransCallback(SInt32 ch, void *data);

#endif // INTR_CODE_H_
