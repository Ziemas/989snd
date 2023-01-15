#include "stick.h"
#include "types.h"
/* data 1c2c */ static UInt32 sTick = 0;
/* data 1c30 */ SInt32 gLockMasterTick = 0;
/* data 1c34 */ UInt32 gMasterTickOwner[32] = {};
/* data 1cb4 */ UInt32 gMasterTickOwnerLast = 0;
/* bss 8d00 */ SInt16 gEnvelopeStatus[48];
/* bss 8d60 */ SInt8 gAutoFreeState[48];

/* 00021a9c 00021b48 */ void snd_LockMasterTick(/* 0x0(sp) */ UInt32 ownerID) {}
/* 00021b48 00021c38 */ void snd_UnlockMasterTick() {}
/* 00021c38 000221fc */ SInt32 snd_SoundTickHandler() {
    /* -0x18(sp) */ SInt16 env;
    /* -0x14(sp) */ struct VoiceAttributes *walk;
    /* data 1cb8 */ static SInt32 in_handler;
    /* -0x10(sp) */ SInt32 core;
    /* -0xc(sp) */ SInt32 c_v;
}

/* 000221fc 00022234 */ UInt32 snd_GetTick() {}
/* 00022234 00022264 */ void snd_SetTick(/* 0x0(sp) */ UInt32 val) {}
