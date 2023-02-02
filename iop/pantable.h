#ifndef PANTABLE_H_
#define PANTABLE_H_

#include "types.h"

extern VolPair *gPanTable;
extern VolPair gPanTable1[180];
extern VolPair gPanTable2[180];

void snd_SetPanTable(VolPair *table);
void snd_SetPanMode(SInt32 mode);

#endif // PANTABLE_H_
