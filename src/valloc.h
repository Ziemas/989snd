#ifndef VALLOC_H_
#define VALLOC_H_

#include "types.h"

extern UInt32 gVAllocOwnerID;
extern UInt32 gAwaitingKeyOn[2];
extern UInt32 gAwaitingKeyOff[2];

#define VOICE_PRIORITY_PERM 127

void snd_InitVoiceAllocator();
void snd_CleanupVoiceAllocator();

SInt32 snd_AllocateVoice(SInt32 vol_group, SInt32 priority);

void snd_PitchBendTone(TonePtr tone, SInt32 pb, SInt32 pm, SInt32 note, SInt32 fine, SInt32 *new_note, SInt32 *new_fine);
UInt16 PS1Note2Pitch(SInt8 center_note, SInt8 center_fine, UInt16 note, SInt16 fine);

void snd_MarkVoicePlaying(SInt32 voice);
void snd_MarkVoiceFree(SInt32 voice);
void snd_StopPendingVoices();

void snd_KeyOffVoiceRaw(int core, int voice);

#endif // VALLOC_H_
