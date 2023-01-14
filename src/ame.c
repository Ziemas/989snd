#include "types.h"


/* 00004dc8 0000500c */ UInt32 snd_PlayAMESound(/* 0x0(sp) */ MIDISoundPtr sound, /* 0x4(sp) */ SInt32 vol, /* 0x8(sp) */ SInt32 pan, /* -0x18(sp) */ SInt16 pitch_mod, /* 0x10(sp) */ int bend) {
	/* -0x16(sp) */ SInt16 bend_;
	/* -0x14(sp) */ AMEHandlerPtr stream_handler;
	/* -0x10(sp) */ MultiMIDIBlockHeaderPtr ame_master;
}

/* 0000500c 00005298 */ SInt32 snd_StartAMESegment(/* 0x0(sp) */ AMEHandlerPtr parent, /* 0x4(sp) */ SInt32 segment) {
	/* -0x18(sp) */ GSoundHandlerPtr walk;
	/* -0x14(sp) */ AMEStreamHeaderPtr stream_ptr;
	/* -0x10(sp) */ MultiMIDIBlockHeaderPtr ame_master;
	/* -0xc(sp) */ MIDISoundPtr m_sound;
}

/* 00005298 00005320 */ void snd_StopAMESegment(/* 0x0(sp) */ MIDIHandlerPtr handler) {}
/* 00005320 00005394 */ AMEStreamHeaderPtr snd_FindAMEStream(/* 0x0(sp) */ MIDIHandlerPtr stream1, /* 0x4(sp) */ SInt32 index) {
	/* -0x10(sp) */ MultiMIDIBlockHeaderPtr ame_master;
}

/* 00005394 0000555c */ void snd_SetAMESoundVolumePan(/* 0x0(sp) */ UInt32 handle, /* 0x4(sp) */ SInt32 vol, /* 0x8(sp) */ SInt32 pan) {
	/* -0x10(sp) */ AMEHandlerPtr stream;
	/* -0xc(sp) */ MIDIHandlerPtr walk;
}

/* 0000555c 00005640 */ void snd_SetAMESoundPitchModifier(/* 0x0(sp) */ UInt32 handle, /* -0x18(sp) */ SInt16 mod) {
	/* -0x14(sp) */ AMEHandlerPtr stream;
	/* -0x10(sp) */ MIDIHandlerPtr walk;
}

/* 00005640 00006600 */ SInt8* snd_AMEFunction(/* 0x0(sp) */ MIDIHandlerPtr stream, /* 0x4(sp) */ UInt8 *ame_header) {
	/* -0x38(sp) */ SInt32 compare_value;
	/* -0x34(sp) */ UInt8 *command_ptr;
	/* -0x30(sp) */ SInt16 next_command;
	/* -0x2c(sp) */ SInt32 ignore_next_command;
	/* -0x28(sp) */ AMEStreamHeaderPtr stream_to_stop;
	/* -0x24(sp) */ SInt32 y;
	/* -0x20(sp) */ SInt32 group;
	/* -0x1c(sp) */ SInt32 counter;
	/* -0x18(sp) */ SInt32 done;
	/* -0x14(sp) */ SInt32 stop;
	/* -0x10(sp) */ SInt8 *stream_end;
	/* -0xc(sp) */ AMEHandlerPtr ame_handler;
}

/* 00006600 00006744 */ void snd_SetMIDIRegister(/* 0x0(sp) */ UInt32 handle, /* 0x4(sp) */ SInt32 reg, /* -0x10(sp) */ SInt16 value) {
	/* -0xc(sp) */ AMEHandlerPtr ame;
}

/* 00006744 00006844 */ void snd_SetAllMIDIRegister(/* 0x0(sp) */ UInt32 handle, /* 0x4(sp) */ SInt8 *vals) {
	/* -0x10(sp) */ AMEHandlerPtr ame;
	/* -0xc(sp) */ SInt32 i;
}

/* 00006844 000068f0 */ SInt32 snd_GetMIDIRegister(/* 0x0(sp) */ UInt32 handle, /* 0x4(sp) */ SInt32 reg) {
	/* -0x10(sp) */ AMEHandlerPtr ame;
}
