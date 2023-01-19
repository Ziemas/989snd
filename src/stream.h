#ifndef STREAM_H_
#define STREAM_H_

#include "types.h"

extern SInt32 gNumVAGStreams;
extern struct VAGStreamHandler *gVAGStreamHandler;

void snd_CloseVAGStreaming();

void snd_PauseVAGStream(UInt32 handle);
void snd_ContinueVAGStream(UInt32 handle);
void snd_StopVAGStream(UInt32 handle);

void snd_ProcessVAGStreamTick(VAGStreamHandlerPtr hand);

#endif // STREAM_H_
