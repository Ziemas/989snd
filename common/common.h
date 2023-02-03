#ifndef COMMON_H_
#define COMMON_H_

enum SL {
    SL_INIT = 0,
    SL_CLOSE = 1,
    SL_LOADBANK = 2,
    SL_LOADBANKBYLOC = 3,
    SL_LOADMMD = 4,
    SL_LOADMMDBYLOC = 5,
    SL_UNLOADBANK_A = 6,
    SL_UNLOADMIDI_A = 7,
    SL_RESOLVEBANKS = 8,
    SL_SETMASTERVOL_A = 9,
    SL_GETMASTERVOL = 10,
    SL_SETPLAYBACKMODE_A = 11,
    SL_GETPLAYBACKMODE = 12,
    SL_SETMIXERMODE_A = 13,
    SL_SETREVERBTYPE = 14,
    SL_SETREVERBDEPTH = 15,
    SL_AUTOVERB = 16,
    SL_PLAYSOUND = 17,
    SL_PLAYSOUND_A = 18,
    SL_PAUSESOUND = 19,
    SL_CONTINUESOUND = 20,
    SL_STOPSOUND = 21,
    SL_PAUSEALLSOUNDS = 22,
    SL_CONTINUEALLSOUNDS = 23,
    SL_STOPALLSOUNDS = 24,
    SL_SOUNDISSTILLPLAYING = 25,
    SL_ISSOUNDALOOPER = 26,
    SL_SETSOUNDVOLPAN_A = 27,
    SL_GETSOUNDORIGPITCH = 28,
    SL_GETSOUNDCURRPITCH = 29,
    SL_SETSOUNDPITCH_A = 30,
    SL_SETSOUNDPITCHBEND_A = 31,
    SL_SETSOUNDPITCHMOD_A = 32,
    SL_SETSOUNDPARAMS = 33,
    SL_AUTOVOL_A = 34,
    SL_AUTOPAN_A = 35,
    SL_AUTOPITCH_A = 36,
    SL_AUTOPITCHBEND_A = 37,
    SL_GETGLOBALEXCITE = 38,
    SL_SETGLOBALEXCITE_A = 39,
    SL_GETMIDIREGISTER = 40,
    SL_SETMIDIREGISTER_A = 41,
    SL_INITVAGSTREAMINGEX = 42,
    SL_PLAYVAGSTREAMBYNAME = 43,
    SL_PLAYVAGSTREAMBYLOC = 44,
    SL_PAUSEVAGSTREAM = 45,
    SL_CONTINUEVAGSTREAM = 46,
    SL_STOPVAGSTREAM = 47,
    SL_GETVAGSTREAMQUEUECOUNT = 48,
    SL_GETVAGSTREAMLOC = 49,
    SL_GETVAGSTREAMTIMEREMAINING = 50,
    SL_GETVAGSTREAMTOTALTIME = 51,
    SL_STOPALLVAGSTREAMS = 52,
    SL_CLOSEVAGSTREAMING = 53,
    SL_STREAMSAFECHECKCDIDLE = 54,
    SL_STREAMSAFECDBREAK = 55,
    SL_STREAMSAFECDREAD = 56,
    SL_ALLOCIOPRAM = 57,
    SL_FREEIOPRAM = 58,
    SL_INITMOVIESOUND = 59,
    SL_CLOSEMOVIESOUND = 60,
    SL_RESETMOVIESOUND = 61,
    SL_STARTMOVIESOUND = 62,
    SL_PAUSEMOVIESOUND = 63,
    SL_GETTRANSSTATUS = 64,
    SL_GETSTICK = 65,
    SL_GETVOICEALLOC = 66,
    SL_LOCKVALLOC = 67,
    SL_EXTERNVALLOC = 68,
    SL_EXTERNVFREE = 69,
    SL_UNLOCKVALLOC = 70,
    SL_SRAMMALLOC = 71,
    SL_SRAMMARKUSED = 72,
    SL_SRAMFREE = 73,
    SL_SRAMGETFREE = 74,
    SL_SRAMMAXFREE = 75,
    SL_EXTERNCALL = 76,
    SL_COMMAND_BATCH = 77,
    SL_SETGROUPVOICERANGE = 78,
    SL_ISVAGSTREAMBUFFERED = 79,
    SL_SETREVERBEX = 80,
    SL_PREALLOCREVERBWORKAREA = 81,
    SL_GETLASTLOADERROR = 82,
    SL_SETPANMODE = 83,
    SL_PLAYSOUNDBYNAME = 84,
    SL_PLAYSOUNDBYNAME_A = 85,
    SL_GETSOUNDUD = 86,
    SL_BANKLOADFROMEE = 87,
    SL_SETMOVIESOUNDVOLPAN = 88,
    SL_BANKLOADFROMIOP = 89,
    SL_UPDATEMOVIEADPCM = 90,
    SL_GETMOVIENAX = 91,
    SL_PLAYSOUNDEX = 92,
    SL_GETSOUNDREG = 93,
    SL_SETSOUNDREG = 94,
    SL_SETALLSOUNDREG = 95,
    SL_SETMASTERVOLUMEDUCKER = 96,
    SL_STOPALLSOUNDSINGROUP = 97,
    SL_SETVAGSTREAMCHANNELRANGE = 98,
    SL_SETVAGSTREAMSUBGROUPVOLPAN = 99,
    SL_SETREVERBMODE = 100,
    SL_CDSTATUS = 101,
    SL_GETSFXGLOBALREG = 102,
    SL_SETSFXGLOBALREG = 103,
    SL_EXTERNCALLWITHDATA = 104,
    SL_STREAMSAFECDSEARCHFILE = 105
};

#endif // COMMON_H_
