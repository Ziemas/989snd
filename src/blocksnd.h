#ifndef BLOCKSND_H_
#define BLOCKSND_H_

#include "types.h"

SInt32 snd_SFX_GRAIN_TYPE_TONE(BlockSoundHandlerPtr handler, SFX2Ptr sfx, SFXGrain2Ptr grain);
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
