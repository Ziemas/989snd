#ifndef LFO_H_
#define LFO_H_

#include "types.h"

enum LFO_TYPE {
    LFO_TYPE_OFF = 0,
    LFO_TYPE_SINE = 1,
    LFO_TYPE_SQUARE = 2,
    LFO_TYPE_TRIANGLE = 3,
    LFO_TYPE_SAW = 4,
    LFO_TYPE_RAND = 5,
};

enum LFO_TARGET {
    LFO_TARGET_VOL = 1,
    LFO_TARGET_PAN = 2,
    LFO_TARGET_PM = 3,
    LFO_TARGET_PB = 4,
    LFO_TARGET_STEP = 5,
    LFO_TARGET_DEPTH = 6,
};

void snd_HandleLFOs();
void snd_RemoveLFO(struct LFOTracker *lfo);
void snd_RemoveLFOsForHandler(BlockSoundHandlerPtr handler);
void snd_InitLFO(struct LFOTracker *tracker);
bool snd_InsertLFO(struct LFOTracker *lfo);
UInt32 snd_DoOneLFO(struct LFOTracker *tracker);
void snd_CalcLFODepth(struct LFOTracker *lfo);

#endif // LFO_H_
