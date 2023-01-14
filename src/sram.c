#include "types.h"
/* data 1c24 */ sSRAMNodePtr gSRAMFreeListHead = NULL;
/* data 1c28 */ SInt32 gSRAMFreeNodesInUse = 0;
/* bss 80e0 */ sSRAMNode gSRAMFreeNodeStorage[129];


/* 000200a8 0002014c */ void snd_SRAMInitMalloc() {
	/* -0x10(sp) */ SInt32 i;
}

/* 0002014c 00020308 */ UInt32 snd_SRAMMalloc(/* 0x0(sp) */ UInt32 size) {
	/* -0x18(sp) */ sSRAMNodePtr p;
	/* -0x14(sp) */ sSRAMNodePtr candidate;
	/* -0x10(sp) */ UInt32 ret_loc;
}

/* 00020308 00020694 */ UInt32 snd_SRAMMallocRev(/* 0x0(sp) */ UInt32 size) {
	/* -0x20(sp) */ SInt32 i;
	/* -0x1c(sp) */ sSRAMNodePtr candidate;
	/* -0x18(sp) */ sSRAMNodePtr storage;
	/* -0x14(sp) */ UInt32 loc;
	/* -0x10(sp) */ UInt32 end;
}

/* 00020694 00020b14 */ void snd_SRAMFree(/* 0x0(sp) */ UInt32 loc, /* 0x4(sp) */ UInt32 size) {
	/* -0x18(sp) */ SInt32 i;
	/* -0x14(sp) */ SInt32 free_node;
	/* -0x10(sp) */ sSRAMNodePtr storage;
}

/* 00020b14 000212b4 */ UInt32 snd_SRAMMarkUsed(/* 0x0(sp) */ UInt32 loc, /* 0x4(sp) */ UInt32 size) {
	/* -0x20(sp) */ SInt32 i;
	/* -0x1c(sp) */ SInt32 nn_size;
	/* -0x18(sp) */ SInt32 nn_loc;
	/* -0x14(sp) */ SInt32 free_node;
	/* -0x10(sp) */ sSRAMNodePtr storage;
}

/* 000212b4 00021420 */ void snd_SRAMRemoveNode(/* 0x0(sp) */ sSRAMNodePtr node) {}
/* 00021420 000215a8 */ void snd_SRAMAddNode(/* 0x0(sp) */ sSRAMNodePtr node) {
	/* -0x10(sp) */ sSRAMNodePtr work;
}

/* 000215a8 00021650 */ sSRAMNodePtr snd_SRAMSearch(/* 0x0(sp) */ sSRAMNodePtr root, /* 0x4(sp) */ UInt32 size) {}
/* 00021650 00021738 */ SInt32 snd_SRAMGetFreeMem() {
	/* -0x18(sp) */ SInt32 i;
	/* -0x14(sp) */ sSRAMNodePtr storage;
	/* -0x10(sp) */ SInt32 free_size;
}

/* 00021738 00021854 */ SInt32 snd_SRAMMaxFreeBlockSize() {
	/* -0x18(sp) */ SInt32 i;
	/* -0x14(sp) */ sSRAMNodePtr storage;
	/* -0x10(sp) */ SInt32 largest;
}

/* 00021854 0002197c */ void snd_SRAMDumpFreeBlocks() {
	/* -0x10(sp) */ SInt32 i;
	/* -0xc(sp) */ sSRAMNodePtr storage;
}

/* 0002197c 00021a9c */ SInt32 snd_GetFreeBlockSize(/* 0x0(sp) */ SInt32 start, /* 0x4(sp) */ UInt32 *loc) {
	/* -0x10(sp) */ SInt32 i;
	/* -0xc(sp) */ sSRAMNodePtr storage;
}
