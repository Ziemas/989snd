#include "loader.h"
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

/* 0000ff74 00010008 */ SInt32 snd_TimerLLTick(/* 0x0(sp) */ struct timercommon *common) {
    iWakeupThread(*common->thid);

    if (gKillTimer) {
        gKillTimer = false;
        return 0;
    }

    return common->compare;
}

/* 00010008 000100e0 */ int snd_TransCallback(/* 0x0(sp) */ SInt32 ch, /* 0x4(sp) */ void *data) {
    if (gTransfering == 0) {
        return 1;
    }

    snd_FreeSPUDMA(gTransfering - 1);
    gTransfering = 0;

    if (gTransferDoneCallback != NULL) {
        gTransferDoneCallback();
    }

    if (gSPURAMTransSema >= 0) {
        iSignalSema(gSPURAMTransSema);
    }

    return 0;
}

/* 000100e0 00010164 */ void snd_DoneVAGReadCB(/* 0x0(sp) */ SInt32 code) {
    gCdBusy = 0;
    if (code != 1) {
        gOddCdError = 1;
        gOddCdErrorCode = code;
    }

    iSignalSema(gDoneLoadSema);
}

/* 00010164 0001022c */ int snd_VAGDMADone0(/* 0x0(sp) */ int ch, /* 0x4(sp) */ void *data) {
    gDMAInUse[ch] = 0;

    if (ch == 0) {
        gLastDMA0Complete = 1;
    } else {
        gLastDMA1Complete = 1;
    }

    iSignalSema(gDoneDMASema);
    if (gWaitingDMAComplete > 0) {
        iWakeupThread(gWaitingDMAComplete);
        gWaitingDMAComplete = 0;
    }

    return 0;
}
