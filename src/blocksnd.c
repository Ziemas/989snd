#include "blocksnd.h"
#include "types.h"
#include "util.h"

/* data 1208 */ SInt8 gBlockGlobalReg[32] = {};
/* data 1228 */ GrainHandler gGrainHandler[] = {
    snd_SFX_GRAIN_TYPE_TONE,
    snd_SFX_GRAIN_TYPE_XREF_ID,
    snd_SFX_GRAIN_TYPE_XREF_NUM,
    snd_SFX_GRAIN_TYPE_LFO_SETTINGS,
    snd_SFX_GRAIN_TYPE_STARTCHILDSOUND,
    snd_SFX_GRAIN_TYPE_STOPCHILDSOUND,
    snd_SFX_GRAIN_TYPE_PLUGIN_MESSAGE,
    snd_SFX_GRAIN_TYPE_BRANCH,
    snd_SFX_GRAIN_TYPE_TONE,
    snd_SFX_UNKNOWN_GRAIN_TYPE,
    snd_SFX_UNKNOWN_GRAIN_TYPE,
    snd_SFX_UNKNOWN_GRAIN_TYPE,
    snd_SFX_UNKNOWN_GRAIN_TYPE,
    snd_SFX_UNKNOWN_GRAIN_TYPE,
    snd_SFX_UNKNOWN_GRAIN_TYPE,
    snd_SFX_UNKNOWN_GRAIN_TYPE,
    snd_SFX_UNKNOWN_GRAIN_TYPE,
    snd_SFX_UNKNOWN_GRAIN_TYPE,
    snd_SFX_UNKNOWN_GRAIN_TYPE,
    snd_SFX_GRAIN_TYPE_CONTROL_NULL,
    snd_SFX_GRAIN_TYPE_LOOP_START,
    snd_SFX_GRAIN_TYPE_LOOP_END,
    snd_SFX_GRAIN_TYPE_LOOP_CONTINUE,
    snd_SFX_GRAIN_TYPE_STOP,
    snd_SFX_GRAIN_TYPE_RAND_PLAY,
    snd_SFX_GRAIN_TYPE_RAND_DELAY,
    snd_SFX_GRAIN_TYPE_RAND_PB,
    snd_SFX_GRAIN_TYPE_PB,
    snd_SFX_GRAIN_TYPE_ADD_PB,
    snd_SFX_GRAIN_TYPE_SET_REGISTER,
    snd_SFX_GRAIN_TYPE_SET_REGISTER_RAND,
    snd_SFX_GRAIN_TYPE_INC_REGISTER,
    snd_SFX_GRAIN_TYPE_DEC_REGISTER,
    snd_SFX_GRAIN_TYPE_TEST_REGISTER,
    snd_SFX_GRAIN_TYPE_MARKER,
    snd_SFX_GRAIN_TYPE_GOTO_MARKER,
    snd_SFX_GRAIN_TYPE_GOTO_RANDOM_MARKER,
    snd_SFX_GRAIN_TYPE_WAIT_FOR_ALL_VOICES,
    snd_SFX_GRAIN_TYPE_PLAY_CYCLE,
    snd_SFX_GRAIN_TYPE_ADD_REGISTER,
    snd_SFX_GRAIN_TYPE_KEY_OFF_VOICES,
    snd_SFX_GRAIN_TYPE_KILL_VOICES,
    snd_SFX_GRAIN_TYPE_ON_STOP_MARKER,
    snd_SFX_GRAIN_TYPE_COPY_REGISTER,
};

/* 00008e24 00008ebc */ UInt32 snd_PlaySFX(/* 0x0(sp) */ SFXBlock2Ptr block, /* 0x4(sp) */ SInt32 sound, /* 0x8(sp) */ SInt32 vol, /* 0xc(sp) */ SInt32 pan, /* 0x10(sp) */ int pitch_mod, /* 0x14(sp) */ int bend) {
    /* -0x10(sp) */ SInt16 _pitch_mod;
    /* -0xe(sp) */ SInt16 _bend;
    UNIMPLEMENTED();
}

/* 00008ebc 000096f4 */ UInt32 snd_PlaySFXWithStruct(/* 0x0(sp) */ SFXBlock2Ptr block, /* 0x4(sp) */ SInt32 sound, /* -0x30(sp) */ SInt16 sfx_vol, /* -0x2e(sp) */ SInt16 sfx_pan, /* 0x10(sp) */ SndPlayParamsPtr params) {
    /* -0x2c(sp) */ SInt32 play_vol;
    /* -0x28(sp) */ SFX2 *sfx;
    /* -0x24(sp) */ BlockSoundHandlerPtr hand;
    /* -0x20(sp) */ SInt32 stop_sound;
    /* -0x1c(sp) */ SInt32 i;
    /* -0x18(sp) */ SInt32 vol;
    /* -0x14(sp) */ SInt32 pan;
    /* -0x10(sp) */ SInt16 pitch_mod;
    /* -0xe(sp) */ SInt16 bend;
    UNIMPLEMENTED();
}

/* 000096f4 000097bc */ UInt32 snd_PlaySFX_EX(/* 0x0(sp) */ SFXBlock2Ptr block, /* 0x4(sp) */ SInt32 sound, /* 0x8(sp) */ SInt32 vol, /* 0xc(sp) */ SInt32 pan, /* 0x10(sp) */ int pitch_mod, /* 0x14(sp) */ int bend, /* 0x18(sp) */ int sfx_vol, /* 0x1c(sp) */ int sfx_pan) {
    /* -0x40(sp) */ SInt16 _pitch_mod;
    /* -0x3e(sp) */ SInt16 _bend;
    /* -0x3c(sp) */ SInt16 _sfx_vol;
    /* -0x3a(sp) */ SInt16 _sfx_pan;
    /* -0x38(sp) */ struct SndPlayParams params;

    UNIMPLEMENTED();
}

/* 000097bc 00009a98 */ UInt32 snd_PlaySFXGrain(/* 0x0(sp) */ SFXBlock2Ptr block, /* 0x4(sp) */ SInt32 sindex, /* 0x8(sp) */ SInt32 gindex) {
    /* -0x10(sp) */ SFX2 *sfx;
    /* -0xc(sp) */ BlockSoundHandlerPtr hand;
    UNIMPLEMENTED();
}

/* 00009a98 00009b20 */ SInt8 snd_GetSFXGlobalReg(/* 0x0(sp) */ SInt32 which) {}
/* 00009b20 00009ba4 */ void snd_SetSFXGlobalReg(/* 0x0(sp) */ SInt32 which, /* -0x10(sp) */ SInt8 val) {}
/* 00009ba4 00009c88 */ SInt8 snd_GetSFXSoundReg(/* 0x0(sp) */ UInt32 handle, /* 0x4(sp) */ SInt32 which) {
    /* -0x10(sp) */ BlockSoundHandlerPtr hand;
    /* -0xc(sp) */ SInt8 val;
    UNIMPLEMENTED();
}

/* 00009c88 00009d58 */ void snd_SetSFXSoundReg(/* 0x0(sp) */ UInt32 handle, /* 0x4(sp) */ SInt32 which, /* -0x10(sp) */ SInt8 val) {
    /* -0xc(sp) */ BlockSoundHandlerPtr hand;
    UNIMPLEMENTED();
}

/* 00009d58 00009e34 */ void snd_SetAllSFXSoundReg(/* 0x0(sp) */ UInt32 handle, /* 0x4(sp) */ SInt8 *vals) {
    /* -0x10(sp) */ BlockSoundHandlerPtr hand;
    /* -0xc(sp) */ SInt32 i;
    UNIMPLEMENTED();
}

/* 00009e34 00009f78 */ SInt32 snd_ProcessBlockSoundTick(/* 0x0(sp) */ BlockSoundHandlerPtr handler) {
    /* -0x10(sp) */ SInt32 stop_sound;
    UNIMPLEMENTED();
}

/* 00009f78 0000a140 */ bool snd_DoBlockSoundStop(/* 0x0(sp) */ BlockSoundHandlerPtr handler, /* 0x4(sp) */ SInt32 silence, /* 0x8(sp) */ bool vlimit_stop) {
    /* -0x10(sp) */ SInt32 stop_sound;
    /* -0xc(sp) */ SInt32 stop_index;
    UNIMPLEMENTED();
}

/* 0000a140 0000a23c */ SInt32 snd_GetOnStopGrainIndex(/* 0x0(sp) */ BlockSoundHandlerPtr handler) {
    /* -0x10(sp) */ SInt32 i;
    /* -0xc(sp) */ SFX2Ptr sfx;
    UNIMPLEMENTED();
}

/* 0000a23c 0000a274 */ SInt32 snd_SFX_GRAIN_TYPE_NULL(/* 0x0(sp) */ BlockSoundHandlerPtr handler, /* 0x4(sp) */ SFX2Ptr sfx, /* 0x8(sp) */ SFXGrain2Ptr grain) {}
/* 0000a274 0000ae90 */ SInt32 snd_SFX_GRAIN_TYPE_TONE(/* 0x0(sp) */ BlockSoundHandlerPtr handler, /* 0x4(sp) */ SFX2Ptr sfx, /* 0x8(sp) */ SFXGrain2Ptr grain) {
    /* -0x20(sp) */ SInt32 voice;
    /* -0x1c(sp) */ SInt32 g_pan;
    /* -0x18(sp) */ SInt32 priority;
    /* -0x14(sp) */ TonePtr tone;
    /* -0x10(sp) */ SInt32 core;
    /* -0xc(sp) */ SInt32 c_v;
}

/* 0000ae90 0000aec8 */ SInt32 snd_SFX_GRAIN_TYPE_XREF_ID(/* 0x0(sp) */ BlockSoundHandlerPtr handler, /* 0x4(sp) */ SFX2Ptr sfx, /* 0x8(sp) */ SFXGrain2Ptr grain) {
    UNIMPLEMENTED();
}
/* 0000aec8 0000af00 */ SInt32 snd_SFX_GRAIN_TYPE_XREF_NUM(/* 0x0(sp) */ BlockSoundHandlerPtr handler, /* 0x4(sp) */ SFX2Ptr sfx, /* 0x8(sp) */ SFXGrain2Ptr grain) {
    UNIMPLEMENTED();
}
/* 0000af00 0000b18c */ SInt32 snd_SFX_GRAIN_TYPE_LFO_SETTINGS(/* 0x0(sp) */ BlockSoundHandlerPtr handler, /* 0x4(sp) */ SFX2Ptr sfx, /* 0x8(sp) */ SFXGrain2Ptr grain) {
    /* -0x18(sp) */ struct LFOTracker *lfo;
    /* -0x14(sp) */ SInt32 index;
    /* -0x10(sp) */ LFOParams *lfop;
    UNIMPLEMENTED();
}

/* 0000b18c 0000b1c4 */ SInt32 snd_SFX_GRAIN_TYPE_CONTROL_NULL(/* 0x0(sp) */ BlockSoundHandlerPtr handler, /* 0x4(sp) */ SFX2Ptr sfx, /* 0x8(sp) */ SFXGrain2Ptr grain) {
    UNIMPLEMENTED();
}
/* 0000b1c4 0000b1fc */ SInt32 snd_SFX_GRAIN_TYPE_LOOP_START(/* 0x0(sp) */ BlockSoundHandlerPtr handler, /* 0x4(sp) */ SFX2Ptr sfx, /* 0x8(sp) */ SFXGrain2Ptr grain) {
    UNIMPLEMENTED();
}
/* 0000b1fc 0000b328 */ SInt32 snd_SFX_GRAIN_TYPE_LOOP_END(/* 0x0(sp) */ BlockSoundHandlerPtr handler, /* 0x4(sp) */ SFX2Ptr sfx, /* 0x8(sp) */ SFXGrain2Ptr grain) {
    /* -0x10(sp) */ SInt32 found;
    /* -0xc(sp) */ SInt32 i;
    UNIMPLEMENTED();
}

/* 0000b328 0000b45c */ SInt32 snd_SFX_GRAIN_TYPE_LOOP_CONTINUE(/* 0x0(sp) */ BlockSoundHandlerPtr handler, /* 0x4(sp) */ SFX2Ptr sfx, /* 0x8(sp) */ SFXGrain2Ptr grain) {
    /* -0x10(sp) */ SInt32 found;
    /* -0xc(sp) */ SInt32 i;
    UNIMPLEMENTED();
}

/* 0000b45c 0000b494 */ SInt32 snd_SFX_GRAIN_TYPE_STOP(/* 0x0(sp) */ BlockSoundHandlerPtr handler, /* 0x4(sp) */ SFX2Ptr sfx, /* 0x8(sp) */ SFXGrain2Ptr grain) {
    UNIMPLEMENTED();
}
/* 0000b494 0000b558 */ SInt32 snd_GetSoundIndexInBlock(/* 0x0(sp) */ SFXBlock2Ptr block, /* 0x4(sp) */ SFX2 *sfx) {
    /* -0x10(sp) */ int i;
    UNIMPLEMENTED();
}

/* 0000b558 0000b73c */ SInt32 snd_SFX_GRAIN_TYPE_RAND_PLAY(/* 0x0(sp) */ BlockSoundHandlerPtr handler, /* 0x4(sp) */ SFX2Ptr sfx, /* 0x8(sp) */ SFXGrain2Ptr grain) {
    /* -0x10(sp) */ SInt32 work32;
    UNIMPLEMENTED();
}

/* 0000b73c 0000b7b8 */ SInt32 snd_SFX_GRAIN_TYPE_RAND_DELAY(/* 0x0(sp) */ BlockSoundHandlerPtr handler, /* 0x4(sp) */ SFX2Ptr sfx, /* 0x8(sp) */ SFXGrain2Ptr grain) {}
/* 0000b7b8 0000b8c8 */ SInt32 snd_SFX_GRAIN_TYPE_RAND_PB(/* 0x0(sp) */ BlockSoundHandlerPtr handler, /* 0x4(sp) */ SFX2Ptr sfx, /* 0x8(sp) */ SFXGrain2Ptr grain) {
    /* -0x10(sp) */ SInt32 work32;
    UNIMPLEMENTED();
}

/* 0000b8c8 0000b9b8 */ SInt32 snd_SFX_GRAIN_TYPE_PB(/* 0x0(sp) */ BlockSoundHandlerPtr handler, /* 0x4(sp) */ SFX2Ptr sfx, /* 0x8(sp) */ SFXGrain2Ptr grain) {
    /* -0x10(sp) */ SInt32 work32;
    UNIMPLEMENTED();
}

/* 0000b9b8 0000ba98 */ SInt32 snd_SFX_GRAIN_TYPE_ADD_PB(/* 0x0(sp) */ BlockSoundHandlerPtr handler, /* 0x4(sp) */ SFX2Ptr sfx, /* 0x8(sp) */ SFXGrain2Ptr grain) {
    /* -0x10(sp) */ SInt32 work32;
    UNIMPLEMENTED();
}

/* 0000ba98 0000bb5c */ SInt32 snd_SFX_GRAIN_TYPE_SET_REGISTER(/* 0x0(sp) */ BlockSoundHandlerPtr handler, /* 0x4(sp) */ SFX2Ptr sfx, /* 0x8(sp) */ SFXGrain2Ptr grain) {
    UNIMPLEMENTED();
}
/* 0000bb5c 0000bc84 */ SInt32 snd_SFX_GRAIN_TYPE_SET_REGISTER_RAND(/* 0x0(sp) */ BlockSoundHandlerPtr handler, /* 0x4(sp) */ SFX2Ptr sfx, /* 0x8(sp) */ SFXGrain2Ptr grain) {
    /* -0x10(sp) */ SInt32 work32;
    UNIMPLEMENTED();
}

/* 0000bc84 0000bdd0 */ SInt32 snd_SFX_GRAIN_TYPE_INC_REGISTER(/* 0x0(sp) */ BlockSoundHandlerPtr handler, /* 0x4(sp) */ SFX2Ptr sfx, /* 0x8(sp) */ SFXGrain2Ptr grain) {
    /* -0x10(sp) */ SInt32 work32;
    UNIMPLEMENTED();
}

/* 0000bdd0 0000bf1c */ SInt32 snd_SFX_GRAIN_TYPE_DEC_REGISTER(/* 0x0(sp) */ BlockSoundHandlerPtr handler, /* 0x4(sp) */ SFX2Ptr sfx, /* 0x8(sp) */ SFXGrain2Ptr grain) {
    /* -0x10(sp) */ SInt32 work32;
    UNIMPLEMENTED();
}

/* 0000bf1c 0000c0e0 */ SInt32 snd_SFX_GRAIN_TYPE_TEST_REGISTER(/* 0x0(sp) */ BlockSoundHandlerPtr handler, /* 0x4(sp) */ SFX2Ptr sfx, /* 0x8(sp) */ SFXGrain2Ptr grain) {
    /* -0x10(sp) */ SInt8 source;
    UNIMPLEMENTED();
}

/* 0000c0e0 0000c118 */ SInt32 snd_SFX_GRAIN_TYPE_MARKER(/* 0x0(sp) */ BlockSoundHandlerPtr handler, /* 0x4(sp) */ SFX2Ptr sfx, /* 0x8(sp) */ SFXGrain2Ptr grain) {
    UNIMPLEMENTED();
}
/* 0000c118 0000c284 */ SInt32 snd_SFX_GRAIN_TYPE_GOTO_MARKER(/* 0x0(sp) */ BlockSoundHandlerPtr handler, /* 0x4(sp) */ SFX2Ptr sfx, /* 0x8(sp) */ SFXGrain2Ptr grain) {
    /* -0x18(sp) */ SInt32 work32;
    /* -0x14(sp) */ SInt32 i;
    /* -0x10(sp) */ SInt32 found;
    UNIMPLEMENTED();
}

/* 0000c284 0000c448 */ SInt32 snd_SFX_GRAIN_TYPE_GOTO_RANDOM_MARKER(/* 0x0(sp) */ BlockSoundHandlerPtr handler, /* 0x4(sp) */ SFX2Ptr sfx, /* 0x8(sp) */ SFXGrain2Ptr grain) {
    /* -0x18(sp) */ SInt32 work32;
    /* -0x14(sp) */ SInt32 i;
    /* -0x10(sp) */ SInt32 found;
    UNIMPLEMENTED();
}

/* 0000c448 0000c4e4 */ SInt32 snd_SFX_GRAIN_TYPE_WAIT_FOR_ALL_VOICES(/* 0x0(sp) */ BlockSoundHandlerPtr handler, /* 0x4(sp) */ SFX2Ptr sfx, /* 0x8(sp) */ SFXGrain2Ptr grain) {}
/* 0000c4e4 0000c664 */ SInt32 snd_SFX_GRAIN_TYPE_PLAY_CYCLE(/* 0x0(sp) */ BlockSoundHandlerPtr handler, /* 0x4(sp) */ SFX2Ptr sfx, /* 0x8(sp) */ SFXGrain2Ptr grain) {
    /* -0x10(sp) */ SInt32 work32;
    UNIMPLEMENTED();
}

/* 0000c664 0000c800 */ SInt32 snd_SFX_GRAIN_TYPE_ADD_REGISTER(/* 0x0(sp) */ BlockSoundHandlerPtr handler, /* 0x4(sp) */ SFX2Ptr sfx, /* 0x8(sp) */ SFXGrain2Ptr grain) {
    /* -0x10(sp) */ SInt32 work32;
    UNIMPLEMENTED();
}

/* 0000c800 0000c870 */ SInt32 snd_SFX_GRAIN_TYPE_KEY_OFF_VOICES(/* 0x0(sp) */ BlockSoundHandlerPtr handler, /* 0x4(sp) */ SFX2Ptr sfx, /* 0x8(sp) */ SFXGrain2Ptr grain) {
    UNIMPLEMENTED();
}
/* 0000c870 0000c8e0 */ SInt32 snd_SFX_GRAIN_TYPE_KILL_VOICES(/* 0x0(sp) */ BlockSoundHandlerPtr handler, /* 0x4(sp) */ SFX2Ptr sfx, /* 0x8(sp) */ SFXGrain2Ptr grain) {
    UNIMPLEMENTED();
}
/* 0000c8e0 0000c934 */ SInt32 snd_SFX_GRAIN_TYPE_ON_STOP_MARKER(/* 0x0(sp) */ BlockSoundHandlerPtr handler, /* 0x4(sp) */ SFX2Ptr sfx, /* 0x8(sp) */ SFXGrain2Ptr grain) {
    UNIMPLEMENTED();
}
/* 0000c934 0000ca64 */ SInt32 snd_SFX_GRAIN_TYPE_COPY_REGISTER(/* 0x0(sp) */ BlockSoundHandlerPtr handler, /* 0x4(sp) */ SFX2Ptr sfx, /* 0x8(sp) */ SFXGrain2Ptr grain) {
    /* -0x10(sp) */ SInt8 work;
    UNIMPLEMENTED();
}

/* 0000ca64 0000cb1c */ SInt32 snd_SFX_GRAIN_TYPE_PLUGIN_MESSAGE(/* 0x0(sp) */ BlockSoundHandlerPtr handler, /* 0x4(sp) */ SFX2Ptr sfx, /* 0x8(sp) */ SFXGrain2Ptr grain) {
    /* -0x10(sp) */ PluginParams *pp;
    UNIMPLEMENTED();
}

/* 0000cb1c 0000cec8 */ SInt32 snd_SFX_GRAIN_TYPE_BRANCH(/* 0x0(sp) */ BlockSoundHandlerPtr handler, /* 0x4(sp) */ SFX2Ptr sfx, /* 0x8(sp) */ SFXGrain2Ptr grain) {
    /* -0x20(sp) */ SInt32 index;
    /* -0x1c(sp) */ SFXBlock2Ptr block;
    /* -0x18(sp) */ SFX2 *new_sfx;
    /* -0x14(sp) */ BlockSoundHandlerPtr weakest;
    /* -0x10(sp) */ PlaySoundParams *psp;
    UNIMPLEMENTED();
}

/* 0000cec8 0000d548 */ SInt32 snd_SFX_GRAIN_TYPE_STARTCHILDSOUND(/* 0x0(sp) */ BlockSoundHandlerPtr handler, /* 0x4(sp) */ SFX2Ptr sfx, /* 0x8(sp) */ SFXGrain2Ptr grain) {
    /* -0x58(sp) */ UInt32 sid;
    /* -0x54(sp) */ SInt32 reg;
    /* -0x50(sp) */ SInt32 vol;
    /* -0x4c(sp) */ SInt32 pan;
    /* -0x48(sp) */ SInt32 index;
    /* -0x44(sp) */ SFXBlock2Ptr block;
    /* -0x40(sp) */ struct SndPlayParams params;
    /* -0x10(sp) */ PlaySoundParams *psp;
    UNIMPLEMENTED();
}

/* 0000d548 0000d740 */ SInt32 snd_SFX_GRAIN_TYPE_STOPCHILDSOUND(/* 0x0(sp) */ BlockSoundHandlerPtr handler, /* 0x4(sp) */ SFX2Ptr sfx, /* 0x8(sp) */ SFXGrain2Ptr grain) {
    /* -0x18(sp) */ SFX2Ptr sts;
    /* -0x14(sp) */ SInt32 index;
    /* -0x10(sp) */ SFXBlock2Ptr block;
    /* -0xc(sp) */ PlaySoundParams *psp;
    UNIMPLEMENTED();
}

/* 0000d740 0000d79c */ SInt32 snd_SFX_UNKNOWN_GRAIN_TYPE(/* 0x0(sp) */ BlockSoundHandlerPtr handler, /* 0x4(sp) */ SFX2Ptr sfx, /* 0x8(sp) */ SFXGrain2Ptr grain) {}
/* 0000d79c 0000da2c */ SInt32 snd_DoGrain(/* 0x0(sp) */ BlockSoundHandlerPtr handler) {
    /* -0x18(sp) */ SFX2Ptr sfx;
    /* -0x14(sp) */ SFXGrain2Ptr grain;
    /* -0x10(sp) */ SInt32 stop_sound;
    /* -0xc(sp) */ SInt32 ret;
    UNIMPLEMENTED();
}

/* 0000da2c 0000e18c */ void snd_SetSFXVolPan(/* 0x0(sp) */ UInt32 handle, /* 0x4(sp) */ SInt32 vol, /* 0x8(sp) */ SInt32 pan, /* 0xc(sp) */ SInt32 cause) {
    /* -0x38(sp) */ BlockSoundHandlerPtr hand;
    /* -0x34(sp) */ SInt32 uses_voice;
    /* -0x30(sp) */ SInt32 own_the_allocator;
    /* -0x2c(sp) */ SFX2Ptr sfx;
    /* -0x28(sp) */ SInt32 new_vol;
    /* -0x24(sp) */ SInt32 new_pan;
    /* -0x20(sp) */ GSoundHandlerPtr child_walk;
    /* -0x1c(sp) */ SInt32 child_vol;
    /* -0x18(sp) */ SInt32 intr_state;
    /* -0x14(sp) */ SInt32 dis;
    /* -0x10(sp) */ SpuVolume spu_vol;
    /* -0xc(sp) */ SInt32 g_pan;
    UNIMPLEMENTED();
}

/* 0000e18c 0000e240 */ void snd_SetSFXPitch(/* 0x0(sp) */ UInt32 handle, /* 0x4(sp) */ SInt32 pitch) {
    /* -0x10(sp) */ BlockSoundHandlerPtr hand;
    UNIMPLEMENTED();
}

/* 0000e240 0000e32c */ void snd_SetSFXPitchbend(/* 0x0(sp) */ UInt32 handle, /* -0x18(sp) */ SInt16 bend) {
    /* -0x14(sp) */ BlockSoundHandlerPtr hand;
    /* -0x10(sp) */ GSoundHandlerPtr child_walk;
    UNIMPLEMENTED();
}

/* 0000e32c 0000e418 */ void snd_SetSFXPitchModifier(/* 0x0(sp) */ UInt32 handle, /* -0x18(sp) */ SInt16 mod) {
    /* -0x14(sp) */ BlockSoundHandlerPtr hand;
    /* -0x10(sp) */ GSoundHandlerPtr child_walk;
}

/* 0000e418 0000e8d8 */ void snd_UpdateSFXPitch(/* 0x0(sp) */ BlockSoundHandlerPtr hand) {
    /* -0x28(sp) */ SInt32 own_the_allocator;
    /* -0x24(sp) */ SInt32 uses_voice;
    /* -0x20(sp) */ SInt32 note;
    /* -0x1c(sp) */ SInt32 fine;
    /* -0x18(sp) */ SInt32 new_pb;
    /* -0x14(sp) */ SInt32 intr_state;
    /* -0x10(sp) */ SInt32 dis;
    /* -0xc(sp) */ SInt32 clock;
    UNIMPLEMENTED();
}

/* 0000e8d8 0000e944 */ SInt32 snd_DoesSFXLoop(/* 0x0(sp) */ SFXBlock2Ptr block, /* 0x4(sp) */ SInt32 sound) {}
/* 0000e944 0000eb8c */ void snd_SFXOwnerProc(/* 0x0(sp) */ SInt32 voice, /* 0x4(sp) */ UInt32 owner, /* 0x8(sp) */ SInt32 flag) {
    /* -0x18(sp) */ BlockSoundHandlerPtr snd;
    /* -0x14(sp) */ SInt32 core;
    /* -0x10(sp) */ SInt32 c_v;
    /* -0xc(sp) */ BlockSoundHandlerPtr parent;
    UNIMPLEMENTED();
}

/* 0000eb8c 0000ed00 */ SInt32 snd_CollapsePan(/* 0x0(sp) */ SInt32 g_pan, /* 0x4(sp) */ SInt32 app_vol, /* 0x8(sp) */ SFX2 *sfx) {
    /* -0x18(sp) */ SInt32 w_vol;
    /* -0x14(sp) */ SInt32 m_vol;
    /* -0x10(sp) */ SInt32 f_vol;
    UNIMPLEMENTED();
}

/* 0000ed00 0000eed0 */ SInt8 snd_ScalePriorityForVolume(/* 0x0(sp) */ SInt32 vol, /* 0x4(sp) */ TonePtr tone) {
    /* -0x18(sp) */ SInt32 p;
    /* -0x14(sp) */ SInt32 depth;
    /* -0x10(sp) */ SInt32 var;
    UNIMPLEMENTED();
}

/* 0000eed0 0000f0b0 */ void snd_ResetAllHandlersForSound(/* 0x0(sp) */ SFX2 *sfx) {
    /* -0x18(sp) */ GSoundHandlerPtr walk;
    /* -0x14(sp) */ BlockSoundHandlerPtr hand;
    /* -0x10(sp) */ int x;
    UNIMPLEMENTED();
}
