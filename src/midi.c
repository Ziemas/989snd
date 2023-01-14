#include "types.h"

/* 0001556c 000157b8 */ UInt32 snd_PlayMIDISound(/* 0x0(sp) */ MIDISoundPtr sound, /* 0x4(sp) */ SInt32 vol, /* 0x8(sp) */ SInt32 pan, /* -0x18(sp) */ SInt16 pitch_mod, /* 0x10(sp) */ int bend) {
	/* -0x16(sp) */ SInt16 _bend;
	/* -0x14(sp) */ MIDIBlockHeaderPtr data_stream;
	/* -0x10(sp) */ MIDIHandlerPtr stream_handler;
}

/* 000157b8 00015884 */ void snd_SetupMIDIStreamHandler(/* 0x0(sp) */ MIDIHandlerPtr hand, /* 0x4(sp) */ MIDIBlockHeaderPtr stream) {}
/* 00015884 000159b8 */ MIDIBlockHeaderPtr snd_GetRealMIDIDataStream(/* 0x0(sp) */ MIDISoundPtr sound) {
	/* -0x10(sp) */ MIDIBlockHeaderPtr stream;
}

/* 000159b8 00015a08 */ void snd_MuteMIDIChannel(/* 0x0(sp) */ MIDIHandlerPtr stream, /* 0x4(sp) */ SInt32 channel) {}
/* 00015a08 00015a60 */ void snd_UnmuteMIDIChannel(/* 0x0(sp) */ MIDIHandlerPtr stream, /* 0x4(sp) */ SInt32 channel) {}
/* 00015a60 000167ac */ SInt32 snd_ProcessMIDITick(/* 0x0(sp) */ MIDIHandlerPtr stream) {
	/* data 1398 */ static SInt32 in_function;
	/* -0x18(sp) */ SInt32 used_bytes;
	/* -0x14(sp) */ SInt32 stream_done;
	/* -0x10(sp) */ SoundBankPtr bank;
}

/* 000167ac 000168b0 */ UInt32 ReadVarLen(/* 0x0(sp) */ UInt8 *buffer, /* 0x4(sp) */ SInt32 *used_bytes) {
	/* -0x10(sp) */ UInt32 ret_val;
	/* -0xc(sp) */ SInt32 used;
}

/* 000168b0 00016fe4 */ void snd_MIDINoteOn(/* 0x0(sp) */ MIDIHandlerPtr stream) {
	/* -0xb0(sp) */ TonePtr tones[32];
	/* -0x30(sp) */ SInt32 num_tones;
	/* -0x2c(sp) */ SInt32 x;
	/* -0x28(sp) */ SInt32 voice;
	/* -0x24(sp) */ SInt32 midi_channel;
	/* -0x20(sp) */ SInt32 program;
	/* -0x1c(sp) */ SInt8 note;
	/* -0x1b(sp) */ SInt8 vol;
	/* -0x18(sp) */ SInt32 pan_calc;
	/* -0x14(sp) */ SInt32 core;
	/* -0x10(sp) */ SInt32 c_v;
}

/* 00016fe4 00017154 */ void snd_MIDINoteOff(/* 0x0(sp) */ MIDIHandlerPtr stream) {
	/* -0x18(sp) */ struct VoiceAttributes *walk;
	/* -0x14(sp) */ SInt32 midi_channel;
	/* -0x10(sp) */ SInt8 note;
}

/* 00017154 0001728c */ void snd_ReleaseDamper(/* 0x0(sp) */ MIDIHandlerPtr stream) {
	/* -0x10(sp) */ struct VoiceAttributes *walk;
	/* -0xc(sp) */ SInt32 midi_channel;
}

/* 0001728c 0001737c */ void snd_ResetControllers(/* 0x0(sp) */ MIDIHandlerPtr stream) {
	/* -0x10(sp) */ SInt32 x;
}

/* 0001737c 000176ac */ void snd_PitchBend(/* 0x0(sp) */ MIDIHandlerPtr stream) {
	/* -0x30(sp) */ SInt32 core;
	/* -0x2c(sp) */ SInt32 c_v;
	/* -0x28(sp) */ SInt32 new_note;
	/* -0x24(sp) */ SInt32 new_fine;
	/* -0x20(sp) */ struct VoiceAttributes *walk;
	/* -0x1c(sp) */ UInt32 hold;
	/* -0x18(sp) */ SInt32 intr_state;
	/* -0x14(sp) */ SInt32 dis;
	/* -0x10(sp) */ SInt32 midi_channel;
}

/* 000176ac 00017718 */ void snd_SetMIDISoundVolumePan(/* 0x0(sp) */ UInt32 handle, /* 0x4(sp) */ SInt32 vol, /* 0x8(sp) */ SInt32 pan) {
	/* -0x10(sp) */ MIDIHandlerPtr stream;
}

/* 00017718 00017b8c */ void snd_SetMIDIHandlerVolumePan(/* 0x0(sp) */ MIDIHandlerPtr stream, /* 0x4(sp) */ SInt32 vol, /* 0x8(sp) */ SInt32 pan) {
	/* -0x28(sp) */ SInt32 pan_calc;
	/* -0x24(sp) */ SpuVolume spu_vol;
	/* -0x20(sp) */ SInt32 core;
	/* -0x1c(sp) */ SInt32 c_v;
	/* -0x18(sp) */ struct VoiceAttributes *walk;
	/* -0x14(sp) */ SInt32 intr_state;
	/* -0x10(sp) */ SInt32 dis;
}

/* 00017b8c 00017bf8 */ void snd_SetMIDISoundPitchModifier(/* 0x0(sp) */ UInt32 handle, /* -0x10(sp) */ SInt16 mod) {
	/* -0xc(sp) */ MIDIHandlerPtr stream;
}

/* 00017bf8 00017e50 */ void snd_SetMIDIHandlerPitchModifier(/* 0x0(sp) */ MIDIHandlerPtr stream, /* -0x28(sp) */ SInt16 mod) {
	/* -0x24(sp) */ SInt32 core;
	/* -0x20(sp) */ SInt32 c_v;
	/* -0x1c(sp) */ SInt32 new_note;
	/* -0x18(sp) */ SInt32 new_fine;
	/* -0x14(sp) */ struct VoiceAttributes *walk;
	/* -0x10(sp) */ SInt32 intr_state;
	/* -0xc(sp) */ SInt32 dis;
}

/* 00017e50 00017f40 */ void snd_MIDISoundOwnerProc(/* 0x0(sp) */ SInt32 voice, /* 0x4(sp) */ UInt32 owner, /* 0x8(sp) */ SInt32 flag) {
	/* -0x18(sp) */ MIDIHandlerPtr snd;
	/* -0x14(sp) */ SInt32 core;
	/* -0x10(sp) */ SInt32 c_v;
}
