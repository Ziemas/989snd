#include "types.h"

/* 000068f0 00006e6c */ void snd_AutoPan(/* 0x0(sp) */ UInt32 handle, /* 0x4(sp) */ SInt32 pan, /* 0x8(sp) */ SInt32 dir, /* 0xc(sp) */ SInt32 delta_time, /* 0x10(sp) */ SInt32 delta_from) {
	/* -0x20(sp) */ SInt32 wouldhavechanged;
	/* -0x1c(sp) */ SInt32 panchange;
	/* -0x18(sp) */ SInt32 shortest;
	/* -0x14(sp) */ GSoundHandlerPtr handler;
	/* -0x10(sp) */ BasicEffectPtr effect;
	/* -0xc(sp) */ SInt32 new_effect;
}

/* 00006e6c 00007178 */ SInt32 snd_AutoPanHandler(/* 0x0(sp) */ BasicEffectPtr effect, /* 0x4(sp) */ GSoundHandlerPtr handler) {
	/* -0x10(sp) */ SInt16 newMpan;
	/* -0xe(sp) */ SInt16 wrap_dest;
}
