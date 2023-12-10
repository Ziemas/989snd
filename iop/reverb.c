#include "reverb.h"
#include "989snd.h"
#include "autoverb.h"
#include "intrman.h"
#include "libsd.h"
#include "loader.h"
#include "sram.h"
#include "types.h"

/* data 1bc0 */ SInt32 gReverbType[2] = {-1, -1};
/* data 1bc8 */ UInt32 gRevRamStart[2] = {0, 0};
/* data 1bd0 */ SInt32 gRevRamPreAllocType[2] = {-1, -1};
/* data 1bd8 */ SInt16 gReverbDepthL[2] = {0, 0};
/* data 1bdc */ SInt16 gReverbDepthR[2] = {0, 0};
/* data 1be0 */ SInt32 gAwaitingReverbChange[2] = {0, 0};
/* data 1be8 */ SInt32 gAwaitingReverbChangeTick[2] = {0, 0};
/* data 1bf0 */ SInt32 gReverbMode = 0;
/* data 1bf4 */ UInt32 gRevRamUse[] = {128, 9920, 8000, 18496, 28640, 44512, 63168, 98368, 98368, 15360};

/* 0001c8e8 0001c980 */ void snd_SetReverbMode(/* 0x0(sp) */ SInt32 reverb_mode) {
    /* -0x10(sp) */ UInt16 core1mode;
    gReverbMode = reverb_mode;
    core1mode = sceSdGetParam(SD_PARAM_MMIX | 1);
    if (gReverbMode) {
        core1mode |= 3;
    } else {
        core1mode &= ~3;
    }

    sceSdSetParam(SD_PARAM_MMIX | 1, core1mode);
}

/* 0001c980 0001c9f8 */ void snd_PreAllocReverbWorkArea(/* 0x0(sp) */ UInt32 core, /* 0x4(sp) */ SInt32 type) {
    if ((core & 1) != 0) {
        snd_PreAllocReverbWorkArea_L(0, type);
    }

    if ((core & 2) != 0) {
        snd_PreAllocReverbWorkArea_L(1, type);
    }
}

/* 0001c9f8 0001cc6c */ void snd_PreAllocReverbWorkArea_L(/* 0x0(sp) */ SInt32 core, /* 0x4(sp) */ SInt32 type) {
    /* -0x10(sp) */ SInt32 dis;
    /* -0xc(sp) */ SInt32 oldstat;

    if (gReverbType[core] > 0) {
        snd_SetReverbType_L(core, 0, 0, 0);
    }

    if (gRevRamPreAllocType[core] > 0) {
        dis = CpuSuspendIntr(&oldstat);
        snd_SRAMFree(gRevRamStart[core], gRevRamUse[gRevRamPreAllocType[core]]);
        if (!dis) {
            CpuResumeIntr(oldstat);
        }
    }
    gRevRamPreAllocType[core] = type;
    if (type > 0) {
        dis = CpuSuspendIntr(&oldstat);
        gRevRamStart[core] = snd_SRAMMallocRev(gRevRamUse[type]);
        if (!dis) {
            CpuResumeIntr(oldstat);
        }

        if (gRevRamStart[core] == 0) {
            gReverbType[core] = -1;
            gRevRamPreAllocType[core] = -1;
            snd_ShowError(85, 0, 0, 0, 0);
        }
    }
}

/* 0001cc6c 0001cd24 */ void snd_SetReverbEx(/* 0x0(sp) */ UInt32 core, /* 0x4(sp) */ SInt32 type, /* 0x8(sp) */ SInt32 depth, /* 0xc(sp) */ SInt32 delay, /* 0x10(sp) */ SInt32 feedback) {
    if ((core & 1) != 0) {
        snd_SetReverbType_L(0, type, delay, feedback);
        snd_SetReverbDepth_L(0, depth, depth);
    }
    if ((core & 2) != 0) {
        snd_SetReverbType_L(1, type, delay, feedback);
        snd_SetReverbDepth_L(1, depth, depth);
    }
}
/* 0001cd24 0001cdbc */ void snd_SetReverbType(/* 0x0(sp) */ SInt32 core, /* 0x4(sp) */ SInt32 type) {
    /* -0x10(sp) */ SInt32 ret1;
    /* -0xc(sp) */ SInt32 ret2;
    if ((core & 1) != 0) {
        snd_SetReverbType_L(0, type, 0, 0);
    }
    if ((core & 2) != 0) {
        snd_SetReverbType_L(1, type, 0, 0);
    }
}

/* 0001cdbc 0001d3b8 */ SInt32 snd_SetReverbType_L(/* 0x0(sp) */ SInt32 core, /* 0x4(sp) */ SInt32 type, /* 0x8(sp) */ SInt32 delay, /* 0xc(sp) */ SInt32 feedback) {
    /* -0x28(sp) */ sceSdEffectAttr ef;
    /* -0x14(sp) */ SInt32 ch;
    /* -0x10(sp) */ SInt32 dis;
    /* -0xc(sp) */ SInt32 oldstat;
    if (gReverbType[core] == type) {
        ef.core = core;
        ef.mode = type;
        ef.depth_L = 0;
        ef.depth_R = 0;
        ef.delay = delay;
        ef.feedback = feedback;
        sceSdSetEffectAttr(core, &ef);
        return 0;
    }

    if (gRevRamPreAllocType[core] > 0 && gRevRamUse[gRevRamPreAllocType[core]] < gRevRamUse[core]) {
        snd_ShowError(87, 0, 0, 0, 0);
        return -13;
    }

    if (gReverbType[core] > 0) {
        dis = CpuSuspendIntr(&oldstat);
        sceSdSetCoreAttr(SD_CORE_EFFECT_ENABLE | core, 0);
        sceSdSetParam(SD_PARAM_EVOLL | core, 0);
        sceSdSetParam(SD_PARAM_EVOLR | core, 0);
        if (!dis) {
            CpuResumeIntr(oldstat);
        }
    }

    if (gReverbType[core] > 0 && gRevRamPreAllocType[core] <= 0) {
        dis = CpuSuspendIntr(&oldstat);
        snd_SRAMFree(gRevRamStart[core], gRevRamUse[gReverbType[core]]);

        if (!dis) {
            CpuResumeIntr(oldstat);
        }
    }

    if (gRevRamPreAllocType[core] <= 0) {
        dis = CpuSuspendIntr(&oldstat);
        if (type != 0) {
            gRevRamStart[core] = snd_SRAMMallocRev(gRevRamUse[type]);
            if (gRevRamStart[core] == 0) {
                if (!dis) {
                    CpuResumeIntr(oldstat);
                }
                gReverbType[core] = -1;
                snd_ShowError(86, 0, 0, 0, 0);
                return -11;
            }
        }
        if (!dis) {
            CpuResumeIntr(oldstat);
        }
    }

    snd_KillAutoVerbForCore(core);

    if (type != 0) {
        sceSdSetAddr(SD_ADDR_EEA | core, gRevRamStart[core] - 1 + gRevRamUse[type]);
    }

    ef.core = core;
    ef.mode = type;
    ef.depth_L = 0;
    ef.depth_R = 0;
    ef.delay = delay;
    ef.feedback = feedback;
    sceSdSetEffectAttr(core, &ef);

    if (type != 0) {
        while (true) {
            ch = snd_GetFreeSPUDMA();
            if (ch != -1) {
                break;
            }
            DelayThread(33333);
        }
        gAwaitingReverbChange[core] = ch + 1;
        gAwaitingReverbChangeTick[core] = 10;
        sceSdCleanEffectWorkArea(core, ch, type);
    }

    gReverbType[core] = type;
    return 0;
}

/* 0001d3b8 0001d454 */ void snd_SetReverbDepth(/* 0x0(sp) */ SInt32 core, /* 0x4(sp) */ SInt32 left, /* 0x8(sp) */ SInt32 right) {
    if ((core & 1) != 0) {
        snd_KillAutoVerbForCore(0);
        snd_SetReverbDepth_L(0, left, right);
    }

    if ((core & 2) != 0) {
        snd_KillAutoVerbForCore(1);
        snd_SetReverbDepth_L(1, left, right);
    }
}

/* 0001d454 0001d5a8 */ void snd_SetReverbDepth_L(/* 0x0(sp) */ SInt32 core, /* 0x4(sp) */ SInt32 left, /* 0x8(sp) */ SInt32 right) {
    /* -0x10(sp) */ SInt32 intr_state;
    /* -0xc(sp) */ SInt32 dis;
    gReverbDepthL[core] = left;
    gReverbDepthR[core] = right;
    if (gAwaitingReverbChange[core] == 0) {
        dis = CpuSuspendIntr(&intr_state);
        sceSdSetParam(SD_PARAM_EVOLL | core, gReverbDepthL[core]);
        sceSdSetParam(SD_PARAM_EVOLR | core, gReverbDepthR[core]);
        if (!dis) {
            CpuResumeIntr(intr_state);
        }
    }
}

/* 0001d5a8 0001d778 */ void snd_CheckVerb() {
    /* -0x10(sp) */ SInt32 intr_state;
    /* -0xc(sp) */ SInt32 dis;
    if (gAwaitingReverbChangeTick[0] != 0) {
        gAwaitingReverbChangeTick[0]--;
        if (gAwaitingReverbChangeTick[0] == 0) {
            dis = CpuSuspendIntr(&intr_state);
            sceSdSetParam(SD_PARAM_EVOLL | 0, gReverbDepthL[0]);
            sceSdSetParam(SD_PARAM_EVOLR | 0, gReverbDepthR[0]);
            sceSdSetCoreAttr(SD_CORE_EFFECT_ENABLE | 0, 1);
            if (!dis) {
                CpuResumeIntr(intr_state);
            }
            snd_FreeSPUDMA(gAwaitingReverbChange[0] - 1);
            gAwaitingReverbChange[0] = 0;
        }
    }
    if (gAwaitingReverbChangeTick[1] != 0) {
        gAwaitingReverbChangeTick[1]--;
        if (gAwaitingReverbChangeTick[1] == 0) {
            dis = CpuSuspendIntr(&intr_state);
            sceSdSetParam(SD_PARAM_EVOLL | 1, gReverbDepthL[1]);
            sceSdSetParam(SD_PARAM_EVOLR | 1, gReverbDepthR[1]);
            sceSdSetCoreAttr(SD_CORE_EFFECT_ENABLE | 1, 1);
            if (!dis) {
                CpuResumeIntr(intr_state);
            }
            snd_FreeSPUDMA(gAwaitingReverbChange[1] - 1);
            gAwaitingReverbChange[1] = 0;
        }
    }

    snd_AutoVerbHandler();
}
