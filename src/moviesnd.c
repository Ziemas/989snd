#include "types.h"
/* data 139c */ SInt32 gMovieDMAChannel = -1;
/* data 13a0 */ void *gMovieSoundBuffer = NULL;
/* data 13a4 */ UInt32 gMovieSoundSPUBuffer = 0;
/* data 13a8 */ UInt32 gMovieSPUBufferSize = 0;
/* data 13ac */ SInt32 gMovieSoundBufferSize = 0;
/* data 13b0 */ SInt32 gMovieVol = 0;
/* data 13b4 */ SInt32 gMoviePan = 0;
/* data 13b8 */ SInt32 gMovieVolGroup = 0;
/* data 13bc */ SInt32 gMovieSoundVoice[2] = {-1, -1};
/* data 13c4 */ bool gMovieSoundRunning = false;
/* data 13c8 */ SInt32 gMovieSoundType = 0;
/* data 13cc */ SInt32 gMovieSoundCh = 2;
/* data 13d0 */ UInt32 gMovieSoundSR = 0xbb80;
/* data 13d4 */ UInt32 gMovieSoundPausePos = 0;
/* bss 8 */ SpuVolume gMovieSPUVol;

/* 00017f40 000183d8 */ void* snd_InitMovieSoundEx(/* 0x0(sp) */ SInt32 buff_size, /* 0x4(sp) */ SInt32 spu_buff_size, /* 0x8(sp) */ SInt32 vol, /* 0xc(sp) */ SInt32 pan, /* 0x10(sp) */ SInt32 vol_group, /* 0x14(sp) */ SInt32 type) {
	/* -0x10(sp) */ SInt32 msg;
}

/* 000183d8 00018468 */ void snd_UpdateMovieADPCM(/* 0x0(sp) */ SInt32 data_size, /* 0x4(sp) */ UInt32 offset) {}
/* 00018468 00018548 */ UInt32 snd_GetMovieNAX() {
	/* -0x18(sp) */ SInt32 core;
	/* -0x14(sp) */ SInt32 voice;
	/* -0x10(sp) */ UInt32 addr;
}

/* 00018548 0001863c */ void snd_ClearMovieSoundBuffer() {
	/* -0x18(sp) */ SInt32 i;
	/* -0x14(sp) */ SInt32 len;
	/* -0x10(sp) */ SInt32 *walk;
}

/* 0001863c 00018790 */ void snd_CloseMovieSound() {}
/* 00018790 000189d8 */ void snd_ResetMovieSound() {
	/* -0x18(sp) */ UInt32 core;
	/* -0x14(sp) */ UInt32 c_v;
	/* -0x10(sp) */ SInt32 intr_state;
	/* -0xc(sp) */ SInt32 dis;
}

/* 000189d8 00019258 */ void snd_StartMovieSoundEx(/* 0x0(sp) */ void *buff, /* 0x4(sp) */ SInt32 buff_size, /* 0x8(sp) */ SInt32 offset, /* 0xc(sp) */ UInt32 sr, /* 0x10(sp) */ SInt32 ch) {
	/* -0x20(sp) */ SInt32 vol;
	/* -0x1c(sp) */ SInt32 ret;
	/* -0x18(sp) */ UInt32 core;
	/* -0x14(sp) */ UInt32 c_v;
	/* -0x10(sp) */ SInt32 intr_state;
	/* -0xc(sp) */ SInt32 dis;
}

/* 00019258 00019624 */ void snd_AdjustMovieVolDueToMasterChange(/* 0x0(sp) */ SInt32 which) {
	/* -0x20(sp) */ SInt32 vol;
	/* -0x1c(sp) */ SInt32 intr_state;
	/* -0x18(sp) */ SInt32 dis;
	/* -0x14(sp) */ UInt32 core;
	/* -0x10(sp) */ UInt32 c_v;
}

/* 00019624 00019a18 */ void snd_SetMovieSoundVolPan(/* 0x0(sp) */ SInt32 vol, /* 0x4(sp) */ SInt32 pan) {
	/* -0x18(sp) */ SInt32 intr_state;
	/* -0x14(sp) */ SInt32 dis;
	/* -0x10(sp) */ UInt32 core;
	/* -0xc(sp) */ UInt32 c_v;
}

/* 00019a18 00019a74 */ UInt32 snd_GetMovieBufferPos() {
	/* -0x10(sp) */ UInt32 ret;
}

/* 00019a74 00019c80 */ SInt32 snd_PauseMovieSound() {
	/* -0x20(sp) */ SInt32 ret;
	/* -0x1c(sp) */ UInt32 core;
	/* -0x18(sp) */ UInt32 c_v;
	/* -0x14(sp) */ SInt32 intr_state;
	/* -0x10(sp) */ SInt32 dis;
}
