#include "types.h"
/* data 1e6c */ SInt32 gMasterVol[32] = {
    0x400,
    0x400,
    0x400,
    0x400,
    0x400,
    0x400,
    0x400,
    0x400,
    0x400,
    0x400,
    0x400,
    0x400,
    0x400,
    0x400,
    0x400,
    0x400,
    0x400,
    0x400,
    0x400,
    0x400,
    0x400,
    0x400,
    0x400,
    0x400,
    0x400,
    0x400,
    0x400,
    0x400,
    0x400,
    0x400,
    0x400,
    0x400,
};
/* data 1eec */ SInt32 gGroupDuckMult[32] = {
    0x1000,
    0x1000,
    0x1000,
    0x1000,
    0x1000,
    0x1000,
    0x1000,
    0x1000,
    0x1000,
    0x1000,
    0x1000,
    0x1000,
    0x1000,
    0x1000,
    0x1000,
    0x1000,
    0x1000,
    0x1000,
    0x1000,
    0x1000,
    0x1000,
    0x1000,
    0x1000,
    0x1000,
    0x1000,
    0x1000,
    0x1000,
    0x1000,
    0x1000,
    0x1000,
    0x1000,
    0x1000,
};
/* data 1f6c */ SInt16 gStereoOrMono = 0;
/* data 1f70 */ SInt32 gCurrentXAVolGroup = -1;
/* data 1f74 */ SInt32 gCurrentXAVol = 0;
/* data 1f78 */ SInt32 gCurrentXAPan = 0;
/* bss 9d50 */ struct DuckerDef gDuckers[4];

/* 00037760 00037844 */ void snd_InitDuckers()
{
    /* -0x10(sp) */ SInt32 i;
}

/* 00037844 00037a34 */ void snd_SetMasterVolume(/* 0x0(sp) */ SInt32 which, /* 0x4(sp) */ SInt32 vol)
{
    /* -0x10(sp) */ SInt32 intr_state;
    /* -0xc(sp) */ SInt32 dis;
}

/* 00037a34 00037a88 */ SInt32 snd_GetMasterVolume(/* 0x0(sp) */ SInt32 which) {}
/* 00037a88 00037ab8 */ void snd_SetPlaybackMode(/* 0x0(sp) */ SInt32 mode) {}
/* 00037ab8 00037af0 */ SInt32 snd_GetPlaybackMode() {}
/* 00037af0 00038240 */ void snd_MakeVolumes(/* 0x0(sp) */ SInt32 snd_vol, /* 0x4(sp) */ SInt32 snd_pan, /* 0x8(sp) */ SInt32 prog_vol, /* 0xc(sp) */ SInt32 prog_pan, /* 0x10(sp) */ SInt32 tone_vol, /* 0x14(sp) */ SInt32 tone_pan, /* 0x18(sp) */ SpuVolume* spu_vol)
{
    /* -0x18(sp) */ SInt32 voll;
    /* -0x14(sp) */ SInt32 volr;
    /* -0x10(sp) */ SInt32 work_vol;
}

/* 00038240 000389c8 */ void snd_MakeVolumesB(/* 0x0(sp) */ SInt32 bank_vol, /* 0x4(sp) */ SInt32 snd_vol, /* 0x8(sp) */ SInt32 snd_pan, /* 0xc(sp) */ SInt32 prog_vol, /* 0x10(sp) */ SInt32 prog_pan, /* 0x14(sp) */ SInt32 tone_vol, /* 0x18(sp) */ SInt32 tone_pan, /* 0x1c(sp) */ SpuVolume* spu_vol)
{
    /* -0x18(sp) */ SInt32 voll;
    /* -0x14(sp) */ SInt32 volr;
    /* -0x10(sp) */ SInt32 work_vol;
}

/* 000389c8 00039044 */ void snd_CalcStereoBalance3d(/* 0x0(sp) */ SInt32 snd_vol, /* 0x4(sp) */ SInt32 snd_pan, /* 0x8(sp) */ SpuVolume* spu_vol)
{
    /* -0x18(sp) */ SInt32 voll;
    /* -0x14(sp) */ SInt32 volr;
    /* -0x10(sp) */ SInt32 work_vol;
}

/* 00039044 000391cc */ SInt16 snd_AdjustVolToGroup(/* -0x18(sp) */ SInt16 vol, /* 0x4(sp) */ SInt32 group)
{
    /* -0x14(sp) */ SInt32 phase;
    /* -0x10(sp) */ SInt32 new_vol;
    /* -0xc(sp) */ SInt32 m_vol;
}

/* 000391cc 000394fc */ void snd_AdjustAllChannelsMasterVolume(/* 0x0(sp) */ UInt32 flags)
{
    /* -0x20(sp) */ SInt32 count;
    /* -0x1c(sp) */ SInt32 intr_state;
    /* -0x18(sp) */ SInt32 dis;
    /* -0x14(sp) */ UInt32 core;
    /* -0x10(sp) */ UInt32 c_v;
}

/* 000394fc 000395e8 */ SInt16 snd_MIDITo360Pan(/* -0x10(sp) */ SInt8 pan) {}
/* 000395e8 00039e1c */ void snd_CheckDuckers()
{
    /* -0x58(sp) */ SInt32 i;
    /* -0x54(sp) */ SInt32 c;
    /* -0x50(sp) */ UInt32 playing_groups;
    /* -0x4c(sp) */ UInt32 affected_groups;
    /* -0x48(sp) */ UInt32 all_affected_groups;
    /* -0x44(sp) */ SInt32 m_vol_track[15];
}

/* 00039e1c 0003a1a0 */ void snd_SetMasterVolumeDucker(/* 0x0(sp) */ SInt32 which, /* 0x4(sp) */ DuckerDefPtr state)
{
    /* -0x10(sp) */ SInt32 delta;
    /* -0xc(sp) */ SInt32 frames;
}
