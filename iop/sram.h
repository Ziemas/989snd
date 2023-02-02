#ifndef SRAM_H_
#define SRAM_H_

#include "types.h"
#define IS_ALIGNED(x, y) (((x) & ((y)-1)) == 0)
#define ALIGN(x, y) ((x) + (y) - ((x) & ((y)-1)))

void snd_SRAMInitMalloc();

UInt32 snd_SRAMMalloc(UInt32 size);
UInt32 snd_SRAMMallocRev(UInt32 size);
void snd_SRAMFree(UInt32 loc, UInt32 size);
sSRAMNodePtr snd_SRAMSearch(sSRAMNodePtr root, UInt32 size);
void snd_SRAMRemoveNode(sSRAMNodePtr node);
void snd_SRAMAddNode(sSRAMNodePtr node);

SInt32 snd_SRAMGetFreeMem();
SInt32 snd_SRAMMaxFreeBlockSize();
UInt32 snd_SRAMMarkUsed(UInt32 loc, UInt32 size);

void snd_SRAMDumpFreeBlocks();

#endif // SRAM_H_
