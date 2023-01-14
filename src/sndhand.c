#include "types.h"
/* data 1c1c */ GSoundHandlerPtr gActiveSoundListHead = NULL;
/* data 1c20 */ GSoundHandlerPtr gActiveSoundListTail = NULL;
/* bss 9d0 */ struct BlockSoundHandler gBlockSoundHandler[64];
/* bss 52d0 */ struct BasicEffect gBasicEffect[64];
/* bss 68e0 */ struct MIDIHandler gMIDIHandler[32];
/* bss 59d0 */ struct AMEHandler gAMEHandler[4];


/* 0001d778 0001d87c */ void snd_InitHandlers(/* 0x0(sp) */ GSoundHandlerPtr handlers, /* 0x4(sp) */ SInt32 count, /* 0x8(sp) */ UInt32 type, /* 0xc(sp) */ SInt32 size) {
	/* -0x10(sp) */ int x;
}

/* 0001d87c 0001d990 */ void snd_InitSoundHandlers() {
	/* -0x10(sp) */ SInt32 x;
}

/* 0001d990 0001db34 */ GSoundHandlerPtr snd_FindFreeHandler(/* 0x0(sp) */ GSoundHandlerPtr handlers, /* 0x4(sp) */ SInt32 count, /* 0x8(sp) */ UInt32 type, /* 0xc(sp) */ SInt32 size) {
	/* -0x18(sp) */ SInt32 x;
	/* -0x14(sp) */ SInt32 owner_num;
	/* -0x10(sp) */ SInt32 intr_state;
	/* -0xc(sp) */ SInt32 dis;
}

/* 0001db34 0001de08 */ bool snd_CheckInstanceLimit(/* 0x0(sp) */ SFX2 *sfx, /* 0x4(sp) */ SInt32 vol, /* 0x8(sp) */ bool parent, /* 0xc(sp) */ BlockSoundHandlerPtr *weakest_holder) {
	/* -0x18(sp) */ SInt32 inst;
	/* -0x14(sp) */ SInt32 type;
	/* -0x10(sp) */ BlockSoundHandlerPtr weakest;
	/* -0xc(sp) */ BlockSoundHandlerPtr walk;
}

/* 0001de08 0001deec */ BlockSoundHandlerPtr snd_GetFreeBlockSoundHandler(/* 0x0(sp) */ SFX2 *sfx, /* 0x4(sp) */ SInt32 vol, /* 0x8(sp) */ bool parent) {
	/* -0x10(sp) */ BlockSoundHandlerPtr weakest;
}

/* 0001deec 0001df40 */ MIDIHandlerPtr snd_GetFreeMIDIHandler() {}
/* 0001df40 0001df94 */ AMEHandlerPtr snd_GetFreeAMEHandler() {}
/* 0001df94 0001e09c */ void snd_ActivateHandler(/* 0x0(sp) */ GSoundHandlerPtr snd) {
	/* -0x10(sp) */ SInt32 intr_state;
	/* -0xc(sp) */ SInt32 dis;
}

/* 0001e09c 0001e574 */ void snd_DeactivateHandler(/* 0x0(sp) */ GSoundHandlerPtr snd, /* 0x4(sp) */ SInt32 and_child) {
	/* -0x18(sp) */ GSoundHandlerPtr walk;
	/* -0x14(sp) */ SInt32 intr_state;
	/* -0x10(sp) */ SInt32 dis;
}

/* 0001e574 0001e6e8 */ void snd_AttachSoundToHandlersChildList(/* 0x0(sp) */ GSoundHandlerPtr handler, /* 0x4(sp) */ UInt32 id) {
	/* -0x10(sp) */ GSoundHandlerPtr child_handle;
	/* -0xc(sp) */ GSoundHandlerPtr walk;
}

/* 0001e6e8 0001eaec */ GSoundHandlerPtr snd_CheckHandlerStillActive(/* 0x0(sp) */ UInt32 handle) {
	/* -0x10(sp) */ SInt32 handler_index;
	/* -0xc(sp) */ SInt32 handler_type;
}

/* 0001eaec 0001eca0 */ void snd_StopHandler(/* 0x0(sp) */ UInt32 handle, /* 0x4(sp) */ SInt32 and_child, /* 0x8(sp) */ SInt32 silence, /* 0xc(sp) */ bool vlimit_stop) {
	/* -0x18(sp) */ SInt32 handler_index;
	/* -0x14(sp) */ SInt32 handler_type;
	/* -0x10(sp) */ GSoundHandlerPtr snd;
}

/* 0001eca0 0001ed78 */ void snd_PauseHandlerPtr(/* 0x0(sp) */ GSoundHandlerPtr snd, /* 0x4(sp) */ SInt32 and_child) {
	/* -0x10(sp) */ GSoundHandlerPtr walk;
}

/* 0001ed78 0001ee50 */ void snd_ContinueHandlerPtr(/* 0x0(sp) */ GSoundHandlerPtr snd, /* 0x4(sp) */ SInt32 and_child) {
	/* -0x10(sp) */ GSoundHandlerPtr walk;
}

/* 0001ee50 0001ef74 */ void snd_PauseAllSoundsInGroup(/* 0x0(sp) */ UInt32 groups) {
	/* -0x10(sp) */ GSoundHandlerPtr walk;
	/* -0xc(sp) */ SInt32 type;
}

/* 0001ef74 0001f050 */ void snd_StopAllSoundsInGroup(/* 0x0(sp) */ UInt32 groups) {
	/* -0x10(sp) */ GSoundHandlerPtr walk;
}

/* 0001f050 0001f174 */ void snd_ContinueAllSoundsInGroup(/* 0x0(sp) */ UInt32 groups) {
	/* -0x10(sp) */ GSoundHandlerPtr walk;
	/* -0xc(sp) */ SInt32 type;
}

/* 0001f174 0001f2d8 */ void snd_StopAllSounds() {
	/* -0x18(sp) */ GSoundHandlerPtr walk;
	/* -0x14(sp) */ struct VoiceFlags voices;
}

/* 0001f2d8 0001f608 */ void snd_StopHandlerPtr(/* 0x0(sp) */ GSoundHandlerPtr snd, /* 0x4(sp) */ SInt32 and_child, /* 0x8(sp) */ SInt32 silence, /* 0xc(sp) */ bool vlimit_stop) {
	/* -0x18(sp) */ SInt32 do_voice_and_deactivate;
	/* -0x14(sp) */ bool kill_block_sound;
	/* -0x10(sp) */ GSoundHandlerPtr walk;
	/* -0xc(sp) */ MultiMIDIBlockHeaderPtr ame_master;
}

/* 0001f608 0001f6bc */ void snd_StopAllHandlersForSound(/* 0x0(sp) */ SoundPtr snd, /* 0x4(sp) */ SInt32 silence, /* 0x8(sp) */ bool vlimit_stop) {
	/* -0x10(sp) */ GSoundHandlerPtr walk;
}

/* 0001f6bc 0001f7a4 */ bool snd_KillChildrenWithSound(/* 0x0(sp) */ GSoundHandlerPtr handler, /* 0x4(sp) */ void *sfx) {
	/* -0x10(sp) */ GSoundHandlerPtr walk;
}

/* 0001f7a4 0001f8c8 */ SInt32 snd_GetNextHandlerVoice(/* 0x0(sp) */ GSoundHandlerPtr snd, /* 0x4(sp) */ SInt32 start_v) {
	/* -0x18(sp) */ SInt32 x;
	/* -0x14(sp) */ SInt32 core;
	/* -0x10(sp) */ SInt32 c_v;
}

/* 0001f8c8 0001fae0 */ void snd_UpdateHandlers() {
	/* -0x18(sp) */ GSoundHandlerPtr walk;
	/* -0x14(sp) */ SInt32 type;
	/* -0x10(sp) */ SInt32 stop_current_handler;
}

/* 0001fae0 0001fc08 */ SInt32 snd_UpdateEffect(/* 0x0(sp) */ EffectChainPtr effect, /* 0x4(sp) */ GSoundHandlerPtr owner) {}
/* 0001fc08 0001fc6c */ void snd_FreeEffectChain(/* 0x0(sp) */ EffectChainPtr effect) {}
/* 0001fc6c 0001fcd0 */ void snd_RemoveEffect(/* 0x0(sp) */ GSoundHandlerPtr handler, /* 0x4(sp) */ SInt32 type_flag) {
	/* -0x10(sp) */ EffectChainPtr found;
}

/* 0001fcd0 0001fd7c */ EffectChainPtr snd_FindEffect(/* 0x0(sp) */ GSoundHandlerPtr handler, /* 0x4(sp) */ SInt32 type_flag) {
	/* -0x10(sp) */ EffectChainPtr walk;
}

/* 0001fd7c 0001fea0 */ BasicEffectPtr snd_GetFreeBasicEffect() {
	/* -0x10(sp) */ SInt32 x;
}

/* 0001fea0 0001ff58 */ void snd_AddEffectToHandler(/* 0x0(sp) */ GSoundHandlerPtr handler, /* 0x4(sp) */ EffectChainPtr effect) {
	/* -0x10(sp) */ EffectChainPtr walk;
}

/* 0001ff58 000200a8 */ void snd_RemoveEffectFromHandler(/* 0x0(sp) */ GSoundHandlerPtr handler, /* 0x4(sp) */ EffectChainPtr effect) {
	/* -0x18(sp) */ SInt32 intr_state;
	/* -0x14(sp) */ SInt32 res;
	/* -0x10(sp) */ EffectChainPtr walk;
}
