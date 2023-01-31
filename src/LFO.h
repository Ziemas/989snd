#ifndef LFO_H_
#define LFO_H_

#include "types.h"

void snd_HandleLFOs();
void snd_RemoveLFO(struct LFOTracker *lfo);
void snd_RemoveLFOsForHandler(BlockSoundHandlerPtr handler);

#endif // LFO_H_
