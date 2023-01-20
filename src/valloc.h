#ifndef VALLOC_H_
#define VALLOC_H_

#include "types.h"

extern UInt32 gVAllocOwnerID;
extern UInt32 gAwaitingKeyOn[2];
extern UInt32 gAwaitingKeyOff[2];
extern SInt32 gVoiceAllocatorInUse;
extern SInt32 gTickAttentionNeeded;

struct VoiceAttributes *gPlayingListHead;

extern struct VoiceAttributes gChannelStatus[48];

extern UInt32 gKeyedOnVoices[2];
extern UInt32 gKeyedOffVoices[2];

enum {
    NUM_VOICE_PER_CORE = 24,
    NUM_VOICES = 48,

    VOICE_PRIORITY_PERM = 127
};

void snd_InitVoiceAllocator();
void snd_CleanupVoiceAllocator();

SInt32 snd_LockVoiceAllocatorEx(bool block, UInt32 ownerID);
void snd_UnlockVoiceAllocator();

SInt32 snd_AllocateVoice(SInt32 vol_group, SInt32 priority);

void snd_PitchBendTone(TonePtr tone, SInt32 pb, SInt32 pm, SInt32 note, SInt32 fine, SInt32 *new_note, SInt32 *new_fine);
UInt16 PS1Note2Pitch(SInt8 center_note, SInt8 center_fine, UInt16 note, SInt16 fine);

void snd_PauseVoicesOwnedWithOwner(GSoundHandlerPtr owner);
void snd_UnPauseVoicesOwnedWithOwner(GSoundHandlerPtr owner);

void snd_VoiceIsDone(SInt32 voice);

void snd_MarkVoicePlaying(SInt32 voice);
void snd_MarkVoiceFree(SInt32 voice);

void snd_StartPendingVoices();
void snd_StopPendingVoices();

void snd_KeyOffVoice(SInt32 voice);
void snd_KeyOffVoicesEx(struct VoiceFlags *voices, bool do_owner_proc);
void snd_SilenceVoicesEx(struct VoiceFlags *voices, bool do_owner_proc);

void snd_KeyOffVoiceRaw(int core, int voice);

#endif // VALLOC_H_
