#ifndef _989SND_H_
#define _989SND_H_

#include "types.h"

extern SInt32 gThreadPriority_TICK;
extern SInt32 gFileReadMutex;
extern SInt32 gEEDMADoneSema;

extern struct Extern989MonitorInfo *g989Monitor;

Extern989HandlerPtr snd_FindExternProcHandler(UInt32 id);

void snd_EEDMADone(SInt32 *sema_id_ptr);

void snd_ParseCommandLineArg(char *arg);
void snd_DumpVersionAndInfo();

void snd_ShowError(SInt32 num, UInt32 a1, UInt32 a2, UInt32 a3, UInt32 a4);
SInt32 snd_GetVal(char *st);

void snd_CMD_SL_CLOSE(SInt8 *msg_data);
void snd_CMD_SL_LOADBANK(SInt8 *msg_data);
void snd_CMD_SL_LOADBANKBYLOC(SInt8 *msg_data);
void snd_CMD_SL_LOADMMD(SInt8 *msg_data);
void snd_CMD_SL_LOADMMDBYLOC(SInt8 *msg_data);
void snd_CMD_SL_UNLOADBANK_A(SInt8 *msg_data);
void snd_CMD_SL_UNLOADMIDI_A(SInt8 *msg_data);
void snd_CMD_SL_RESOLVEBANKS(SInt8 *msg_data);
void snd_CMD_SL_SETMASTERVOL_A(SInt8 *msg_data);
void snd_CMD_SL_GETMASTERVOL(SInt8 *msg_data);
void snd_CMD_SL_SETPLAYBACKMODE_A(SInt8 *msg_data);
void snd_CMD_SL_GETPLAYBACKMODE(SInt8 *msg_data);
void snd_CMD_SL_SETMIXERMODE_A(SInt8 *msg_data);
void snd_CMD_SL_SETREVERBTYPE(SInt8 *msg_data);
void snd_CMD_SL_SETREVERBDEPTH(SInt8 *msg_data);
void snd_CMD_SL_AUTOVERB(SInt8 *msg_data);
void snd_CMD_SL_PLAYSOUND(SInt8 *msg_data);
void snd_CMD_SL_PLAYSOUND_A(SInt8 *msg_data);
void snd_CMD_SL_PAUSESOUND(SInt8 *msg_data);
void snd_CMD_SL_CONTINUESOUND(SInt8 *msg_data);
void snd_CMD_SL_STOPSOUND(SInt8 *msg_data);
void snd_CMD_SL_PAUSEALLSOUNDS(SInt8 *msg_data);
void snd_CMD_SL_CONTINUEALLSOUNDS(SInt8 *msg_data);
void snd_CMD_SL_STOPALLSOUNDS(SInt8 *msg_data);
void snd_CMD_SL_SOUNDISSTILLPLAYING(SInt8 *msg_data);
void snd_CMD_SL_ISSOUNDALOOPER(SInt8 *msg_data);
void snd_CMD_SL_SETSOUNDVOLPAN_A(SInt8 *msg_data);
void snd_CMD_SL_GETSOUNDORIGPITCH(SInt8 *msg_data);
void snd_CMD_SL_GETSOUNDCURRPITCH(SInt8 *msg_data);
void snd_CMD_SL_SETSOUNDPITCH_A(SInt8 *msg_data);
void snd_CMD_SL_SETSOUNDPITCHBEND_A(SInt8 *msg_data);
void snd_CMD_SL_SETSOUNDPITCHMOD_A(SInt8 *msg_data);
void snd_CMD_SL_SETSOUNDPARAMS(SInt8 *msg_data);
void snd_CMD_SL_AUTOVOL_A(SInt8 *msg_data);
void snd_CMD_SL_AUTOPAN_A(SInt8 *msg_data);
void snd_CMD_SL_AUTOPITCH_A(SInt8 *msg_data);
void snd_CMD_SL_AUTOPITCHBEND_A(SInt8 *msg_data);
void snd_CMD_SL_GETGLOBALEXCITE(SInt8 *msg_data);
void snd_CMD_SL_SETGLOBALEXCITE_A(SInt8 *msg_data);
void snd_CMD_SL_GETMIDIREGISTER(SInt8 *msg_data);
void snd_CMD_SL_SETMIDIREGISTER_A(SInt8 *msg_data);
void snd_CMD_SL_INITVAGSTREAMINGEX(SInt8 *msg_data);
void snd_CMD_SL_PLAYVAGSTREAMBYNAME(SInt8 *msg_data);
void snd_CMD_SL_PLAYVAGSTREAMBYLOC(SInt8 *msg_data);
void snd_CMD_SL_PAUSEVAGSTREAM(SInt8 *msg_data);
void snd_CMD_SL_CONTINUEVAGSTREAM(SInt8 *msg_data);
void snd_CMD_SL_STOPVAGSTREAM(SInt8 *msg_data);
void snd_CMD_SL_GETVAGSTREAMQUEUECOUNT(SInt8 *msg_data);
void snd_CMD_SL_GETVAGSTREAMLOC(SInt8 *msg_data);
void snd_CMD_SL_GETVAGSTREAMTIMEREMAINING(SInt8 *msg_data);
void snd_CMD_SL_GETVAGSTREAMTOTALTIME(SInt8 *msg_data);
void snd_CMD_SL_STOPALLVAGSTREAMS(SInt8 *msg_data);
void snd_CMD_SL_CLOSEVAGSTREAMING(SInt8 *msg_data);
void snd_CMD_SL_STREAMSAFECHECKCDIDLE(SInt8 *msg_data);
void snd_CMD_SL_STREAMSAFECDBREAK(SInt8 *msg_data);
void snd_CMD_SL_STREAMSAFECDREAD(SInt8 *msg_data);
void snd_CMD_SL_ALLOCIOPRAM(SInt8 *msg_data);
void snd_CMD_SL_FREEIOPRAM(SInt8 *msg_data);
void snd_CMD_SL_INITMOVIESOUND(SInt8 *msg_data);
void snd_CMD_SL_CLOSEMOVIESOUND(SInt8 *msg_data);
void snd_CMD_SL_RESETMOVIESOUND(SInt8 *msg_data);
void snd_CMD_SL_STARTMOVIESOUND(SInt8 *msg_data);
void snd_CMD_SL_PAUSEMOVIESOUND(SInt8 *msg_data);
void snd_CMD_SL_GETTRANSSTATUS(SInt8 *msg_data);
void snd_CMD_SL_GETSTICK(SInt8 *msg_data);
void snd_CMD_SL_GETVOICEALLOC(SInt8 *msg_data);
void snd_CMD_SL_LOCKVALLOC(SInt8 *msg_data);
void snd_CMD_SL_EXTERNVALLOC(SInt8 *msg_data);
void snd_CMD_SL_EXTERNVFREE(SInt8 *msg_data);
void snd_CMD_SL_UNLOCKVALLOC(SInt8 *msg_data);
void snd_CMD_SL_SRAMMALLOC(SInt8 *msg_data);
void snd_CMD_SL_SRAMMARKUSED(SInt8 *msg_data);
void snd_CMD_SL_SRAMFREE(SInt8 *msg_data);
void snd_CMD_SL_SRAMGETFREE(SInt8 *msg_data);
void snd_CMD_SL_SRAMMAXFREE(SInt8 *msg_data);
void snd_CMD_SL_EXTERNCALL(SInt8 *msg_data);
void snd_CMD_SL_COMMAND_BATCH(SInt8 *msg_data);
void snd_CMD_SL_SETGROUPVOICERANGE(SInt8 *msg_data);
void snd_CMD_SL_ISVAGSTREAMBUFFERED(SInt8 *msg_data);
void snd_CMD_SL_SETREVERBEX(SInt8 *msg_data);
void snd_CMD_SL_PREALLOCREVERBWORKAREA(SInt8 *msg_data);
void snd_CMD_SL_GETLASTLOADERROR(SInt8 *msg_data);
void snd_CMD_SL_SETPANMODE(SInt8 *msg_data);
void snd_CMD_SL_PLAYSOUNDBYNAME(SInt8 *msg_data);
void snd_CMD_SL_PLAYSOUNDBYNAME_A(SInt8 *msg_data);
void snd_CMD_SL_GETSOUNDUD(SInt8 *msg_data);
void snd_CMD_SL_BANKLOADFROMEE(SInt8 *msg_data);
void snd_CMD_SL_SETMOVIESOUNDVOLPAN(SInt8 *msg_data);
void snd_CMD_SL_BANKLOADFROMIOP(SInt8 *msg_data);
void snd_CMD_SL_UPDATEMOVIEADPCM(SInt8 *msg_data);
void snd_CMD_SL_GETMOVIENAX(SInt8 *msg_data);
void snd_CMD_SL_PLAYSOUNDEX(SInt8 *msg_data);
void snd_CMD_SL_GETSOUNDREG(SInt8 *msg_data);
void snd_CMD_SL_SETSOUNDREG(SInt8 *msg_data);
void snd_CMD_SL_SETALLSOUNDREG(SInt8 *msg_data);
void snd_CMD_SL_SETMASTERVOLUMEDUCKER(SInt8 *msg_data);
void snd_CMD_SL_STOPALLSOUNDSINGROUP(SInt8 *msg_data);
void snd_CMD_SL_SETVAGSTREAMCHANNELRANGE(SInt8 *msg_data);
void snd_CMD_SL_SETVAGSTREAMSUBGROUPVOLPAN(SInt8 *msg_data);
void snd_CMD_SL_SETREVERBMODE(SInt8 *msg_data);
void snd_CMD_SL_CDSTATUS(SInt8 *msg_data);
void snd_CMD_SL_GETSFXGLOBALREG(SInt8 *msg_data);
void snd_CMD_SL_SETSFXGLOBALREG(SInt8 *msg_data);
void snd_CMD_SL_EXTERNCALLWITHDATA(SInt8 *msg_data);

#endif // 989SND_H_
