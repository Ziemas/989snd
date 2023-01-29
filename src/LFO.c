#include "types.h"
#include "util.h"

#include "lfo_sine.inc"
/* data 11e8 */ struct LFOTracker *gActiveLFOs = NULL;
/* data 11ec */ SInt32 gNumLFOsInUse = 0;

/* 00003e5c 00003e90 */ SInt32 snd_LFO_TYPE_OFF(/* 0x0(sp) */ struct LFOTracker *tracker, /* 0x4(sp) */ int step) {
    UNIMPLEMENTED();
}
/* 00003e90 00003ee8 */ SInt32 snd_LFO_TYPE_SINE(/* 0x0(sp) */ struct LFOTracker *tracker, /* 0x4(sp) */ int step) {
    UNIMPLEMENTED();
}
/* 00003ee8 00003f44 */ SInt32 snd_LFO_TYPE_SQUARE(/* 0x0(sp) */ struct LFOTracker *tracker, /* 0x4(sp) */ int step) {
    UNIMPLEMENTED();
}
/* 00003f44 00004058 */ SInt32 snd_LFO_TYPE_TRIANGLE(/* 0x0(sp) */ struct LFOTracker *tracker, /* 0x4(sp) */ int step) {
    /* -0x10(sp) */ SInt32 ret_val;
    UNIMPLEMENTED();
}

/* 00004058 00004120 */ SInt32 snd_LFO_TYPE_SAW(/* 0x0(sp) */ struct LFOTracker *tracker, /* 0x4(sp) */ int step) {
    /* -0x10(sp) */ SInt32 ret_val;
    UNIMPLEMENTED();
}

/* 00004120 00004254 */ SInt32 snd_LFO_TYPE_RAND(/* 0x0(sp) */ struct LFOTracker *tracker, /* 0x4(sp) */ int step) {
    UNIMPLEMENTED();
}

/* data 11f0 */ LFOFunction gLFOFuncs[] = {
    snd_LFO_TYPE_OFF,
    snd_LFO_TYPE_SINE,
    snd_LFO_TYPE_SQUARE,
    snd_LFO_TYPE_TRIANGLE,
    snd_LFO_TYPE_SAW,
    snd_LFO_TYPE_RAND,
};

/* 00004254 00004364 */ SInt32 snd_GetLFO(/* s1 17 */ struct LFOTracker *tracker, /* s0 16 */ SInt32 step_mult) {
    /* -0x18(sp) */ SInt32 ret_val;
    /* -0x14(sp) */ SInt32 step;
    UNIMPLEMENTED();
}

/* 00004364 0000444c */ void snd_InitLFO(/* 0x0(sp) */ struct LFOTracker *tracker) {}
/* 0000444c 000044e4 */ void snd_RemoveLFOsForHandler(/* 0x0(sp) */ BlockSoundHandlerPtr handler) {
    /* -0x10(sp) */ SInt32 i;
    UNIMPLEMENTED();
}

/* 000044e4 00004628 */ void snd_RemoveLFO(/* 0x0(sp) */ struct LFOTracker *lfo) {
    /* -0x10(sp) */ struct LFOTracker *walk;
    UNIMPLEMENTED();
}

/* 00004628 00004774 */ bool snd_InsertLFO(/* 0x0(sp) */ struct LFOTracker *lfo) {
    /* -0x10(sp) */ struct LFOTracker *walk;
    UNIMPLEMENTED();
}

/* 00004774 00004b44 */ UInt32 snd_DoOneLFO(/* 0x0(sp) */ struct LFOTracker *tracker) {
    /* -0x20(sp) */ BlockSoundHandlerPtr handler;
    /* -0x1c(sp) */ struct LFOTracker *target_tracker;
    /* -0x18(sp) */ SInt32 range;
    /* -0x14(sp) */ SInt32 lfo;
    /* -0x10(sp) */ SInt32 new_val;
    /* -0xc(sp) */ UInt32 ret;
    UNIMPLEMENTED();
}

/* 00004b44 00004c44 */ void snd_HandleLFOs() {
    /* -0x10(sp) */ struct LFOTracker *tracker;
    /* -0xc(sp) */ UInt32 flags;
    UNIMPLEMENTED();
}

/* 00004c44 00004dc8 */ void snd_CalcLFODepth(/* 0x0(sp) */ struct LFOTracker *lfo) {
    /* -0x10(sp) */ SFX2 *sfx;
    UNIMPLEMENTED();
}
