#include "autovol.h"
#include "types.h"

/* 00008738 00008c00 */ void snd_AutoVol(/* 0x0(sp) */ UInt32 handle, /* 0x4(sp) */ SInt32 vol, /* 0x8(sp) */ SInt32 delta_time, /* 0xc(sp) */ SInt32 delta_from) {
    /* -0x20(sp) */ SInt32 wouldhavechanged;
    /* -0x1c(sp) */ SInt32 volchange;
    /* -0x18(sp) */ SInt32 stopit;
    /* -0x14(sp) */ GSoundHandlerPtr handler;
    /* -0x10(sp) */ BasicEffectPtr effect;
    /* -0xc(sp) */ SInt32 new_effect;
}

/* 00008c00 00008e24 */ SInt32 snd_AutoVolHandler(/* 0x0(sp) */ BasicEffectPtr effect, /* 0x4(sp) */ GSoundHandlerPtr handler) {
    /* -0x10(sp) */ SInt16 newMvol;
    /* -0xe(sp) */ SInt16 stopit;
}
