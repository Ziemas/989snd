#include "types.h"

/* bss 110 */ struct BasicEffect gVerbFX[2][2];

/* 00007c20 00007cb0 */ void snd_AutoReverb(/* 0x0(sp) */ SInt32 core, /* 0x4(sp) */ SInt32 depth, /* 0x8(sp) */ SInt32 delta_time, /* 0xc(sp) */ SInt32 channel_flags) {}
/* 00007cb0 000081e8 */ void snd_AutoReverb_L(/* 0x0(sp) */ SInt32 core, /* 0x4(sp) */ SInt32 depth, /* 0x8(sp) */ SInt32 delta_time, /* 0xc(sp) */ SInt32 channel_flags) {
    /* -0x18(sp) */ SInt32 depthchange;
    /* -0x14(sp) */ SInt16 original_depth;
    /* -0x10(sp) */ SInt32 effect_index;
}

/* 000081e8 0000825c */ void snd_KillAutoVerbForCore(/* 0x0(sp) */ SInt32 core) {}
/* 0000825c 00008738 */ void snd_AutoVerbHandler() {
    /* -0x10(sp) */ SInt32 core;
    /* -0xc(sp) */ SInt32 newdepth;
}
