#include "types.h"
/* data 13d8 */ static SInt16 r250_index = 0;
/* data 13dc */ static UInt16 r250_buffer[250];
/* data 15d0 */ static UInt32 seed = 1;

/* 00019c80 00019ee8 */ void snd_RandInit(/* -0x18(sp) */ SInt16 seed) {
	/* -0x16(sp) */ SInt16 j;
	/* -0x14(sp) */ SInt16 k;
	/* -0x12(sp) */ UInt16 mask;
	/* -0x10(sp) */ UInt16 msb;
}

/* 00019ee8 0001a008 */ UInt16 snd_RandomUInt16() {
	/* v1 3 */ SInt32 j;
	/* a0 4 */ UInt32 new_rand;
}


/* 0001a008 0001a074 */ static UInt16 myrand() {}
/* 0001a074 0001a0a8 */ static void mysrand(/* -0x10(sp) */ UInt16 newseed) {}
