#ifndef STREAM_H_
#define STREAM_H_

#include "types.h"

extern SInt32 gNumVAGStreams;
extern struct VAGStreamHandler *gVAGStreamHandler;

void snd_CloseVAGStreaming();

#endif // STREAM_H_
