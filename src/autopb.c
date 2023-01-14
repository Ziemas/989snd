#include "types.h"

/* 00007178 0000754c */ void snd_AutoPitchBend(/* 0x0(sp) */ UInt32 handle, /* 0x4(sp) */ SInt32 pb, /* 0x8(sp) */ SInt32 delta_time, /* 0xc(sp) */ SInt32 delta_from) {
	/* -0x20(sp) */ SInt32 wouldhavechanged;
	/* -0x1c(sp) */ SInt32 pitchchange;
	/* -0x18(sp) */ SInt32 current_pitch;
	/* -0x14(sp) */ GSoundHandlerPtr handler;
	/* -0x10(sp) */ BasicEffectPtr effect;
	/* -0xc(sp) */ SInt32 new_effect;
}

/* 0000754c 000076c8 */ SInt32 snd_AutoPitchBendHandler(/* 0x0(sp) */ BasicEffectPtr effect, /* 0x4(sp) */ GSoundHandlerPtr handler) {
	/* -0x10(sp) */ SInt32 newMpitch;
}
