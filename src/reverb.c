#include "types.h"
/* data 1bc0 */ SInt32 gReverbType[2] = {-1, -1};
/* data 1bc8 */ UInt32 gRevRamStart[2] = {0, 0};
/* data 1bd0 */ SInt32 gRevRamPreAllocType[2] = {-1, -1};
/* data 1bd8 */ SInt16 gReverbDepthL[2] = {0, 0};
/* data 1bdc */ SInt16 gReverbDepthR[2] = {0, 0};
/* data 1be0 */ SInt32 gAwaitingReverbChange[2] = {0, 0};
/* data 1be8 */ SInt32 gAwaitingReverbChangeTick[2] = {0, 0};
/* data 1bf0 */ SInt32 gReverbMode = 0;
/* data 1bf4 */ UInt32 gRevRamUse[0];

/* 0001c8e8 0001c980 */ void snd_SetReverbMode(/* 0x0(sp) */ SInt32 reverb_mode) {
    /* -0x10(sp) */ UInt16 core1mode;
}

/* 0001c980 0001c9f8 */ void snd_PreAllocReverbWorkArea(/* 0x0(sp) */ UInt32 core, /* 0x4(sp) */ SInt32 type) {}
/* 0001c9f8 0001cc6c */ void snd_PreAllocReverbWorkArea_L(/* 0x0(sp) */ SInt32 core, /* 0x4(sp) */ SInt32 type) {
    /* -0x10(sp) */ SInt32 dis;
    /* -0xc(sp) */ SInt32 oldstat;
}

/* 0001cc6c 0001cd24 */ void snd_SetReverbEx(/* 0x0(sp) */ UInt32 core, /* 0x4(sp) */ SInt32 type, /* 0x8(sp) */ SInt32 depth, /* 0xc(sp) */ SInt32 delay, /* 0x10(sp) */ SInt32 feedback) {}
/* 0001cd24 0001cdbc */ void snd_SetReverbType(/* 0x0(sp) */ SInt32 core, /* 0x4(sp) */ SInt32 type) {
    /* -0x10(sp) */ SInt32 ret1;
    /* -0xc(sp) */ SInt32 ret2;
}

/* 0001cdbc 0001d3b8 */ SInt32 snd_SetReverbType_L(/* 0x0(sp) */ SInt32 core, /* 0x4(sp) */ SInt32 type, /* 0x8(sp) */ SInt32 delay, /* 0xc(sp) */ SInt32 feedback) {
    /* -0x28(sp) */ sceSdEffectAttr ef;
    /* -0x14(sp) */ SInt32 ch;
    /* -0x10(sp) */ SInt32 dis;
    /* -0xc(sp) */ SInt32 oldstat;
}

/* 0001d3b8 0001d454 */ void snd_SetReverbDepth(/* 0x0(sp) */ SInt32 core, /* 0x4(sp) */ SInt32 left, /* 0x8(sp) */ SInt32 right) {}
/* 0001d454 0001d5a8 */ void snd_SetReverbDepth_L(/* 0x0(sp) */ SInt32 core, /* 0x4(sp) */ SInt32 left, /* 0x8(sp) */ SInt32 right) {
    /* -0x10(sp) */ SInt32 intr_state;
    /* -0xc(sp) */ SInt32 dis;
}

/* 0001d5a8 0001d778 */ void snd_CheckVerb() {
    /* -0x10(sp) */ SInt32 intr_state;
    /* -0xc(sp) */ SInt32 dis;
}
