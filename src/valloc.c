#include "types.h"
/* data 1dfc */ SInt32 gLockBlockerSema = -1;
/* data 1e00 */ SInt32 gTickAttentionNeeded = 0;
/* data 1e04 */ SInt32 gVoiceAllocationBlocked = 0;
/* data 1e08 */ SInt32 gVoiceAllocatorInUse = 0;
/* data 1e0c */ UInt32 gVAllocOwnerID = 0;
/* data 1e10 */ SInt32 gChannelMode = 0;
/* data 1e14 */ VoiceRange gVoiceRanges[16];
/* data 1e54 */ struct VoiceAttributes *gPlayingListHead = NULL;
/* data 1e58 */ SInt32 gNoiseOwnerPriority[2] = {-1, -1};
/* data 1e60 */ SInt32 gNoiseOwnerVoice[2] = {-1, -1};
/* data 1e68 */ SInt32 gThreadWaitingForVoiceAllocator = -1;
/* bss 92d0 */ struct VoiceAttributes gChannelStatus[48];
/* bss 20 */ UInt32 gAwaitingKeyOn[2];
/* bss 28 */ UInt32 gAwaitingKeyOff[2];
/* bss 30 */ UInt32 gKeyedOnVoices[2];
/* bss 38 */ UInt32 gKeyedOffVoices[2];
/* bss 18 */ UInt32 gReverbVoices[2];
/* bss 10 */ UInt32 gExternVoices[2];


/* 00033514 00033708 */ void snd_InitVoiceAllocator() {
	/* -0x20(sp) */ SInt32 count;
	/* -0x1c(sp) */ SemaParam sema;
}

/* 00033708 0003374c */ void snd_CleanupVoiceAllocator() {}
/* 0003374c 000338c0 */ void snd_SetMixerMode(/* 0x0(sp) */ SInt32 channel_mode, /* 0x4(sp) */ SInt32 reverb_mode) {
	/* -0x10(sp) */ SInt32 count;
}

/* 000338c0 0003396c */ void snd_SetGroupVoiceRange(/* 0x0(sp) */ SInt32 group, /* 0x4(sp) */ SInt32 min, /* 0x8(sp) */ SInt32 max) {}
/* 0003396c 00033a50 */ SInt32 snd_LockVoiceAllocatorEx(/* 0x0(sp) */ BOOL block, /* 0x4(sp) */ UInt32 ownerID) {
	/* -0x10(sp) */ SInt32 ws_ret;
}

/* 00033a50 00033a88 */ SInt32 snd_IsVoiceAllocatorLocked() {}
/* 00033a88 00033aec */ SInt32 snd_CheckAllocatorCueTick() {}
/* 00033aec 00033b34 */ void snd_UnlockVoiceAllocator() {}
/* 00033b34 00033c9c */ SInt32 snd_ExternVoiceAlloc(/* 0x0(sp) */ SInt32 vol_group, /* 0x4(sp) */ SInt32 priority) {
	/* -0x18(sp) */ SInt32 voice;
	/* -0x14(sp) */ UInt32 core;
	/* -0x10(sp) */ UInt32 c_v;
}

/* 00033c9c 00033d8c */ void snd_ExternVoiceFree(/* 0x0(sp) */ SInt32 voice) {
	/* -0x10(sp) */ UInt32 core;
	/* -0xc(sp) */ UInt32 c_v;
}

/* 00033d8c 00033f60 */ void snd_SetVoiceNoiseOrWave(/* 0x0(sp) */ SInt32 voice, /* 0x4(sp) */ BOOL wave) {
	/* -0x18(sp) */ SInt32 intr_state;
	/* -0x14(sp) */ SInt32 dis;
	/* -0x10(sp) */ UInt32 core;
	/* -0xc(sp) */ UInt32 c_v;
}

/* 00033f60 00034294 */ SInt32 snd_AllocateNoiseVoice(/* 0x0(sp) */ SInt32 vol_group, /* 0x4(sp) */ SInt32 priority) {
	/* -0x18(sp) */ SInt32 range_temp;
	/* -0x14(sp) */ SInt32 voice;
	/* -0x10(sp) */ SInt32 intr_state;
	/* -0xc(sp) */ SInt32 dis;
}

/* 00034294 000348d8 */ SInt32 snd_AllocateVoice(/* 0x0(sp) */ SInt32 vol_group, /* 0x4(sp) */ SInt32 priority) {
	/* -0x18(sp) */ SInt32 count;
	/* -0x14(sp) */ SInt32 start;
	/* -0x10(sp) */ SInt32 end;
	/* -0xc(sp) */ SInt32 lowest_priority;
}

/* 000348d8 00035254 */ void snd_StartVAGVoice(/* 0x0(sp) */ SInt32 voice, /* 0x4(sp) */ BOOL noise) {
	/* -0x30(sp) */ SInt32 note;
	/* -0x2c(sp) */ SInt32 fine;
	/* -0x28(sp) */ UInt32 pitch;
	/* -0x24(sp) */ SInt32 intr_state;
	/* -0x20(sp) */ SInt32 dis;
	/* -0x1c(sp) */ UInt32 flags;
	/* -0x18(sp) */ UInt32 core;
	/* -0x14(sp) */ UInt32 c_v;
	/* -0x10(sp) */ SInt32 clock;
}

/* 00035254 000353c0 */ void snd_StartVoices(/* 0x0(sp) */ VoiceFlags *voices) {
	/* -0x10(sp) */ SInt32 count;
}

/* 000353c0 0003555c */ void snd_StartPendingVoices() {
	/* -0x10(sp) */ SInt32 intr_state;
	/* -0xc(sp) */ SInt32 dis;
}

/* 0003555c 000356a8 */ void snd_StopPendingVoices() {
	/* -0x10(sp) */ SInt32 intr_state;
	/* -0xc(sp) */ SInt32 dis;
}

/* 000356a8 000356f4 */ void snd_GetKeyedOnVoices(/* 0x0(sp) */ VoiceFlags *voices) {}
/* 000356f4 00035740 */ void snd_GetKeyedOffVoices(/* 0x0(sp) */ VoiceFlags *voices) {}
/* 00035740 00035908 */ void snd_VoiceIsDone(/* 0x0(sp) */ SInt32 voice) {}
/* 00035908 000359b8 */ UInt32 snd_GetVoiceAge(/* 0x0(sp) */ SInt32 voice) {}
/* 000359b8 00035a14 */ SInt32 snd_GetVoiceStatus(/* 0x0(sp) */ SInt32 voice) {}
/* 00035a14 00035a70 */ SInt32 snd_GetVoicePriority(/* 0x0(sp) */ SInt32 voice) {}
/* 00035a70 00035cdc */ void snd_KeyOffVoice(/* 0x0(sp) */ SInt32 voice) {
	/* -0x10(sp) */ SInt32 core;
	/* -0xc(sp) */ SInt32 c_v;
}

/* 00035cdc 00035fd0 */ void snd_KeyOffVoicesEx(/* 0x0(sp) */ VoiceFlags *voices, /* 0x4(sp) */ BOOL do_owner_proc) {
	/* -0x18(sp) */ SInt32 count;
	/* -0x14(sp) */ SInt32 core;
	/* -0x10(sp) */ SInt32 c_v;
}

/* 00035fd0 000362b0 */ void snd_SilenceVoicesEx(/* 0x0(sp) */ VoiceFlags *voices, /* 0x4(sp) */ BOOL do_owner_proc) {
	/* -0x20(sp) */ SInt32 count;
	/* -0x1c(sp) */ SInt32 core;
	/* -0x18(sp) */ SInt32 c_v;
	/* -0x14(sp) */ SInt32 intr_state;
	/* -0x10(sp) */ SInt32 dis;
}

/* 000362b0 00036470 */ void snd_PauseVoices(/* 0x0(sp) */ VoiceFlags *voices) {
	/* -0x20(sp) */ SInt32 count;
	/* -0x1c(sp) */ SInt32 core;
	/* -0x18(sp) */ SInt32 c_v;
	/* -0x14(sp) */ SInt32 intr_state;
	/* -0x10(sp) */ SInt32 dis;
}

/* 00036470 00036678 */ void snd_PauseVoicesOwnedWithOwner(/* 0x0(sp) */ GSoundHandlerPtr owner) {
	/* -0x20(sp) */ SInt32 count;
	/* -0x1c(sp) */ SInt32 core;
	/* -0x18(sp) */ SInt32 c_v;
	/* -0x14(sp) */ SInt32 intr_state;
	/* -0x10(sp) */ SInt32 dis;
}

/* 00036678 00036a74 */ void snd_UnPauseVoices(/* 0x0(sp) */ VoiceFlags *voices) {
	/* -0x28(sp) */ SInt32 note;
	/* -0x24(sp) */ SInt32 fine;
	/* -0x20(sp) */ SInt32 count;
	/* -0x1c(sp) */ SInt32 core;
	/* -0x18(sp) */ SInt32 c_v;
	/* -0x14(sp) */ UInt32 pitch;
	/* -0x10(sp) */ SInt32 intr_state;
	/* -0xc(sp) */ SInt32 dis;
}

/* 00036a74 00036eb8 */ void snd_UnPauseVoicesOwnedWithOwner(/* 0x0(sp) */ GSoundHandlerPtr owner) {
	/* -0x28(sp) */ SInt32 note;
	/* -0x24(sp) */ SInt32 fine;
	/* -0x20(sp) */ SInt32 count;
	/* -0x1c(sp) */ SInt32 core;
	/* -0x18(sp) */ SInt32 c_v;
	/* -0x14(sp) */ UInt32 pitch;
	/* -0x10(sp) */ SInt32 intr_state;
	/* -0xc(sp) */ SInt32 dis;
}

/* 00036eb8 00036f0c */ struct VoiceAttributes* snd_GetVoiceStatusPtr(/* 0x0(sp) */ SInt32 voice) {}
/* 00036f0c 0003702c */ UInt16 PS1Note2Pitch(/* -0x18(sp) */ SInt8 center_note, /* -0x17(sp) */ SInt8 center_fine, /* -0x16(sp) */ UInt16 note, /* -0x14(sp) */ SInt16 fine) {
	/* -0x10(sp) */ UInt32 pitch;
	/* -0xc(sp) */ BOOL ps1;
}

/* 0003702c 000371a0 */ void snd_PitchBendTone(/* 0x0(sp) */ TonePtr tone, /* 0x4(sp) */ SInt32 pb, /* 0x8(sp) */ SInt32 pm, /* 0xc(sp) */ SInt32 note, /* 0x10(sp) */ SInt32 fine, /* 0x14(sp) */ SInt32 *new_note, /* 0x18(sp) */ SInt32 *new_fine) {
	/* -0x10(sp) */ SInt32 pitch;
}

/* 000371a0 00037338 */ void snd_MarkVoicePlaying(/* 0x0(sp) */ SInt32 voice) {
	/* -0x10(sp) */ struct VoiceAttributes *va;
	/* -0xc(sp) */ struct VoiceAttributes *walk;
}

/* 00037338 000374fc */ void snd_MarkVoiceFree(/* 0x0(sp) */ SInt32 voice) {
	/* -0x18(sp) */ struct VoiceAttributes *va;
	/* -0x14(sp) */ UInt32 core;
	/* -0x10(sp) */ struct VoiceAttributes *walk;
}

/* 000374fc 00037534 */ void snd_HardFreeVoice(/* 0x0(sp) */ SInt32 voice) {}
/* 00037534 00037684 */ void snd_KeyOnVoiceRaw(/* 0x0(sp) */ int core, /* 0x4(sp) */ int voice, /* 0x8(sp) */ BOOL reverb) {}
/* 00037684 00037760 */ void snd_KeyOffVoiceRaw(/* 0x0(sp) */ int core, /* 0x4(sp) */ int voice) {}
