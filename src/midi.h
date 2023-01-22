#ifndef MIDI_H_
#define MIDI_H_

#include "types.h"

SInt32 snd_ProcessMIDITick(MIDIHandlerPtr stream);

UInt32 snd_PlayMIDISound(MIDISoundPtr sound, SInt32 vol, SInt32 pan, SInt16 pitch_mod, int bend);
void snd_SetMIDISoundVolumePan(UInt32 handle, SInt32 vol, SInt32 pan);

void snd_SetMIDISoundPitchModifier(UInt32 handle, SInt16 mod);

#endif // MIDI_H_
