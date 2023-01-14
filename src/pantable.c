#include "types.h"
/* data 15d4 */ VolPair *gPanTable = NULL;
/* data 15d8 */ VolPair gPanTable1[180];
/* data 18a8 */ VolPair gPanTable2[180];


/* 0001a0a8 0001a0d8 */ void snd_SetPanTable(/* 0x0(sp) */ VolPair *table) {}
/* 0001a0d8 0001a170 */ void snd_SetPanMode(/* 0x0(sp) */ SInt32 mode) {}
