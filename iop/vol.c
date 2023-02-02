#include "vol.h"

#include "989snd.h"
#include "intrman.h"
#include "libsd.h"
#include "moviesnd.h"
#include "pantable.h"
#include "sndhand.h"
#include "types.h"
#include "valloc.h"

/* data 1e6c */ SInt32 gMasterVol[32] = {
    0x400,
    0x400,
    0x400,
    0x400,
    0x400,
    0x400,
    0x400,
    0x400,
    0x400,
    0x400,
    0x400,
    0x400,
    0x400,
    0x400,
    0x400,
    0x400,
    0x400,
    0x400,
    0x400,
    0x400,
    0x400,
    0x400,
    0x400,
    0x400,
    0x400,
    0x400,
    0x400,
    0x400,
    0x400,
    0x400,
    0x400,
    0x400,
};
/* data 1eec */ SInt32 gGroupDuckMult[32] = {
    0x1000,
    0x1000,
    0x1000,
    0x1000,
    0x1000,
    0x1000,
    0x1000,
    0x1000,
    0x1000,
    0x1000,
    0x1000,
    0x1000,
    0x1000,
    0x1000,
    0x1000,
    0x1000,
    0x1000,
    0x1000,
    0x1000,
    0x1000,
    0x1000,
    0x1000,
    0x1000,
    0x1000,
    0x1000,
    0x1000,
    0x1000,
    0x1000,
    0x1000,
    0x1000,
    0x1000,
    0x1000,
};
/* data 1f6c */ SInt16 gStereoOrMono = 0;
/* data 1f70 */ SInt32 gCurrentXAVolGroup = -1;
/* data 1f74 */ SInt32 gCurrentXAVol = 0;
/* data 1f78 */ SInt32 gCurrentXAPan = 0;
/* bss 9d50 */ struct DuckerDef gDuckers[4];

/* 00037760 00037844 */ void snd_InitDuckers() {
    /* -0x10(sp) */ SInt32 i;
    for (i = 0; i < VOL_NUM_DUCKERS; i++) {
        gDuckers[i].source_group = -1;
    }

    for (i = 0; i < VOL_NUM_GROUPS; i++) {
        gGroupDuckMult[i] = 0x1000;
    }
}

/* 00037844 00037a34 */ void snd_SetMasterVolume(/* 0x0(sp) */ SInt32 which, /* 0x4(sp) */ SInt32 vol) {
    /* -0x10(sp) */ SInt32 intr_state;
    /* -0xc(sp) */ SInt32 dis;
    if (vol > 0x400) {
        vol = 0x400;
    }
    if (vol < 0) {
        vol = 0;
    }

    if (which == VOL_NUM_GROUPS) {
        snd_ShowError(83, 0, 0, 0, 0);
        return;
    }

    gMasterVol[which] = vol;
    if (g989Monitor != NULL && g989Monitor->vol_changed != NULL) {
        g989Monitor->vol_changed(which, vol);
    }

    if (which == VOL_GROUP_MASTER) {
        dis = CpuSuspendIntr(&intr_state);
        sceSdSetParam(SD_PARAM_MVOLL | 1, 0x3FFF * vol / 0x400);
        sceSdSetParam(SD_PARAM_MVOLR | 1, 0x3FFF * vol / 0x400);
        if (!dis) {
            CpuResumeIntr(intr_state);
        }
    } else {
        snd_AdjustAllChannelsMasterVolume(1 << which);
        snd_AdjustMovieVolDueToMasterChange(which);
    }
}

/* 00037a34 00037a88 */ SInt32 snd_GetMasterVolume(/* 0x0(sp) */ SInt32 which) {
    return gMasterVol[which];
}

/* 00037a88 00037ab8 */ void snd_SetPlaybackMode(/* 0x0(sp) */ SInt32 mode) {
    gStereoOrMono = mode;
}

/* 00037ab8 00037af0 */ SInt32 snd_GetPlaybackMode() {
    return gStereoOrMono;
}

/* 00037af0 00038240 */ void snd_MakeVolumes(/* 0x0(sp) */ SInt32 snd_vol, /* 0x4(sp) */ SInt32 snd_pan, /* 0x8(sp) */ SInt32 prog_vol, /* 0xc(sp) */ SInt32 prog_pan, /* 0x10(sp) */ SInt32 tone_vol, /* 0x14(sp) */ SInt32 tone_pan, /* 0x18(sp) */ SpuVolume *spu_vol) {
    /* -0x18(sp) */ SInt32 voll;
    /* -0x14(sp) */ SInt32 volr;
    /* -0x10(sp) */ SInt32 work_vol;

    work_vol = snd_vol * 258;
    work_vol = (work_vol * prog_vol) / 0x7f;
    work_vol = (work_vol * tone_vol) / 0x7f;

    if (work_vol == 0) {
        spu_vol->left = 0;
        spu_vol->right = 0;
        return;
    }

    if (gStereoOrMono == PLAYBACK_MODE_MONO) {
        spu_vol->left = work_vol;
        spu_vol->right = work_vol;
    }

    snd_pan += tone_pan + prog_pan;
    while (snd_pan >= 360) {
        snd_pan -= 360;
    }

    while (snd_pan < 0) {
        snd_pan += 360;
    }

    if (snd_pan >= 270) {
        snd_pan -= 270;
    } else {
        snd_pan += 90;
    }

    if (snd_pan < 180) {
        voll = (gPanTable[snd_pan].left * work_vol) / 0x3fff;
        volr = (gPanTable[snd_pan].right * work_vol) / 0x3fff;

        if (spu_vol->left < 0 && spu_vol->right < 0) {
            voll = -voll;
            voll = -volr;
        }

        if (spu_vol->left >= 0 && spu_vol->right < 0) {
            if (-spu_vol->right >= spu_vol->left) {
                voll = voll * (spu_vol->right / -spu_vol->right);
                volr = volr * (spu_vol->right / -spu_vol->right);
            } else {
                voll = voll * (spu_vol->left / spu_vol->left);
                volr = volr * (spu_vol->left / spu_vol->left);
            }
        }

        if (spu_vol->left < 0 && spu_vol->right >= 0) {
            if (spu_vol->right >= -spu_vol->left) {
                voll = voll * (spu_vol->right / spu_vol->right);
                volr = volr * (spu_vol->right / spu_vol->right);
            } else {
                voll = voll * (spu_vol->left / -spu_vol->left);
                volr = volr * (spu_vol->left / -spu_vol->left);
            }
        }
    } else {
        volr = (gPanTable[snd_pan - 180].left * work_vol) / 0x3fff;
        voll = (gPanTable[snd_pan - 180].right * work_vol) / 0x3fff;

        if (gStereoOrMono != 2) {
            if (spu_vol->left < 0 && spu_vol->right < 0) {
                if (voll >= volr) {
                    voll = -voll;
                } else {
                    volr = -volr;
                }
            }

            if (spu_vol->left >= 0 && spu_vol->right >= 0) {
                if (volr >= voll) {
                    voll = -voll;
                } else {
                    volr = -volr;
                }
            }

            if (spu_vol->left >= 0 && spu_vol->right < 0) {
                volr = -volr;
            }

            if (spu_vol->left < 0 && spu_vol->right >= 0) {
                voll = -voll;
            }
        }
    }

    spu_vol->left = voll;
    spu_vol->right = volr;
}

/* 00038240 000389c8 */ void snd_MakeVolumesB(/* 0x0(sp) */ SInt32 bank_vol, /* 0x4(sp) */ SInt32 snd_vol, /* 0x8(sp) */ SInt32 snd_pan, /* 0xc(sp) */ SInt32 prog_vol, /* 0x10(sp) */ SInt32 prog_pan, /* 0x14(sp) */ SInt32 tone_vol, /* 0x18(sp) */ SInt32 tone_pan, /* 0x1c(sp) */ SpuVolume *spu_vol) {
    /* -0x18(sp) */ SInt32 voll;
    /* -0x14(sp) */ SInt32 volr;
    /* -0x10(sp) */ SInt32 work_vol;

    work_vol = snd_vol * 258;
    work_vol = (work_vol * prog_vol) / 0x7f;
    work_vol = (work_vol * tone_vol) / 0x7f;
    work_vol = (work_vol * bank_vol) / 0x7f;

    if (work_vol == 0) {
        spu_vol->left = 0;
        spu_vol->right = 0;
        return;
    }

    if (gStereoOrMono == PLAYBACK_MODE_MONO) {
        spu_vol->left = work_vol;
        spu_vol->right = work_vol;
    }

    snd_pan += tone_pan + prog_pan;
    while (snd_pan >= 360) {
        snd_pan -= 360;
    }

    while (snd_pan < 0) {
        snd_pan += 360;
    }

    if (snd_pan >= 270) {
        snd_pan -= 270;
    } else {
        snd_pan += 90;
    }

    if (snd_pan < 180) {
        voll = (gPanTable[snd_pan].left * work_vol) / 0x3fff;
        volr = (gPanTable[snd_pan].right * work_vol) / 0x3fff;

        if (spu_vol->left < 0 && spu_vol->right < 0) {
            voll = -voll;
            voll = -volr;
        }

        if (spu_vol->left >= 0 && spu_vol->right < 0) {
            if (-spu_vol->right >= spu_vol->left) {
                voll = voll * (spu_vol->right / -spu_vol->right);
                volr = volr * (spu_vol->right / -spu_vol->right);
            } else {
                voll = voll * (spu_vol->left / spu_vol->left);
                volr = volr * (spu_vol->left / spu_vol->left);
            }
        }

        if (spu_vol->left < 0 && spu_vol->right >= 0) {
            if (spu_vol->right >= -spu_vol->left) {
                voll = voll * (spu_vol->right / spu_vol->right);
                volr = volr * (spu_vol->right / spu_vol->right);
            } else {
                voll = voll * (spu_vol->left / -spu_vol->left);
                volr = volr * (spu_vol->left / -spu_vol->left);
            }
        }
    } else {
        volr = (gPanTable[snd_pan - 180].left * work_vol) / 0x3fff;
        voll = (gPanTable[snd_pan - 180].right * work_vol) / 0x3fff;

        if (gStereoOrMono != 2) {
            if (spu_vol->left < 0 && spu_vol->right < 0) {
                if (voll >= volr) {
                    voll = -voll;
                } else {
                    volr = -volr;
                }
            }

            if (spu_vol->left >= 0 && spu_vol->right >= 0) {
                if (volr >= voll) {
                    voll = -voll;
                } else {
                    volr = -volr;
                }
            }

            if (spu_vol->left >= 0 && spu_vol->right < 0) {
                volr = -volr;
            }

            if (spu_vol->left < 0 && spu_vol->right >= 0) {
                voll = -voll;
            }
        }
    }

    spu_vol->left = voll;
    spu_vol->right = volr;
}

/* 000389c8 00039044 */ void snd_CalcStereoBalance3d(/* 0x0(sp) */ SInt32 snd_vol, /* 0x4(sp) */ SInt32 snd_pan, /* 0x8(sp) */ SpuVolume *spu_vol) {
    /* -0x18(sp) */ SInt32 voll;
    /* -0x14(sp) */ SInt32 volr;
    /* -0x10(sp) */ SInt32 work_vol;

    work_vol = snd_vol * 258;

    if (work_vol == 0) {
        spu_vol->left = 0;
        spu_vol->right = 0;
        return;
    }

    if (gStereoOrMono == PLAYBACK_MODE_MONO) {
        spu_vol->left = work_vol;
        spu_vol->right = work_vol;
    }

    while (snd_pan >= 360) {
        snd_pan -= 360;
    }

    while (snd_pan < 0) {
        snd_pan += 360;
    }

    if (snd_pan >= 270) {
        snd_pan -= 270;
    } else {
        snd_pan += 90;
    }

    if (snd_pan < 180) {
        voll = (gPanTable2[snd_pan].left * work_vol) / 0x3fff;
        volr = (gPanTable2[snd_pan].right * work_vol) / 0x3fff;

        if (spu_vol->left < 0 && spu_vol->right < 0) {
            voll = -voll;
            voll = -volr;
        }

        if (spu_vol->left >= 0 && spu_vol->right < 0) {
            if (-spu_vol->right >= spu_vol->left) {
                voll = voll * (spu_vol->right / -spu_vol->right);
                volr = volr * (spu_vol->right / -spu_vol->right);
            } else {
                voll = voll * (spu_vol->left / spu_vol->left);
                volr = volr * (spu_vol->left / spu_vol->left);
            }
        }

        if (spu_vol->left < 0 && spu_vol->right >= 0) {
            if (spu_vol->right >= -spu_vol->left) {
                voll = voll * (spu_vol->right / spu_vol->right);
                volr = volr * (spu_vol->right / spu_vol->right);
            } else {
                voll = voll * (spu_vol->left / -spu_vol->left);
                volr = volr * (spu_vol->left / -spu_vol->left);
            }
        }
    } else {
        volr = (gPanTable2[snd_pan - 180].left * work_vol) / 0x3fff;
        voll = (gPanTable2[snd_pan - 180].right * work_vol) / 0x3fff;

        if (gStereoOrMono != 2) {
            if (spu_vol->left < 0 && spu_vol->right < 0) {
                if (voll >= volr) {
                    voll = -voll;
                } else {
                    volr = -volr;
                }
            }

            if (spu_vol->left >= 0 && spu_vol->right >= 0) {
                if (volr >= voll) {
                    voll = -voll;
                } else {
                    volr = -volr;
                }
            }

            if (spu_vol->left >= 0 && spu_vol->right < 0) {
                volr = -volr;
            }

            if (spu_vol->left < 0 && spu_vol->right >= 0) {
                voll = -voll;
            }
        }
    }

    spu_vol->left = voll;
    spu_vol->right = volr;
}

/* 00039044 000391cc */ SInt16 snd_AdjustVolToGroup(/* -0x18(sp) */ SInt16 vol, /* 0x4(sp) */ SInt32 group) {
    /* -0x14(sp) */ SInt32 phase;
    /* -0x10(sp) */ SInt32 new_vol;
    /* -0xc(sp) */ SInt32 m_vol;

    if (group >= VOL_NUM_GROUPS) {
        return vol;
    }
    if (vol >= 0x7fff) {
        vol = 0x7ffe;
    }

    m_vol = (gMasterVol[group] * gGroupDuckMult[group]) / 0x10000;
    new_vol = (vol * m_vol) / 0x400;

    phase = 1;
    if (new_vol < 0) {
        phase = -1;
    }

    return (SInt16)(((new_vol * new_vol) / 0x7ffe) * phase);
}

/* 000391cc 000394fc */ void snd_AdjustAllChannelsMasterVolume(/* 0x0(sp) */ UInt32 flags) {
    /* -0x20(sp) */ SInt32 count;
    /* -0x1c(sp) */ SInt32 intr_state;
    /* -0x18(sp) */ SInt32 dis;
    /* -0x14(sp) */ UInt32 core;
    /* -0x10(sp) */ UInt32 c_v;
    snd_LockVoiceAllocatorEx(1, 138);
    dis = CpuSuspendIntr(&intr_state);
    for (int i = 0; i < NUM_VOICES; i++) {
        if ((gChannelStatus[i].Owner->flags & HND_PAUSED) == 0 &&
            (gChannelStatus[i].Status == 1 || gChannelStatus[i].Status == 4) &&
            (flags & (1 << gChannelStatus[i].VolGroup)) != 0) {
            core = i / NUM_VOICE_PER_CORE;
            c_v = i % NUM_VOICE_PER_CORE;

            sceSdSetParam(SD_VPARAM_VOLL | SD_VOICE(core, c_v),
                          snd_AdjustVolToGroup(gChannelStatus[i].Volume.left, gChannelStatus[i].VolGroup) >> 1);
            sceSdSetParam(SD_VPARAM_VOLR | SD_VOICE(core, c_v),
                          snd_AdjustVolToGroup(gChannelStatus[i].Volume.right, gChannelStatus[i].VolGroup) >> 1);
        }
    }

    if (!dis) {
        CpuResumeIntr(intr_state);
    }
    snd_UnlockVoiceAllocator();
}

/* 000394fc 000395e8 */ SInt16 snd_MIDITo360Pan(/* -0x10(sp) */ SInt8 pan) {
    if (pan >= 64) {
        return (SInt16)(90 * (pan - 64) / 63);
    } else {
        return (SInt16)(90 * pan / 64 + 270);
    }
}

/* 000395e8 00039e1c */ void snd_CheckDuckers() {
    /* -0x58(sp) */ SInt32 i;
    /* -0x54(sp) */ SInt32 c;
    /* -0x50(sp) */ UInt32 playing_groups = 0;
    /* -0x4c(sp) */ UInt32 affected_groups;
    /* -0x48(sp) */ UInt32 all_affected_groups = 0;
    /* -0x44(sp) */ SInt32 m_vol_track[15];
    snd_LockVoiceAllocatorEx(1, 139);
    for (i = 0; i < NUM_VOICES; i++) {
        if (gChannelStatus[i].Status == 1 || gChannelStatus[i].Status == 4) {
            if (gChannelStatus[i].Owner != NULL) {
                if ((gChannelStatus[i].Owner->flags & HND_PAUSED) == 0) {
                    playing_groups |= 1 << gChannelStatus[i].VolGroup;
                }
            }
        }
    }
    snd_UnlockVoiceAllocator();
    for (c = 0; c < VOL_NUM_GROUPS; c++) {
        m_vol_track[c] = 0x10000;
    }

    for (i = 0; i < VOL_NUM_DUCKERS; i++) {
        affected_groups = 0;

        if (gDuckers[i].source_group == -1) {
            continue;
        }

        if ((playing_groups & (1 << gDuckers[i].source_group)) != 0) {
            if (gDuckers[i].current_duck_mult != gDuckers[i].full_duck_mult) {
                gDuckers[i].current_duck_mult -= gDuckers[i].attack_time;
                if (gDuckers[i].current_duck_mult < gDuckers[i].full_duck_mult) {
                    gDuckers[i].current_duck_mult = gDuckers[i].full_duck_mult;
                }
            }

            affected_groups = gDuckers[i].target_groups;
        } else if (gDuckers[i].current_duck_mult != 0x10000) {
            gDuckers[i].current_duck_mult += gDuckers[i].release_time;
            if (gDuckers[i].current_duck_mult >= 0x10000 || gDuckers[i].current_duck_mult < 0) {
                gDuckers[i].current_duck_mult = 0x10000;
            }

            affected_groups = gDuckers[i].target_groups;
        }

        if (affected_groups != 0) {
            for (c = 0; c < VOL_NUM_GROUPS; c++) {
                if ((affected_groups & (1 << c)) != 0 && gDuckers[i].current_duck_mult < m_vol_track[c]) {
                    m_vol_track[c] = gDuckers[i].current_duck_mult;
                }

                all_affected_groups |= affected_groups;
            }
        }
    }

    // huh
    all_affected_groups = 0;
    for (c = 0; c < VOL_NUM_GROUPS; c++) {
        if (gGroupDuckMult[c] != m_vol_track[c]) {
            gGroupDuckMult[c] = m_vol_track[c];
            all_affected_groups |= 1 << c;
        }
    }

    if (all_affected_groups != 0) {
        snd_AdjustAllChannelsMasterVolume(all_affected_groups);
    }
}

/* 00039e1c 0003a1a0 */ void snd_SetMasterVolumeDucker(/* 0x0(sp) */ SInt32 which, /* 0x4(sp) */ DuckerDefPtr state) {
    /* -0x10(sp) */ SInt32 delta;
    /* -0xc(sp) */ SInt32 frames;

    if (which < 0 || which >= 4) {
        return;
    }

    if (state == NULL) {
        gDuckers[which].source_group = -1;
        return;
    }

    gDuckers[which] = *state;
    delta = 0x10000 - gDuckers[which].full_duck_mult;
    frames = (30 * gDuckers[which].attack_time) >> 16;
    if (frames == 0) {
        frames = 1;
    }
    gDuckers[which].attack_time = delta / frames;
    if (gDuckers[which].attack_time == 0) {
        gDuckers[which].attack_time = 1;
    }

    frames = (30 * gDuckers[which].release_time) >> 16;
    if (frames == 0) {
        frames = 1;
    }
    gDuckers[which].release_time = delta / frames;
    if (gDuckers[which].release_time == 0) {
        gDuckers[which].release_time = 1;
    }

    gDuckers[which].current_duck_mult = 0x10000;
}
