#include "ame.h"
#include "989snd.h"
#include "midi.h"
#include "playsnd.h"
#include "sndhand.h"
#include "stick.h"
#include "types.h"
#include "util.h"
#include "valloc.h"

/* 00004dc8 0000500c */ UInt32 snd_PlayAMESound(/* 0x0(sp) */ MIDISoundPtr sound, /* 0x4(sp) */ SInt32 vol, /* 0x8(sp) */ SInt32 pan, /* -0x18(sp) */ SInt16 pitch_mod, /* 0x10(sp) */ int bend) {
    ///* -0x16(sp) */ SInt16 bend_;
    /* -0x14(sp) */ AMEHandlerPtr stream_handler;
    /* -0x10(sp) */ MultiMIDIBlockHeaderPtr ame_master;

    ame_master = sound->MIDIBlock;
    if (ame_master == NULL) {
        snd_ShowError(89, 0, 0, 0, 0);
        return 0;
    }

    if ((ame_master->Flags & 2) != 0) {
        return 0;
    }

    stream_handler = snd_GetFreeAMEHandler();
    if (stream_handler == NULL) {
        return 0;
    }

    ame_master->Flags |= HND_PAUSED;

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

    stream_handler->SH.Current_PM = vol;
    stream_handler->SH.Sound = sound;
    stream_handler->SH.Effects = NULL;
    stream_handler->SH.VolGroup = sound->VolGroup;
    snd_StartAMESegment(stream_handler, 0);
    snd_ActivateHandler(&stream_handler->SH);
    return stream_handler->SH.OwnerID;
}

/* 0000500c 00005298 */ SInt32 snd_StartAMESegment(/* 0x0(sp) */ AMEHandlerPtr parent, /* 0x4(sp) */ SInt32 segment) {
    /* -0x18(sp) */ GSoundHandlerPtr walk;
    /* -0x14(sp) */ AMEStreamHeaderPtr stream_ptr;
    /* -0x10(sp) */ MultiMIDIBlockHeaderPtr ame_master;
    /* -0xc(sp) */ MIDISoundPtr m_sound = parent->SH.Sound;
    ame_master = m_sound->MIDIBlock;
    stream_ptr = ame_master->BlockPtr[segment];

    if ((stream_ptr->MH.SH.OwnerID & 0x80000000) != 0) {
        return 0;
    }

    if (stream_ptr->MB.BankPtr == NULL) {
        return 0;
    }

    stream_ptr->MH.SH.first_child = NULL;
    stream_ptr->MH.SH.siblings = NULL;

    if (parent->SH.first_child != NULL) {
        walk = parent->SH.first_child;
        while (walk->siblings != NULL) {
            walk = walk->siblings;
        }

        walk->siblings = &stream_ptr->MH.SH;
    } else {
        parent->SH.first_child = &stream_ptr->MH.SH;
    }

    stream_ptr->MH.SH.parent = &parent->SH;
    stream_ptr->MH.SH.Current_Vol = parent->SH.Current_Vol;
    stream_ptr->MH.SH.Current_Pan = parent->SH.Current_Pan;
    stream_ptr->MH.SH.Current_PM = parent->SH.Current_PM;
    stream_ptr->MH.SH.flags = parent->SH.flags;
    stream_ptr->MH.SH.VolGroup = parent->SH.VolGroup;
    stream_ptr->MH.SH.Sound = parent->SH.Sound;
    stream_ptr->MH.SH.Voices.core[1] = 0;
    stream_ptr->MH.SH.Voices.core[0] = 0;
    stream_ptr->MH.Repeats = 0;

    snd_SetupMIDIStreamHandler(&stream_ptr->MH, &stream_ptr->MB);
    snd_ActivateHandler(&stream_ptr->MH.SH);

    return 1;
}

/* 00005298 00005320 */ void snd_StopAMESegment(/* 0x0(sp) */ MIDIHandlerPtr handler) {
    if ((handler->SH.OwnerID & 0x80000000) != 0) {
        snd_StopHandlerPtr(&handler->SH, 0, 0, 0);
        handler->Flags &= ~1;
    }
}

/* 00005320 00005394 */ AMEStreamHeaderPtr snd_FindAMEStream(/* 0x0(sp) */ MIDIHandlerPtr stream1, /* 0x4(sp) */ SInt32 index) {
    /* -0x10(sp) */ MultiMIDIBlockHeaderPtr ame_master;
    ame_master = ((MIDISoundPtr)stream1->SH.Sound)->MIDIBlock;
    return (AMEStreamHeaderPtr)ame_master->BlockPtr[index];
}

/* 00005394 0000555c */ void snd_SetAMESoundVolumePan(/* 0x0(sp) */ UInt32 handle, /* 0x4(sp) */ SInt32 vol, /* 0x8(sp) */ SInt32 pan) {
    /* -0x10(sp) */ AMEHandlerPtr stream;
    /* -0xc(sp) */ MIDIHandlerPtr walk;
    snd_LockVoiceAllocatorEx(1, 10);
    stream = (AMEHandlerPtr)snd_CheckHandlerStillActive(handle);
    if (stream == NULL) {
        snd_UnlockVoiceAllocator();
        return;
    }

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

    snd_UnlockVoiceAllocator();
    for (walk = stream->SH.first_child; walk != NULL; walk = walk->SH.siblings) {
        snd_SetMIDIHandlerVolumePan(walk, vol, pan);
    }
}

/* 0000555c 00005640 */ void snd_SetAMESoundPitchModifier(/* 0x0(sp) */ UInt32 handle, /* -0x18(sp) */ SInt16 mod) {
    /* -0x14(sp) */ AMEHandlerPtr stream;
    /* -0x10(sp) */ MIDIHandlerPtr walk;
    snd_LockVoiceAllocatorEx(1, 11);
    stream = (AMEHandlerPtr)snd_CheckHandlerStillActive(handle);
    if (stream == NULL) {
        snd_UnlockVoiceAllocator();
        return;
    }

    stream->SH.Current_PM = mod;

    snd_UnlockVoiceAllocator();

    for (walk = stream->SH.first_child; walk != NULL; walk = walk->SH.siblings) {
        snd_SetMIDIHandlerPitchModifier(walk, mod);
    }
}

/* 00005640 00006600 */ SInt8 *snd_AMEFunction(/* 0x0(sp) */ MIDIHandlerPtr stream, /* 0x4(sp) */ UInt8 *ame_header) {
    /* -0x38(sp) */ SInt32 compare_value;
    /* -0x34(sp) */ UInt8 *command_ptr;
    /* -0x30(sp) */ SInt16 next_command;
    /* -0x2c(sp) */ SInt32 ignore_next_command;
    /* -0x28(sp) */ AMEStreamHeaderPtr stream_to_stop;
    /* -0x24(sp) */ SInt32 y;
    /* -0x20(sp) */ SInt32 group;
    /* -0x1c(sp) */ SInt32 counter;
    /* -0x18(sp) */ SInt32 done;
    /* -0x14(sp) */ SInt32 stop;
    /* -0x10(sp) */ SInt8 *stream_end;
    /* -0xc(sp) */ AMEHandlerPtr ame_handler;

    if (stream == NULL) {
        return (SInt8 *)1;
    }

    if (stream->SH.parent != NULL) {
        ame_handler = (AMEHandlerPtr)stream->SH.parent;
    } else {
        ame_handler = (AMEHandlerPtr)stream;
    }

    command_ptr = ame_header;
    ignore_next_command = 0;
    done = 0;
    stop = 0;

    while (!done) {
        next_command = *command_ptr++;
        switch (next_command) {
        case 0:
            if (ignore_next_command) {
                if (ignore_next_command == 1) {
                    ignore_next_command = 0;
                } else {
                    ignore_next_command = gGlobalExcite < command_ptr[0] + 1;
                }
            }
            command_ptr += 1;
            break;
        case 1:
            if (ignore_next_command) {
                if (ignore_next_command == 1) {
                    ignore_next_command = 0;
                } else {
                    ignore_next_command = gGlobalExcite != command_ptr[0] + 1;
                }
            }
            command_ptr += 1;
            break;
        case 2:
            if (ignore_next_command) {
                if (ignore_next_command == 1) {
                    ignore_next_command = 0;
                } else {
                    ignore_next_command = gGlobalExcite > command_ptr[0] + 1;
                }
            }
            command_ptr += 1;
            break;
        case 3:
            if (ignore_next_command) {
                if (ignore_next_command == 1) {
                    ignore_next_command = 0;
                } else {
                    stream_to_stop = snd_FindAMEStream(stream, command_ptr[0]);
                    if (&stream_to_stop->MH == stream) {
                        stop = 1;
                        done = 1;
                    } else {
                        snd_StopAMESegment(&stream_to_stop->MH);
                    }
                }
            }
            command_ptr += 1;
            break;
        case 4:
            if (ignore_next_command == 1)
                ignore_next_command = 2;
            break;
        case 5:
            if (ignore_next_command == 2)
                ignore_next_command = 0;
            break;
        case 6:
            if (ignore_next_command) {
                if (ignore_next_command == 1) {
                    ignore_next_command = 0;
                }
            } else {
                ignore_next_command = command_ptr[1] - 1 < ame_handler->MIDIRegister[command_ptr[0]];
            }
            command_ptr += 2;
            break;
        case 7:
            if (ignore_next_command) {
                if (ignore_next_command == 1) {
                    ignore_next_command = 0;
                }
            } else {
                ignore_next_command = ame_handler->MIDIRegister[command_ptr[0]] < command_ptr[1] + 1;
            }
            command_ptr += 2;
            break;
        case 11:
            ame_handler->MIDIMacro[command_ptr[0]] = &command_ptr[1];
            while (*command_ptr != 0xf7) {
                command_ptr++;
            }
            command_ptr++;
            break;
        case 12:
            if (ignore_next_command) {
                if (ignore_next_command == 1) {
                    ignore_next_command = 0;
                }
            } else {
                stream_end = snd_AMEFunction(stream, ame_handler->MIDIMacro[command_ptr[0]]);
                if (stream_end == NULL) {
                    stop = 1;
                    done = 1;
                }
            }
            command_ptr += 1;
            break;
        case 13:
            if (ignore_next_command) {
                if (ignore_next_command == 1) {
                    ignore_next_command = 0;
                }
            } else {
                stop = 1;
                done = 1;
                snd_StartAMESegment(ame_handler, ame_handler->MIDIRegister[command_ptr[0]] - 1);
            }
            command_ptr += 1;
            break;
        case 14:
            if (ignore_next_command) {
                if (ignore_next_command == 1) {
                    ignore_next_command = 0;
                }
            } else {
                snd_StartAMESegment(ame_handler, ame_handler->MIDIRegister[command_ptr[0]] - 1);
            }
            command_ptr += 1;
            break;
        case 15:
            if (ignore_next_command) {
                while (*command_ptr != 0xf7) {
                    command_ptr++;
                }
                command_ptr++;
                if (ignore_next_command == 1) {
                    ignore_next_command = 0;
                }
            } else {
                group = *command_ptr++;
                ame_handler->group[group].basis = *command_ptr++;
                for (y = 0; *command_ptr != 0xf7; y++) {
                    ame_handler->group[group].channel[y] = *command_ptr++;
                    ame_handler->group[group].excite_min[y] = *command_ptr++;
                    ame_handler->group[group].excite_max[y] = *command_ptr++;
                }
                ame_handler->group[group].num_channels = y;
                command_ptr += 1;
            }
            break;
        case 16:
            if (ignore_next_command) {
                if (ignore_next_command == 1) {
                    ignore_next_command = 0;
                }
            } else {
                if (ame_handler->group[command_ptr[0]].basis == 0) {
                    compare_value = gGlobalExcite;
                } else {
                    compare_value = ame_handler->MIDIRegister[ame_handler->group[command_ptr[0]].basis - 1];
                }
                for (counter = 0; counter < ame_handler->group[command_ptr[0]].num_channels; counter++) {
                    if (ame_handler->group[command_ptr[0]].excite_min[counter] - 1 >= compare_value ||
                        ame_handler->group[command_ptr[0]].excite_max[counter] + 1 < compare_value) {
                        snd_MuteMIDIChannel(stream, ame_handler->group[command_ptr[0]].channel[counter]);
                    } else {
                        snd_UnmuteMIDIChannel(stream, ame_handler->group[command_ptr[0]].channel[counter]);
                    }
                }
            }
            command_ptr += 1;
            break;
        case 17:
            if (ignore_next_command) {
                if (ignore_next_command == 1) {
                    ignore_next_command = 0;
                }
            } else {
                stop = 1;
                done = 1;
                snd_StartAMESegment(ame_handler, command_ptr[0]);
            }
            command_ptr += 1;
            break;
        case 18:
            if (ignore_next_command) {
                if (ignore_next_command == 1) {
                    ignore_next_command = 0;
                }
            } else {
                snd_StartAMESegment(ame_handler, command_ptr[0]);
            }
            command_ptr += 1;
            break;
        case 19:
            if (ignore_next_command) {
                if (ignore_next_command == 1) {
                    ignore_next_command = 0;
                }
            } else {
                ame_handler->MIDIRegister[command_ptr[0]] = command_ptr[1];
            }
            command_ptr += 2;
            break;
        case 20:
            if (ignore_next_command) {
                if (ignore_next_command == 1) {
                    ignore_next_command = 0;
                }
            } else {
                if (ame_handler->MIDIRegister[command_ptr[0]] != 127) {
                    ame_handler->MIDIRegister[command_ptr[0]]++;
                }
            }
            command_ptr += 1;
            break;
        case 21:
            if (ignore_next_command) {
                if (ignore_next_command == 1) {
                    ignore_next_command = 0;
                }
            } else {
                if (ame_handler->MIDIRegister[command_ptr[0]] > 0) {
                    ame_handler->MIDIRegister[command_ptr[0]]--;
                }
            }
            command_ptr += 1;
            break;
        case 22:
            if (ignore_next_command) {
                if (ignore_next_command == 1) {
                    ignore_next_command = 0;
                }
            } else {
                ignore_next_command = ame_handler->MIDIRegister[command_ptr[0]] != command_ptr[1];
            }
            command_ptr += 2;
            break;

        default:
            break;
        }

        if (*ame_header == 0xf7) {
            done = 1;
        }
    }

    if (stop) {
        return 0;
    }

    return command_ptr;
}

/* 00006600 00006744 */ void snd_SetMIDIRegister(/* 0x0(sp) */ UInt32 handle, /* 0x4(sp) */ SInt32 reg, /* -0x10(sp) */ SInt16 value) {
    /* -0xc(sp) */ AMEHandlerPtr ame;
    if (reg < 0 || reg >= 16) {
        snd_ShowError(92, reg, value, 0, 0);
        return;
    }

    snd_LockMasterTick(10);

    if (HND_GET_TYPE(handle) != HANDLER_AME) {
        snd_UnlockMasterTick();
        return;
    }

    ame = (AMEHandlerPtr)snd_CheckHandlerStillActive(handle);
    if (ame == NULL) {
        snd_UnlockMasterTick();
        return;
    }

    if (value > 127) {
        value = 127;
    }
    if (value < 0) {
        value = 0;
    }

    ame->MIDIRegister[reg] = value;

    snd_UnlockMasterTick();
}

/* 00006744 00006844 */ void snd_SetAllMIDIRegister(/* 0x0(sp) */ UInt32 handle, /* 0x4(sp) */ SInt8 *vals) {
    /* -0x10(sp) */ AMEHandlerPtr ame;
    /* -0xc(sp) */ SInt32 i;
    snd_LockMasterTick(11);

    if (HND_GET_TYPE(handle) != HANDLER_AME) {
        snd_UnlockMasterTick();
        return;
    }

    ame = (AMEHandlerPtr)snd_CheckHandlerStillActive(handle);
    if (ame == NULL) {
        snd_UnlockMasterTick();
        return;
    }

    for (i = 0; i < 16; i++) {
        ame->MIDIRegister[i] = vals[i];
    }

    snd_UnlockMasterTick();
}

/* 00006844 000068f0 */ SInt32 snd_GetMIDIRegister(/* 0x0(sp) */ UInt32 handle, /* 0x4(sp) */ SInt32 reg) {
    /* -0x10(sp) */ AMEHandlerPtr ame;

    if (HND_GET_TYPE(handle) != HANDLER_AME) {
        return 0;
    }

    ame = (AMEHandlerPtr)snd_CheckHandlerStillActive(handle);
    if (ame == NULL) {
        return 0;
    }

    return ame->MIDIRegister[reg];
}
