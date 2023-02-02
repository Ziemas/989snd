#ifndef STREAM_H_
#define STREAM_H_

#include "types.h"

extern SInt32 gNumVAGStreams;
extern struct VAGStreamHandler *gVAGStreamHandler;
extern struct VAGBuffer *VAGStreamDMAList;
extern SInt32 gStartDMASema;

bool snd_InitVAGStreamingEx(SInt32 num_channels, SInt32 buffer_size, UInt32 read_mode, bool enable_streamsafe_from_ee);

UInt32 snd_PlayVAGStreamByNameEx(char *name1, char *name2, UInt32 offset1, UInt32 offset2, SInt32 vol, SInt32 pan, SInt32 vol_group, UInt32 queue, SInt32 sub_group, UInt32 flags);
UInt32 snd_PlayVAGStreamByLocEx(SInt32 loc1, SInt32 loc2, SInt32 offset1, SInt32 offset2, SInt32 vol, SInt32 pan, SInt32 vol_group, UInt32 queue, SInt32 sub_group, UInt32 flags);

SInt32 snd_GetVAGStreamQueueCount(UInt32 handle);
void snd_SetVAGStreamChannelRange(SInt32 vol_group, SInt32 ch_min, SInt32 ch_max);
void snd_CloseVAGStreaming();

void snd_SetVAGStreamSubGroupVolPan(UInt32 handle, SInt32 group, SInt32 vol, SInt32 pan);

void snd_StopAllStreams();
void snd_PauseVAGStream(UInt32 handle);
void snd_ContinueVAGStream(UInt32 handle);
void snd_StopVAGStream(UInt32 handle);

UInt32 snd_GetVAGStreamLoc(UInt32 handle);
UInt32 snd_GetVAGStreamTimeRemaining(UInt32 handle);
UInt32 snd_GetVAGStreamTotalTime(UInt32 handle);
SInt32 snd_IsVAGStreamBuffered(UInt32 handle);

void snd_ProcessVAGStreamTick(VAGStreamHandlerPtr hand);

void snd_SetVAGStreamVolPan(UInt32 handle, SInt32 vol, SInt32 pan);

void snd_SetVagStreamPitchModifier(UInt32 handle, SInt16 mod);

SInt32 snd_StreamSafeCdSync(SInt32 mode);
SInt32 snd_StreamSafeCdRead(UInt32 lbn, UInt32 sectors, void *buf);
SInt32 snd_StreamSafeCdBreak();
SInt32 snd_StreamSafeCdSearchFile(sceCdlFILE *fp, char *name);
SInt32 snd_StreamSafeCdGetError();
SInt32 snd_StreamSafeCheckCDIdle(SInt32 block);

#endif // STREAM_H_
