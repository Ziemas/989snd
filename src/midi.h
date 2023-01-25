#ifndef MIDI_H_
#define MIDI_H_

#include "types.h"

enum {
    MID_ID = 0x2044494D,
    MMID_ID = 0,
};

UInt32 snd_PlayMIDISound(MIDISoundPtr sound, SInt32 vol, SInt32 pan, SInt16 pitch_mod, int bend);
void snd_SetupMIDIStreamHandler(MIDIHandlerPtr hand, MIDIBlockHeaderPtr stream);

void snd_ResetControllers(MIDIHandlerPtr stream);

MIDIBlockHeaderPtr snd_GetRealMIDIDataStream(MIDISoundPtr sound);
SInt32 snd_ProcessMIDITick(MIDIHandlerPtr stream);
void snd_SetMIDISoundVolumePan(UInt32 handle, SInt32 vol, SInt32 pan);

void snd_SetMIDISoundPitchModifier(UInt32 handle, SInt16 mod);

#endif // MIDI_H_
