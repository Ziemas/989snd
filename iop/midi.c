#include "midi.h"
#include "989snd.h"
#include "ame.h"
#include "init.h"
#include "intrman.h"
#include "libsd-common.h"
#include "loader.h"
#include "playsnd.h"
#include "sndhand.h"
#include "stdio.h"
#include "types.h"
#include "valloc.h"
#include "vol.h"

/* 0001556c 000157b8 */ UInt32 snd_PlayMIDISound(/* 0x0(sp) */ MIDISoundPtr sound, /* 0x4(sp) */ SInt32 vol, /* 0x8(sp) */ SInt32 pan, /* -0x18(sp) */ SInt16 pitch_mod, /* 0x10(sp) */ int bend) {
    /* -0x16(sp) */ SInt16 _bend;
    /* -0x14(sp) */ MIDIBlockHeaderPtr data_stream;
    /* -0x10(sp) */ MIDIHandlerPtr stream_handler;

    data_stream = snd_GetRealMIDIDataStream(sound);
    if (data_stream == NULL || data_stream->BankPtr == NULL) {
        return 0;
    }

    stream_handler = snd_GetFreeMIDIHandler();
    if (stream_handler == NULL) {
        return -1;
    }

    if (vol == 0x7fffffff) {
        vol = 0x400;
    }

    stream_handler->SH.Current_Vol = (sound->Vol * vol) >> 10;

    if (stream_handler->SH.Current_Vol > 127) {
        stream_handler->SH.Current_Vol = 127;
    }

    if (pan == -1 || pan == -2) {
        stream_handler->SH.Current_Pan = sound->Pan;
    } else {
        stream_handler->SH.Current_Pan = pan;
    }

    stream_handler->SH.Current_PM = pitch_mod;
    stream_handler->Repeats = sound->Repeats;
    stream_handler->SH.Sound = sound;
    stream_handler->SH.Original_Vol = sound->Vol;
    stream_handler->SH.Original_Pan = sound->Pan;
    stream_handler->SH.Effects = NULL;
    stream_handler->SH.flags = 0;
    stream_handler->SH.VolGroup = sound->VolGroup;
    snd_SetupMIDIStreamHandler(stream_handler, data_stream);
    snd_ActivateHandler(&stream_handler->SH);
    return stream_handler->SH.OwnerID;
}

/* 000157b8 00015884 */ void snd_SetupMIDIStreamHandler(/* 0x0(sp) */ MIDIHandlerPtr hand, /* 0x4(sp) */ MIDIBlockHeaderPtr stream) {
    hand->BankPtr = stream->BankPtr;
    hand->DataStart = stream->DataStart;
    hand->PlayPos = stream->DataStart;
    hand->Tempo = stream->Tempo;
    hand->TickError = 0;
    hand->PPQ = stream->PPQ;
    snd_ResetControllers(hand);
    hand->Flags |= 1;
}

/* 00015884 000159b8 */ MIDIBlockHeaderPtr snd_GetRealMIDIDataStream(/* 0x0(sp) */ MIDISoundPtr sound) {
    /* -0x10(sp) */ MIDIBlockHeaderPtr stream = sound->MIDIBlock;

    if (stream == NULL) {
        snd_ShowError(28, 0, 0, 0, 0);
        return NULL;
    }

    if (stream->DataID == MID_ID) {
        return stream;
    }

    if (((MultiMIDIBlockHeaderPtr)stream)->NumMIDIBlocks - 1 < sound->Index) {
        snd_ShowError(29, 0, 0, 0, 0);
        return NULL;
    }

    return ((MultiMIDIBlockHeaderPtr)stream)->BlockPtr[sound->Index];
}

/* 000159b8 00015a08 */ void snd_MuteMIDIChannel(/* 0x0(sp) */ MIDIHandlerPtr stream, /* 0x4(sp) */ SInt32 channel) {
    stream->MuteState |= 1 << channel;
}

/* 00015a08 00015a60 */ void snd_UnmuteMIDIChannel(/* 0x0(sp) */ MIDIHandlerPtr stream, /* 0x4(sp) */ SInt32 channel) {
    stream->MuteState &= ~(1 << channel);
}

/* 00015a60 000167ac */ SInt32 snd_ProcessMIDITick(/* 0x0(sp) */ MIDIHandlerPtr stream) {
    /* data 1398 */ static SInt32 in_function = 0;
    /* -0x18(sp) */ SInt32 used_bytes;
    /* -0x14(sp) */ SInt32 stream_done;
    /* -0x10(sp) */ SoundBankPtr bank;

    if (in_function != 0) {
        return 0;
    }
    in_function = 1;

    if ((stream->Flags & 1) != 0) {
        stream->PPT = 100 * gMicsPerTick / (stream->Tempo / stream->PPQ);
        stream->RunningStatus = 0;
        stream->TickDelta = 100 * ReadVarLen(stream->PlayPos, &used_bytes);
        stream->TickCountdown = (stream->TickDelta / 100 * stream->Tempo / stream->PPQ - 1 + gMicsPerTick) / gMicsPerTick;
        stream->PlayPos += used_bytes;
        stream->Flags &= ~1;
        stream->TickError = 0;
    } else {
        stream->TickCountdown--;
    }
    stream_done = 0;

    while (stream_done == 0 && stream->TickCountdown == 0) {
        if (*stream->PlayPos < 0) {
            stream->RunningStatus = *stream->PlayPos++;
        }

        if (stream->RunningStatus == 0xFF) {
            if (*stream->PlayPos == 0x2f) {
                stream->Repeats--;
                if (stream->Repeats != 0) {
                    stream->PlayPos = stream->DataStart;
                    if (stream->Repeats < 0) {
                        stream->Repeats = 0;
                    }
                } else {
                    stream_done = 1;
                }
            } else if (*stream->PlayPos == 0x51) {
                stream->PlayPos += 2;
                stream->Tempo = (stream->PlayPos[0] << 16) | (stream->PlayPos[1] << 8) | stream->PlayPos[2];
                stream->PPT = 100 * gMicsPerTick / (stream->Tempo / stream->PPQ);
                stream->PlayPos += 3;
            } else {
                snd_ShowError(96, *stream->PlayPos, 0, 0, 0);
                stream_done = 1;
            }
        } else if (stream->RunningStatus == 0xf0) {
            if (*stream->PlayPos == 0x75) {
                stream->PlayPos++;
                stream->PlayPos = snd_AMEFunction(stream, stream->PlayPos);
                if (stream->PlayPos) {
                    stream->PlayPos++;
                } else {
                    stream_done = 1;
                }
            } else {
                while (*(UInt8 *)stream->PlayPos != 0xf7) {
                    stream->PlayPos++;
                }
                stream->PlayPos++;
                snd_ShowError(97, 0, 0, 0, 0);
            }
        } else {
            switch (stream->RunningStatus & 0xF0) {
            case 0x80:
                snd_MIDINoteOff(stream);
                stream->PlayPos += 2;
                break;
            case 0x90:
                if (stream->PlayPos[1] != 0) {
                    if ((stream->MuteState & (1 << (stream->RunningStatus & 0xF))) == 0) {
                        snd_MIDINoteOn(stream);
                    }
                } else {
                    snd_MIDINoteOff(stream);
                }
                stream->PlayPos += 2;
                break;
            case 0xB0:
                switch (*stream->PlayPos) {
                case 0:
                    bank = snd_FindBankByNum(stream->PlayPos[1]);
                    if (bank != NULL) {
                        stream->BankPtr = bank;
                    } else {
                        stream_done = 1;
                        snd_ShowError(104, stream->PlayPos[1], 0, 0, 0);
                    }
                    break;
                case 0x7:
                    stream->Vol[stream->RunningStatus & 0xf] = stream->PlayPos[1];
                    break;
                case 0xA:
                    stream->Pan[stream->RunningStatus & 0xf] = snd_MIDITo360Pan(stream->PlayPos[1]);
                    break;
                case 0x40:
                    if (stream->PlayPos[1] < 64) {
                        snd_ReleaseDamper(stream);
                    } else {
                        stream->DamperState |= 1 << (stream->RunningStatus & 0xf);
                    }
                    break;
                }
                stream->PlayPos += 2;
                break;
            case 0xC0:
                stream->Prog[stream->RunningStatus & 0xf] = stream->PlayPos[0];
                stream->PlayPos += 1;
                break;
            case 0xD0:
                snd_MIDINoteOff(stream);
                stream->PlayPos += 1;
                break;
            case 0xE0:
                snd_PitchBend(stream);
                stream->PlayPos += 2;
                break;
            default:
                snd_ShowError(98, stream->RunningStatus & 0xF0, 0, 0, 0);
                stream_done = 1;
                break;
            }
        }

        if (!stream_done) {
            stream->TickDelta = 100 * ReadVarLen(stream->PlayPos, &used_bytes) + stream->TickError;
            if (stream->TickDelta < 0 || stream->TickDelta < stream->PPT / 2) {
                stream->TickError = stream->TickDelta;
                stream->TickDelta = 0;
            }

            if (stream->TickDelta) {
                stream->TickCountdown = (stream->TickDelta / 100 * stream->Tempo / stream->PPQ - 1 + gMicsPerTick) / gMicsPerTick;
                stream->TickError = stream->TickDelta - stream->PPT * stream->TickCountdown;
            }

            stream->PlayPos += used_bytes;
        }
    }

    in_function = 0;
    return stream_done;
}

/* 000167ac 000168b0 */ UInt32 ReadVarLen(/* 0x0(sp) */ UInt8 *buffer, /* 0x4(sp) */ SInt32 *used_bytes) {
    /* -0x10(sp) */ UInt32 ret_val = *buffer;
    /* -0xc(sp) */ SInt32 used = 1;

    if ((ret_val & 0x80) != 0) {
        ret_val &= 0x7f;
        while ((*buffer & 0x80) != 0) {
            used++;
            buffer++;
            ret_val = (ret_val << 7) + (*buffer & 0x7f);
        }
    }

    *used_bytes = used;
    return ret_val;
}

/* 000168b0 00016fe4 */ void snd_MIDINoteOn(/* 0x0(sp) */ MIDIHandlerPtr stream) {
    /* -0xb0(sp) */ TonePtr tones[32];
    /* -0x30(sp) */ SInt32 num_tones;
    /* -0x2c(sp) */ SInt32 x;
    /* -0x28(sp) */ SInt32 voice;
    /* -0x24(sp) */ SInt32 midi_channel = stream->RunningStatus & 0xF;
    /* -0x20(sp) */ SInt32 program = stream->Prog[midi_channel];
    /* -0x1c(sp) */ SInt8 note = stream->PlayPos[0];
    /* -0x1b(sp) */ SInt8 vol = stream->PlayPos[1];
    /* -0x18(sp) */ SInt32 pan_calc;
    /* -0x14(sp) */ SInt32 core;
    /* -0x10(sp) */ SInt32 c_v;
    num_tones = snd_CollectTones(stream->BankPtr, program, program, tones);

    if (snd_LockVoiceAllocatorEx(1, 42) == 0) {
        return;
    }

    for (x = 0; x < num_tones; x++) {
        voice = snd_AllocateVoice(stream->SH.Sound->VolGroup, tones[x]->Priority);
        if (voice == -1) {
            continue;
        }

        core = voice / 24;
        c_v = voice % 24;

        stream->SH.Voices.core[core] |= 1 << c_v;
        gChannelStatus[voice].Owner = &stream->SH;
        gChannelStatus[voice].OwnerProc = snd_MIDISoundOwnerProc;
        gChannelStatus[voice].Tone = tones[x];
        gChannelStatus[voice].StartNote = note;
        gChannelStatus[voice].StartFine = 0;
        gChannelStatus[voice].Priority = tones[x]->Priority;
        gChannelStatus[voice].VolGroup = stream->SH.Sound->VolGroup;
        gChannelStatus[voice].Volume.left = 0;
        gChannelStatus[voice].Volume.right = 0;

        pan_calc = stream->Pan[midi_channel] + stream->SH.Current_Pan;
        if (pan_calc >= 360) {
            pan_calc -= 360;
        }

        snd_MakeVolumesB(stream->SH.Current_Vol,
                         vol * stream->Vol[midi_channel] / 127,
                         pan_calc,
                         stream->BankPtr->FirstProg[program].Vol,
                         stream->BankPtr->FirstProg[program].Pan,
                         tones[x]->Vol,
                         tones[x]->Pan,
                         &gChannelStatus[voice].Volume);

        gChannelStatus[voice].OwnerData.MIDIData.MidiChannel = midi_channel;
        gChannelStatus[voice].Current_PB = stream->PitchBend[midi_channel];
        gChannelStatus[voice].Current_PM = stream->SH.Current_PM;
        gChannelStatus[voice].OwnerData.MIDIData.KeyOnVelocity = vol;
        gChannelStatus[voice].OwnerData.MIDIData.KeyOnProg = &stream->BankPtr->FirstProg[program];
        gChannelStatus[voice].OwnerData.MIDIData.ShouldBeOff = 0;
        snd_StartVAGVoice(voice, false);
    }

    snd_UnlockVoiceAllocator();
}

/* 00016fe4 00017154 */ void snd_MIDINoteOff(/* 0x0(sp) */ MIDIHandlerPtr stream) {
    /* -0x18(sp) */ struct VoiceAttributes *walk;
    /* -0x14(sp) */ SInt32 midi_channel = stream->RunningStatus & 0xf;
    /* -0x10(sp) */ SInt8 note = stream->PlayPos[0];
    if (snd_LockVoiceAllocatorEx(1, 43) == 0) {
        return;
    }

    for (walk = gPlayingListHead; walk != NULL; walk = walk->playlist) {
        if (walk->Owner != &stream->SH ||
            walk->OwnerData.MIDIData.MidiChannel != midi_channel ||
            walk->StartNote != note) {
            continue;
        }

        if ((stream->DamperState & (1 << midi_channel)) != 0) {
            walk->OwnerData.MIDIData.ShouldBeOff = 1;
        } else {
            snd_KeyOffVoice(walk->voice);
        }
    }

    snd_UnlockVoiceAllocator();
}

/* 00017154 0001728c */ void snd_ReleaseDamper(/* 0x0(sp) */ MIDIHandlerPtr stream) {
    /* -0x10(sp) */ struct VoiceAttributes *walk;
    /* -0xc(sp) */ SInt32 midi_channel = stream->RunningStatus & 0xf;
    stream->DamperState &= ~(1 << midi_channel);

    snd_LockVoiceAllocatorEx(1, 44);
    for (walk = gPlayingListHead; walk != NULL; walk = walk->playlist) {
        if (walk->Owner != &stream->SH ||
            walk->OwnerData.MIDIData.MidiChannel != midi_channel ||
            walk->OwnerData.MIDIData.ShouldBeOff != 1) {
            continue;
        }

        snd_KeyOffVoice(walk->voice);
    }

    snd_UnlockVoiceAllocator();
}

/* 0001728c 0001737c */ void snd_ResetControllers(/* 0x0(sp) */ MIDIHandlerPtr stream) {
    /* -0x10(sp) */ SInt32 x;

    stream->MuteState = 0;
    for (x = 0; x < NUM_MIDI_CHANNELS; x++) {
        stream->Prog[x] = 0;
        stream->Vol[x] = 127;
        stream->Pan[x] = 0;
        stream->PitchBend[x] = 0;
    }
    stream->DamperState = 0;
}

/* 0001737c 000176ac */ void snd_PitchBend(/* 0x0(sp) */ MIDIHandlerPtr stream) {
    /* -0x30(sp) */ SInt32 core;
    /* -0x2c(sp) */ SInt32 c_v;
    /* -0x28(sp) */ SInt32 new_note;
    /* -0x24(sp) */ SInt32 new_fine;
    /* -0x20(sp) */ struct VoiceAttributes *walk;
    /* -0x1c(sp) */ UInt32 hold;
    /* -0x18(sp) */ SInt32 intr_state;
    /* -0x14(sp) */ SInt32 dis;
    /* -0x10(sp) */ SInt32 midi_channel = stream->RunningStatus & 0xf;

    hold = 0xFFFF * ((stream->PlayPos[0] & 0x7f) | ((stream->PlayPos[1] & 0x7f) << 7)) / 0x3fff;
    stream->PitchBend[midi_channel] = hold + 0x8000;

    snd_LockVoiceAllocatorEx(true, 45);
    dis = CpuSuspendIntr(&intr_state);

    for (walk = gPlayingListHead; walk != NULL; walk = walk->playlist) {
        if (walk->Owner != &stream->SH ||
            walk->OwnerData.MIDIData.MidiChannel != midi_channel) {
            continue;
        }

        walk->Current_PB = stream->PitchBend[midi_channel];
        core = walk->voice / 24;
        c_v = walk->voice % 24;

        snd_PitchBendTone(walk->Tone,
                          walk->Current_PB,
                          walk->Current_PM,
                          walk->StartNote,
                          walk->StartFine,
                          &new_note, &new_fine);

        sceSdSetParam(SD_VPARAM_PITCH | SD_VOICE(core, c_v),
                      PS1Note2Pitch(walk->Tone->CenterNote, walk->Tone->CenterFine, new_note, new_fine));
    }

    if (!dis) {
        CpuResumeIntr(intr_state);
    }
    snd_UnlockVoiceAllocator();
}

/* 000176ac 00017718 */ void snd_SetMIDISoundVolumePan(/* 0x0(sp) */ UInt32 handle, /* 0x4(sp) */ SInt32 vol, /* 0x8(sp) */ SInt32 pan) {
    /* -0x10(sp) */ MIDIHandlerPtr stream = snd_CheckHandlerStillActive(handle);
    if (stream != NULL) {
        snd_SetMIDIHandlerVolumePan(stream, vol, pan);
    }
}

/* 00017718 00017b8c */ void snd_SetMIDIHandlerVolumePan(/* 0x0(sp) */ MIDIHandlerPtr stream, /* 0x4(sp) */ SInt32 vol, /* 0x8(sp) */ SInt32 pan) {
    /* -0x28(sp) */ SInt32 pan_calc;
    /* -0x24(sp) */ SpuVolume spu_vol;
    /* -0x20(sp) */ SInt32 core;
    /* -0x1c(sp) */ SInt32 c_v;
    /* -0x18(sp) */ struct VoiceAttributes *walk;
    /* -0x14(sp) */ SInt32 intr_state;
    /* -0x10(sp) */ SInt32 dis;
    snd_LockVoiceAllocatorEx(true, 46);
    if (vol >= 0) {
        if (vol != 0x7fffffff) {
            stream->SH.Current_Vol = (stream->SH.Sound->Vol * vol) >> 10;
        }
    } else {
        stream->SH.Current_Vol = -vol;
    }

    if (stream->SH.Current_Vol > 127) {
        stream->SH.Current_Vol = 127;
    }

    if (pan == -1) {
        stream->SH.Current_Pan = stream->SH.Sound->Pan;
    } else if (pan != -2) {
        stream->SH.Current_Pan = pan;
    }

    for (walk = gPlayingListHead; walk != NULL; walk = walk->playlist) {
        if (walk->Owner != &stream->SH) {
            continue;
        }

        pan_calc = stream->Pan[walk->OwnerData.MIDIData.MidiChannel] + stream->SH.Current_Pan;
        if (pan_calc >= 360) {
            pan_calc -= 360;
        }

        snd_MakeVolumesB(stream->SH.Current_Vol,
                         walk->OwnerData.MIDIData.KeyOnVelocity * stream->Vol[walk->OwnerData.MIDIData.MidiChannel],
                         pan_calc,
                         walk->OwnerData.MIDIData.KeyOnProg->Vol,
                         walk->OwnerData.MIDIData.KeyOnProg->Pan,
                         walk->Tone->Vol,
                         walk->Tone->Pan,
                         &walk->Volume);

        if ((stream->SH.flags & HND_PAUSED) == 0) {
            spu_vol.left = snd_AdjustVolToGroup(walk->Volume.left, walk->VolGroup);
            spu_vol.right = snd_AdjustVolToGroup(walk->Volume.right, walk->VolGroup);
            core = walk->voice / 24;
            c_v = walk->voice % 24;
            dis = CpuSuspendIntr(&intr_state);
            sceSdSetParam(SD_VPARAM_VOLL | SD_VOICE(core, c_v), spu_vol.left);
            sceSdSetParam(SD_VPARAM_VOLR | SD_VOICE(core, c_v), spu_vol.right);
            if (!dis) {
                CpuResumeIntr(intr_state);
            }
        }
    }

    snd_UnlockVoiceAllocator();
}

/* 00017b8c 00017bf8 */ void snd_SetMIDISoundPitchModifier(/* 0x0(sp) */ UInt32 handle, /* -0x10(sp) */ SInt16 mod) {
    /* -0xc(sp) */ MIDIHandlerPtr stream = snd_CheckHandlerStillActive(handle);
    if (stream != NULL) {
        snd_SetMIDIHandlerPitchModifier(stream, mod);
    }
}

/* 00017bf8 00017e50 */ void snd_SetMIDIHandlerPitchModifier(/* 0x0(sp) */ MIDIHandlerPtr stream, /* -0x28(sp) */ SInt16 mod) {
    /* -0x24(sp) */ SInt32 core;
    /* -0x20(sp) */ SInt32 c_v;
    /* -0x1c(sp) */ SInt32 new_note;
    /* -0x18(sp) */ SInt32 new_fine;
    /* -0x14(sp) */ struct VoiceAttributes *walk;
    /* -0x10(sp) */ SInt32 intr_state;
    /* -0xc(sp) */ SInt32 dis;

    snd_LockVoiceAllocatorEx(true, 47);
    stream->SH.Current_PM = mod;
    for (walk = gPlayingListHead; walk != NULL; walk = walk->playlist) {
        if (walk->Owner != &stream->SH) {
            continue;
        }

        walk->Current_PM = mod;

        if ((stream->SH.flags & HND_PAUSED) == 0) {

            snd_PitchBendTone(walk->Tone,
                              walk->Current_PB,
                              walk->Current_PM,
                              walk->StartNote,
                              walk->StartFine,
                              &new_note, &new_fine);

            core = walk->voice / 24;
            c_v = walk->voice % 24;

            dis = CpuSuspendIntr(&intr_state);
            sceSdSetParam(SD_VPARAM_PITCH | SD_VOICE(core, c_v),
                          PS1Note2Pitch(walk->Tone->CenterNote, walk->Tone->CenterFine, new_note, new_fine));
            if (!dis) {
                CpuResumeIntr(intr_state);
            }
        }
    }

    snd_UnlockVoiceAllocator();
}

/* 00017e50 00017f40 */ void snd_MIDISoundOwnerProc(/* 0x0(sp) */ SInt32 voice, /* 0x4(sp) */ UInt32 owner, /* 0x8(sp) */ SInt32 flag) {
    /* -0x18(sp) */ MIDIHandlerPtr snd = (MIDIHandlerPtr)owner;
    /* -0x14(sp) */ SInt32 core = voice / 24;
    /* -0x10(sp) */ SInt32 c_v = voice % 24;
    snd->SH.Voices.core[core] &= ~(1 << c_v);
}
