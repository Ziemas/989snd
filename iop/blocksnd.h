#ifndef BLOCKSND_H_
#define BLOCKSND_H_

#include "types.h"

#define GRAIN_ARG(x) (grain->OpcodeData.MicroOp.Arg[(x)])

enum BLOCK_FLAG {
    BLOCK_HAS_NAMES = 0x100,
    BLOCK_HAS_USERDATA = 0x200,
};

enum GRAIN_TYPE {
    GRAIN_NULL = 0x0,
    GRAIN_TONE = 0x1,
    GRAIN_TONE2 = 0x9,
    GRAIN_XREF_ID = 0x2,
    GRAIN_XREF_NUM = 0x3,
    GRAIN_LFO_SETTINGS = 0x4,
    GRAIN_STARTCHILDSOUND = 0x5,
    GRAIN_STOPCHILDSOUND = 0x6,
    GRAIN_PLUGIN_MESSAGE = 0x7,
    GRAIN_BRANCH = 0x8,
    GRAIN_CONTROL_NULL = 0x14,
    GRAIN_LOOP_START = 0x15,
    GRAIN_LOOP_END = 0x16,
    GRAIN_LOOP_CONTINUE = 0x17,
    GRAIN_STOP = 0x18,
    GRAIN_RAND_PLAY = 0x19,
    GRAIN_RAND_DELAY = 0x1A,
    GRAIN_RAND_PB = 0x1B,
    GRAIN_PB = 0x1C,
    GRAIN_ADD_PB = 0x1D,
    GRAIN_SET_REGISTER = 0x1E,
    GRAIN_SET_REGISTER_RAND = 0x1F,
    GRAIN_INC_REGISTER = 0x20,
    GRAIN_DEC_REGISTER = 0x21,
    GRAIN_TEST_REGISTER = 0x22,
    GRAIN_MARKER = 0x23,
    GRAIN_GOTO_MARKER = 0x24,
    GRAIN_GOTO_RANDOM_MARKER = 0x25,
    GRAIN_WAIT_FOR_ALL_VOICES = 0x26,
    GRAIN_PLAY_CYCLE = 0x27,
    GRAIN_ADD_REGISTER = 0x28,
    GRAIN_KEY_OFF_VOICES = 0x29,
    GRAIN_KILL_VOICES = 0x2A,
    GRAIN_ON_STOP_MARKER = 0x2B,
    GRAIN_COPY_REGISTER = 0x2C,
};

UInt32 snd_PlaySFX(SFXBlock2Ptr block, SInt32 sound, SInt32 vol, SInt32 pan, int pitch_mod, int bend);
UInt32 snd_PlaySFX_EX(SFXBlock2Ptr block, SInt32 sound, SInt32 vol, SInt32 pan, int pitch_mod, int bend, int sfx_vol, int sfx_pan);
UInt32 snd_PlaySFXWithStruct(SFXBlock2Ptr block, SInt32 sound, SInt16 sfx_vol, SInt16 sfx_pan, SndPlayParamsPtr params);

bool snd_DoBlockSoundStop(BlockSoundHandlerPtr handler, SInt32 silence, bool vlimit_stop);

SInt32 snd_ProcessBlockSoundTick(BlockSoundHandlerPtr handler);
void snd_UpdateSFXPitch(BlockSoundHandlerPtr hand);
SInt32 snd_DoesSFXLoop(SFXBlock2Ptr block, SInt32 sound);
void snd_SFXOwnerProc(SInt32 voice, UInt32 owner, SInt32 flag);
void snd_SetSFXPitch(UInt32 handle, SInt32 pitch);
void snd_SetSFXPitchbend(UInt32 handle, SInt16 bend);
void snd_SetSFXVolPan(UInt32 handle, SInt32 vol, SInt32 pan, SInt32 cause);

void snd_SetSFXPitchModifier(UInt32 handle, SInt16 mod);

SInt8 snd_GetSFXGlobalReg(SInt32 which);
void snd_SetSFXGlobalReg(SInt32 which, SInt8 val);
SInt8 snd_GetSFXSoundReg(UInt32 handle, SInt32 which);
void snd_SetSFXSoundReg(UInt32 handle, SInt32 which, SInt8 val);
void snd_SetAllSFXSoundReg(UInt32 handle, SInt8 *vals);

SInt32 snd_DoGrain(BlockSoundHandlerPtr handler);
SInt32 snd_CollapsePan(SInt32 g_pan, SInt32 app_vol, SFX2 *sfx);
SInt8 snd_ScalePriorityForVolume(SInt32 vol, TonePtr tone);

SInt32 snd_GetOnStopGrainIndex(BlockSoundHandlerPtr handler);

SInt32 snd_SFX_GRAIN_TYPE_NULL(BlockSoundHandlerPtr handler, SFX2Ptr sfx, SFXGrain2Ptr grain);
SInt32 snd_SFX_GRAIN_TYPE_TONE(BlockSoundHandlerPtr handler, SFX2Ptr sfx, SFXGrain2Ptr grain);
SInt32 snd_SFX_GRAIN_TYPE_XREF_ID(BlockSoundHandlerPtr handler, SFX2Ptr sfx, SFXGrain2Ptr grain);
SInt32 snd_SFX_GRAIN_TYPE_XREF_NUM(BlockSoundHandlerPtr handler, SFX2Ptr sfx, SFXGrain2Ptr grain);
SInt32 snd_SFX_GRAIN_TYPE_LFO_SETTINGS(BlockSoundHandlerPtr handler, SFX2Ptr sfx, SFXGrain2Ptr grain);
SInt32 snd_SFX_GRAIN_TYPE_STARTCHILDSOUND(BlockSoundHandlerPtr handler, SFX2Ptr sfx, SFXGrain2Ptr grain);
SInt32 snd_SFX_GRAIN_TYPE_STOPCHILDSOUND(BlockSoundHandlerPtr handler, SFX2Ptr sfx, SFXGrain2Ptr grain);
SInt32 snd_SFX_GRAIN_TYPE_PLUGIN_MESSAGE(BlockSoundHandlerPtr handler, SFX2Ptr sfx, SFXGrain2Ptr grain);
SInt32 snd_SFX_GRAIN_TYPE_BRANCH(BlockSoundHandlerPtr handler, SFX2Ptr sfx, SFXGrain2Ptr grain);
SInt32 snd_SFX_UNKNOWN_GRAIN_TYPE(BlockSoundHandlerPtr handler, SFX2Ptr sfx, SFXGrain2Ptr grain);
SInt32 snd_SFX_GRAIN_TYPE_CONTROL_NULL(BlockSoundHandlerPtr handler, SFX2Ptr sfx, SFXGrain2Ptr grain);
SInt32 snd_SFX_GRAIN_TYPE_LOOP_START(BlockSoundHandlerPtr handler, SFX2Ptr sfx, SFXGrain2Ptr grain);
SInt32 snd_SFX_GRAIN_TYPE_LOOP_END(BlockSoundHandlerPtr handler, SFX2Ptr sfx, SFXGrain2Ptr grain);
SInt32 snd_SFX_GRAIN_TYPE_LOOP_CONTINUE(BlockSoundHandlerPtr handler, SFX2Ptr sfx, SFXGrain2Ptr grain);
SInt32 snd_SFX_GRAIN_TYPE_STOP(BlockSoundHandlerPtr handler, SFX2Ptr sfx, SFXGrain2Ptr grain);
SInt32 snd_SFX_GRAIN_TYPE_RAND_PLAY(BlockSoundHandlerPtr handler, SFX2Ptr sfx, SFXGrain2Ptr grain);
SInt32 snd_SFX_GRAIN_TYPE_RAND_DELAY(BlockSoundHandlerPtr handler, SFX2Ptr sfx, SFXGrain2Ptr grain);
SInt32 snd_SFX_GRAIN_TYPE_RAND_PB(BlockSoundHandlerPtr handler, SFX2Ptr sfx, SFXGrain2Ptr grain);
SInt32 snd_SFX_GRAIN_TYPE_PB(BlockSoundHandlerPtr handler, SFX2Ptr sfx, SFXGrain2Ptr grain);
SInt32 snd_SFX_GRAIN_TYPE_ADD_PB(BlockSoundHandlerPtr handler, SFX2Ptr sfx, SFXGrain2Ptr grain);
SInt32 snd_SFX_GRAIN_TYPE_SET_REGISTER(BlockSoundHandlerPtr handler, SFX2Ptr sfx, SFXGrain2Ptr grain);
SInt32 snd_SFX_GRAIN_TYPE_SET_REGISTER_RAND(BlockSoundHandlerPtr handler, SFX2Ptr sfx, SFXGrain2Ptr grain);
SInt32 snd_SFX_GRAIN_TYPE_INC_REGISTER(BlockSoundHandlerPtr handler, SFX2Ptr sfx, SFXGrain2Ptr grain);
SInt32 snd_SFX_GRAIN_TYPE_DEC_REGISTER(BlockSoundHandlerPtr handler, SFX2Ptr sfx, SFXGrain2Ptr grain);
SInt32 snd_SFX_GRAIN_TYPE_TEST_REGISTER(BlockSoundHandlerPtr handler, SFX2Ptr sfx, SFXGrain2Ptr grain);
SInt32 snd_SFX_GRAIN_TYPE_MARKER(BlockSoundHandlerPtr handler, SFX2Ptr sfx, SFXGrain2Ptr grain);
SInt32 snd_SFX_GRAIN_TYPE_GOTO_MARKER(BlockSoundHandlerPtr handler, SFX2Ptr sfx, SFXGrain2Ptr grain);
SInt32 snd_SFX_GRAIN_TYPE_GOTO_RANDOM_MARKER(BlockSoundHandlerPtr handler, SFX2Ptr sfx, SFXGrain2Ptr grain);
SInt32 snd_SFX_GRAIN_TYPE_WAIT_FOR_ALL_VOICES(BlockSoundHandlerPtr handler, SFX2Ptr sfx, SFXGrain2Ptr grain);
SInt32 snd_SFX_GRAIN_TYPE_PLAY_CYCLE(BlockSoundHandlerPtr handler, SFX2Ptr sfx, SFXGrain2Ptr grain);
SInt32 snd_SFX_GRAIN_TYPE_ADD_REGISTER(BlockSoundHandlerPtr handler, SFX2Ptr sfx, SFXGrain2Ptr grain);
SInt32 snd_SFX_GRAIN_TYPE_KEY_OFF_VOICES(BlockSoundHandlerPtr handler, SFX2Ptr sfx, SFXGrain2Ptr grain);
SInt32 snd_SFX_GRAIN_TYPE_KILL_VOICES(BlockSoundHandlerPtr handler, SFX2Ptr sfx, SFXGrain2Ptr grain);
SInt32 snd_SFX_GRAIN_TYPE_ON_STOP_MARKER(BlockSoundHandlerPtr handler, SFX2Ptr sfx, SFXGrain2Ptr grain);
SInt32 snd_SFX_GRAIN_TYPE_COPY_REGISTER(BlockSoundHandlerPtr handler, SFX2Ptr sfx, SFXGrain2Ptr grain);

#endif // BLOCKSND_H_
