#ifndef AME_H_
#define AME_H_

#include "types.h"

enum MMIDI_FLAG {
    MMIDI_FLAG_UNK2 = 2,
};

UInt32 snd_PlayAMESound(MIDISoundPtr sound, SInt32 vol, SInt32 pan, SInt16 pitch_mod, int bend);
void snd_SetAMESoundVolumePan(UInt32 handle, SInt32 vol, SInt32 pan);

#endif // AME_H_
