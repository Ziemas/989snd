#include "types.h"
/* data 1b78 */ SInt32 gGlobalExcite = 0;
/* data 1b7c */ SoundBankPtr gSoloBank[8] = {NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL};
/* data 1b9c */ SInt32 gSoloSound[8] = {
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
};
/* data 1bbc */ SInt32 gNumSoloSounds = 0;

/* 0001a170 0001a408 */ SInt32 snd_GetSoundUserData(/* 0x0(sp) */ SoundBankPtr bank, /* 0x4(sp) */ char *bank_name, /* 0x8(sp) */ int sound_index, /* 0xc(sp) */ char *sound_name, /* 0x10(sp) */ int *destination) {
    /* -0x10(sp) */ SFXBlock2Ptr block;
}

/* 0001a408 0001a5c0 */ UInt32 snd_PlaySoundByNameVolPanPMPB(/* 0x0(sp) */ SoundBankPtr block, /* 0x4(sp) */ char *block_name, /* 0x8(sp) */ char *sound, /* 0xc(sp) */ SInt32 vol, /* 0x10(sp) */ SInt32 pan, /* 0x14(sp) */ int pitch_mod, /* 0x18(sp) */ int bend) {
    /* -0x10(sp) */ SInt16 _pitch_mod;
    /* -0xe(sp) */ SInt16 _bend;
    /* -0xc(sp) */ SInt32 index;
}

/* 0001a5c0 0001a900 */ UInt32 snd_PlaySoundEx(/* 0x0(sp) */ SndPlayParamsPtr params) {
    /* -0x20(sp) */ SoundBankPtr bank;
    /* -0x1c(sp) */ SInt32 index;
    /* -0x18(sp) */ SInt32 vol;
    /* -0x14(sp) */ SInt32 pan;
    /* -0x10(sp) */ SInt16 pitch_mod;
    /* -0xe(sp) */ SInt16 bend;
}

/* 0001a900 0001ab88 */ UInt32 snd_PlaySoundVolPanPMPB(/* 0x0(sp) */ SoundBankPtr bank, /* 0x4(sp) */ SInt32 sound, /* 0x8(sp) */ SInt32 vol, /* 0xc(sp) */ SInt32 pan, /* 0x10(sp) */ int pitch_mod, /* 0x14(sp) */ int bend) {
    /* -0x18(sp) */ SInt16 _pitch_mod;
    /* -0x16(sp) */ SInt16 _bend;
    /* -0x14(sp) */ SoundPtr sound_ptr;
    /* -0x10(sp) */ UInt32 ret;
}

/* 0001ab88 0001ac00 */ void snd_StopSound(/* 0x0(sp) */ UInt32 handle) {
    /* -0x10(sp) */ GSoundHandlerPtr snd_ptr;
}

/* 0001ac00 0001aca0 */ UInt32 snd_SoundIsStillPlaying(/* 0x0(sp) */ UInt32 handle) {}
/* 0001aca0 0001ae5c */ SInt32 snd_IsSoundALooper(/* 0x0(sp) */ SoundBankPtr bank, /* 0x4(sp) */ SInt32 sound) {
    /* -0x10(sp) */ SoundPtr sound_ptr;
}

/* 0001ae5c 0001af74 */ void snd_SetSoundVolPan(/* 0x0(sp) */ UInt32 handle, /* 0x4(sp) */ SInt32 vol, /* 0x8(sp) */ SInt32 pan) {
    /* -0x10(sp) */ SInt32 type;
}

/* 0001af74 0001b138 */ UInt32 snd_SetSoundParams(/* 0x0(sp) */ UInt32 handle, /* 0x4(sp) */ UInt32 mask, /* 0x8(sp) */ SInt32 vol, /* 0xc(sp) */ SInt32 pan, /* 0x10(sp) */ int pitch_mod, /* 0x14(sp) */ int bend) {
    /* -0x10(sp) */ SInt16 _pitch_mod;
    /* -0xe(sp) */ SInt16 _bend;
    /* -0xc(sp) */ SInt32 p;
}

/* 0001b138 0001b2d8 */ SInt32 snd_GetSoundOriginalPitch(/* 0x0(sp) */ SoundBankPtr bank, /* 0x4(sp) */ SInt32 sound) {
    /* -0x10(sp) */ SoundPtr sound_ptr;
}

/* 0001b2d8 0001b3b4 */ SInt32 snd_GetSoundCurrentPitch(/* 0x0(sp) */ UInt32 handle) {
    /* -0x18(sp) */ SInt32 type;
    /* -0x14(sp) */ SInt32 ret_val;
    /* -0x10(sp) */ VAGSoundHandlerPtr hand;
}

/* 0001b3b4 0001b4b4 */ SInt32 snd_GetSoundPitchBend(/* 0x0(sp) */ UInt32 handle) {
    /* -0x18(sp) */ SInt32 type;
    /* -0x14(sp) */ SInt32 ret_val;
    /* -0x10(sp) */ VAGSoundHandlerPtr hand;
}

/* 0001b4b4 0001b564 */ void snd_SetSoundPitch(/* 0x0(sp) */ UInt32 handle, /* 0x4(sp) */ SInt32 pitch) {
    /* -0x10(sp) */ SInt32 type;
}

/* 0001b564 0001b61c */ void snd_SetSoundPitchBend(/* 0x0(sp) */ UInt32 handle, /* -0x10(sp) */ SInt16 bend) {
    /* -0xc(sp) */ SInt32 type;
}

/* 0001b61c 0001b730 */ void snd_SetSoundPitchModifier(/* 0x0(sp) */ UInt32 handle, /* -0x10(sp) */ SInt16 mod) {
    /* -0xc(sp) */ SInt32 type;
}

/* 0001b730 0001b7fc */ SInt8 snd_GetSoundReg(/* 0x0(sp) */ UInt32 handle, /* 0x4(sp) */ SInt32 which) {
    /* -0x10(sp) */ SInt32 type;
    /* -0xc(sp) */ SInt8 val;
}

/* 0001b7fc 0001b8c0 */ void snd_SetSoundReg(/* 0x0(sp) */ UInt32 handle, /* 0x4(sp) */ SInt32 which, /* -0x10(sp) */ SInt8 val) {
    /* -0xc(sp) */ SInt32 type;
}

/* 0001b8c0 0001b96c */ void snd_SetAllSoundReg(/* 0x0(sp) */ UInt32 handle, /* 0x4(sp) */ SInt8 *vals) {
    /* -0x10(sp) */ SInt32 type;
}

/* 0001b96c 0001ba8c */ SInt32 snd_GetSoundPitchModifier(/* 0x0(sp) */ UInt32 handle) {
    /* -0x18(sp) */ SInt32 type;
    /* -0x14(sp) */ SInt32 ret_val;
    /* -0x10(sp) */ VAGSoundHandlerPtr hand;
}

/* 0001ba8c 0001bb78 */ void snd_PauseSound(/* 0x0(sp) */ UInt32 handle) {
    /* -0x10(sp) */ SInt32 type;
    /* -0xc(sp) */ GSoundHandlerPtr snd_ptr;
}

/* 0001bb78 0001bc64 */ void snd_ContinueSound(/* 0x0(sp) */ UInt32 handle) {
    /* -0x10(sp) */ SInt32 type;
    /* -0xc(sp) */ GSoundHandlerPtr snd_ptr;
}

/* 0001bc64 0001bdf0 */ void snd_StopAllSoundsInBank(/* 0x0(sp) */ SoundBankPtr bank, /* 0x4(sp) */ SInt32 silence) {
    /* -0x10(sp) */ SInt32 x;
    /* -0xc(sp) */ SFXBlock2Ptr block;
}

/* 0001bdf0 0001be50 */ void snd_SetGlobalExcite(/* 0x0(sp) */ SInt32 value) {}
/* 0001be50 0001be88 */ SInt32 snd_GetGlobalExcite() {}
/* 0001be88 0001bf88 */ SoundBankPtr snd_FindBankByName(/* 0x0(sp) */ char *name) {
    /* -0x10(sp) */ SFXBlock2Ptr walk;
    /* -0xc(sp) */ UInt32 *comp;
}

/* 0001bf88 0001c300 */ SInt32 snd_FindSoundByName(/* 0x0(sp) */ SFXBlock2Ptr block, /* 0x4(sp) */ char *name, /* 0x8(sp) */ SFXBlock2Ptr *found_block) {
    /* -0x28(sp) */ UInt32 *buffer;
    /* -0x24(sp) */ SInt32 index;
    /* -0x20(sp) */ SFXNamePtr snd_names;
    /* -0x1c(sp) */ SInt32 count;
    /* -0x18(sp) */ SFXBlock2Ptr walk;
}

/* 0001c300 0001c3e0 */ SInt32 snd_CalcSoundNameHash(/* 0x0(sp) */ char *name) {
    /* -0x10(sp) */ SInt32 ret;
}

/* 0001c3e0 0001c5f4 */ SInt32 snd_CollectTones(/* 0x0(sp) */ SoundBankPtr bank, /* 0x4(sp) */ SInt32 prog, /* 0x8(sp) */ SInt32 key, /* 0xc(sp) */ TonePtr *tones) {
    /* -0x18(sp) */ SInt32 x;
    /* -0x14(sp) */ SInt32 tones_in_prog;
    /* -0x10(sp) */ TonePtr tone_list;
    /* -0xc(sp) */ SInt32 tone_count;
}

/* 0001c5f4 0001c720 */ void snd_DEBUG_SoloSound(/* 0x0(sp) */ SoundBankPtr bank, /* 0x4(sp) */ SInt32 sound) {
    /* -0x10(sp) */ SInt32 x;
}

/* 0001c720 0001c868 */ SInt32 snd_DEBUG_CheckSolo(/* 0x0(sp) */ SoundBankPtr bank, /* 0x4(sp) */ SInt32 sound) {
    /* -0x10(sp) */ SInt32 x;
}

/* 0001c868 0001c8e8 */ void snd_DEBUG_ClearSolo() {
    /* -0x10(sp) */ SInt32 x;
}
