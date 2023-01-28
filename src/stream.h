#ifndef STREAM_H_
#define STREAM_H_

#include "types.h"

extern SInt32 gNumVAGStreams;
extern struct VAGStreamHandler *gVAGStreamHandler;
extern struct VAGBuffer *VAGStreamDMAList;
extern SInt32 gStartDMASema;

void snd_CloseVAGStreaming();

void snd_PauseVAGStream(UInt32 handle);
void snd_ContinueVAGStream(UInt32 handle);
void snd_StopVAGStream(UInt32 handle);

void snd_ProcessVAGStreamTick(VAGStreamHandlerPtr hand);

void snd_SetVAGStreamVolPan(UInt32 handle, SInt32 vol, SInt32 pan);

void snd_SetVagStreamPitchModifier(UInt32 handle, SInt16 mod);

SInt32 snd_StreamSafeCdSync(SInt32 mode);
SInt32 snd_StreamSafeCdRead(UInt32 lbn, UInt32 sectors, void *buf);
SInt32 snd_StreamSafeCdGetError();

#endif // STREAM_H_
