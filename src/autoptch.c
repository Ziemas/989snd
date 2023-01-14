#include "types.h"

/* 000076c8 00007ab0 */ void snd_AutoPitch(/* 0x0(sp) */ UInt32 handle, /* 0x4(sp) */ SInt32 pitch, /* 0x8(sp) */ SInt32 delta_time, /* 0xc(sp) */ SInt32 delta_from) {
	/* -0x20(sp) */ SInt32 wouldhavechanged;
	/* -0x1c(sp) */ SInt32 pitchchange;
	/* -0x18(sp) */ SInt32 current_pitch;
	/* -0x14(sp) */ GSoundHandlerPtr handler;
	/* -0x10(sp) */ BasicEffectPtr effect;
	/* -0xc(sp) */ SInt32 new_effect;
}

/* 00007ab0 00007c20 */ SInt32 snd_AutoPitchHandler(/* 0x0(sp) */ BasicEffectPtr effect, /* 0x4(sp) */ GSoundHandlerPtr handler) {
	/* -0x10(sp) */ SInt16 newMpitch;
}
