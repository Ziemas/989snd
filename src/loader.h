#ifndef LOADER_H_
#define LOADER_H_

/* 00015014 000152f8 */ void* snd_IOPMemAlloc(/* 0x0(sp) */ SInt32 size, /* 0x4(sp) */ SInt32 use, /* 0x8(sp) */ SInt32 *act_size);
/* 000152f8 00015360 */ void snd_IOPMemFree(/* 0x0(sp) */ void *mem);


#endif // LOADER_H_
