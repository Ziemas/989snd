#include "types.h"
/* data 1308 */ bool gKillTimer = false;
/* data 130c */ SInt32 gTransfering = 0;
/* data 1310 */ SpuTransferCallbackProc gTransferDoneCallback = NULL;
/* data 1314 */ SInt32 gSPURAMTransSema = -1;
/* data 1318 */ SInt32 gDoneLoadSema = 0;
/* data 131c */ SInt32 gOddCdError = 0;
/* data 1320 */ SInt32 gOddCdErrorCode = 0;
/* data 1324 */ SInt32 gCdBusy = 0;
/* data 1328 */ SInt32 gCdBusyTime = 0;
/* data 132c */ SInt32 gDMAInUse[2] = {0, 0};
/* data 1334 */ SInt32 gLastDMA0Complete = 0;
/* data 1338 */ SInt32 gLastDMA1Complete = 0;
/* data 133c */ SInt32 gDoneDMASema = 0;
/* data 1340 */ SInt32 gWaitingDMAComplete = 0;

/* 0000ff74 00010008 */ SInt32 snd_TimerLLTick(/* 0x0(sp) */ struct timercommon* common) {}
/* 00010008 000100e0 */ int snd_TransCallback(/* 0x0(sp) */ SInt32 ch, /* 0x4(sp) */ void* data) {}
/* 000100e0 00010164 */ void snd_DoneVAGReadCB(/* 0x0(sp) */ SInt32 code) {}
/* 00010164 0001022c */ int snd_VAGDMADone0(/* 0x0(sp) */ int ch, /* 0x4(sp) */ void* data) {}
