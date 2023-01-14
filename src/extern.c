#include "types.h"
/* data 12dc */ SInt32 gResLibInited = 0;


/* 0000f0b0 0000f138 */ SInt32 InitResidentModule() {
	/* -0x10(sp) */ SInt32 ret;
}

/* 0000f138 0000f1d4 */ UInt32 snd_PlaySoundByNameVolPanPMPB_E(/* 0x0(sp) */ SoundBankPtr block, /* 0x4(sp) */ char *block_name, /* 0x8(sp) */ char *sound, /* 0xc(sp) */ SInt32 vol, /* 0x10(sp) */ SInt32 pan, /* 0x14(sp) */ int pitch_mod, /* 0x18(sp) */ int bend) {
	/* -0x10(sp) */ SInt16 _pitch_mod;
	/* -0xe(sp) */ SInt16 _bend;
	/* -0xc(sp) */ UInt32 ret;
}

/* 0000f1d4 0000f248 */ SInt32 snd_GetSoundUserData_E(/* 0x0(sp) */ SoundBankPtr bank, /* 0x4(sp) */ char *bank_name, /* 0x8(sp) */ int sound_index, /* 0xc(sp) */ char *sound_name, /* 0x10(sp) */ int *destination) {
	/* -0x10(sp) */ SInt32 ret;
}

/* 0000f248 0000f298 */ void snd_SetMIDIRegister_E(/* 0x0(sp) */ UInt32 handle, /* 0x4(sp) */ SInt32 reg, /* -0x10(sp) */ SInt16 value) {}
/* 0000f298 0000f2f0 */ SInt32 snd_GetMIDIRegister_E(/* 0x0(sp) */ UInt32 handle, /* 0x4(sp) */ SInt32 reg) {
	/* -0x10(sp) */ SInt32 ret;
}

/* 0000f2f0 0000f328 */ void snd_SetGlobalExcite_E(/* 0x0(sp) */ SInt32 value) {}
/* 0000f328 0000f370 */ SInt32 snd_GetGlobalExcite_E() {
	/* -0x10(sp) */ SInt32 ret;
}

/* 0000f370 0000f3f0 */ UInt32 snd_SRAMMalloc_E(/* 0x0(sp) */ UInt32 size) {
	/* -0x18(sp) */ UInt32 ret;
	/* -0x14(sp) */ SInt32 dis;
	/* -0x10(sp) */ SInt32 oldstat;
}

/* 0000f3f0 0000f478 */ UInt32 snd_SRAMMarkUsed_E(/* 0x0(sp) */ UInt32 loc, /* 0x4(sp) */ UInt32 size) {
	/* -0x18(sp) */ UInt32 ret;
	/* -0x14(sp) */ SInt32 dis;
	/* -0x10(sp) */ SInt32 oldstat;
}

/* 0000f478 0000f4e8 */ void snd_SRAMFree_E(/* 0x0(sp) */ UInt32 loc, /* 0x4(sp) */ UInt32 size) {
	/* -0x10(sp) */ SInt32 dis;
	/* -0xc(sp) */ SInt32 oldstat;
}

/* 0000f4e8 0000f564 */ SInt32 snd_GetDopplerPitchMod_E(/* 0x0(sp) */ SInt32 approaching_mph) {
	/* -0x10(sp) */ SInt32 ret;
}

/* 0000f564 0000f59c */ void snd_MIDINoteOn_E(/* 0x0(sp) */ MIDIHandlerPtr stream) {}
/* 0000f59c 0000f5d4 */ void snd_MIDINoteOff_E(/* 0x0(sp) */ MIDIHandlerPtr stream) {}
/* 0000f5d4 0000f60c */ void snd_PitchBend_E(/* 0x0(sp) */ MIDIHandlerPtr stream) {}
/* 0000f60c 0000f668 */ SInt16 snd_MIDITo360Pan_E(/* -0x10(sp) */ SInt8 pan) {
	/* -0xe(sp) */ SInt16 ret;
}

/* 0000f668 0000f6a0 */ void snd_ReleaseDamper_E(/* 0x0(sp) */ MIDIHandlerPtr stream) {}
/* 0000f6a0 0000f6d8 */ void snd_ResetControllers_E(/* 0x0(sp) */ MIDIHandlerPtr stream) {}
/* 0000f6d8 0000f750 */ SInt32 snd_SRAMGetFreeMem_E() {
	/* -0x18(sp) */ SInt32 ret;
	/* -0x14(sp) */ SInt32 dis;
	/* -0x10(sp) */ SInt32 oldstat;
}

/* 0000f750 0000f7c8 */ SInt32 snd_SRAMMaxFreeBlockSize_E() {
	/* -0x18(sp) */ SInt32 ret;
	/* -0x14(sp) */ SInt32 dis;
	/* -0x10(sp) */ SInt32 oldstat;
}
