#include "reverb.h"
#include "types.h"
#include "util.h"

/* bss 110 */ struct BasicEffect gVerbFX[2][2];

void snd_AutoReverb_L(SInt32 core, SInt32 depth, SInt32 delta_time, SInt32 channel_flags);

/* 00007c20 00007cb0 */ void snd_AutoReverb(/* 0x0(sp) */ SInt32 core, /* 0x4(sp) */ SInt32 depth, /* 0x8(sp) */ SInt32 delta_time, /* 0xc(sp) */ SInt32 channel_flags) {
    if ((core & 1) != 0) {
        snd_AutoReverb_L(0, depth, delta_time, channel_flags);
    }

    if ((core & 2) != 0) {
        snd_AutoReverb_L(1, depth, delta_time, channel_flags);
    }
}

/* 00007cb0 000081e8 */ void snd_AutoReverb_L(/* 0x0(sp) */ SInt32 core, /* 0x4(sp) */ SInt32 depth, /* 0x8(sp) */ SInt32 delta_time, /* 0xc(sp) */ SInt32 channel_flags) {
    /* -0x18(sp) */ SInt32 depthchange;
    /* -0x14(sp) */ SInt16 original_depth;
    /* -0x10(sp) */ SInt32 effect_index;

    if (channel_flags == 3) {
        snd_AutoReverb(core ? 2 : 1, depth, delta_time, 1);
        snd_AutoReverb(core ? 2 : 1, depth, delta_time, 2);
        return;
    }

    if (channel_flags == 1) {
        effect_index = 0;
        original_depth = gReverbDepthL[core];
    } else {
        effect_index = 1;
        original_depth = gReverbDepthR[core];
    }

    gVerbFX[core][effect_index].delta_value = 0;
    if (delta_time != 0) {
        depthchange = depth - original_depth;
        if (depth == original_depth) {
            return;
        }

        gVerbFX[core][effect_index].destination = depth;
        if (abs(depthchange) >= delta_time) {
            gVerbFX[core][effect_index].ec.delta_type = 0;
            gVerbFX[core][effect_index].ec.delta_counter = 0;
            gVerbFX[core][effect_index].delta_value = depthchange / delta_time;
        } else {
            gVerbFX[core][effect_index].delta_time = abs(delta_time / depthchange);
            gVerbFX[core][effect_index].ec.delta_type = 1;
            gVerbFX[core][effect_index].ec.delta_counter = gVerbFX[core][effect_index].delta_time;
            gVerbFX[core][effect_index].delta_value = (depth >= original_depth) ? 1 : -1;
        }

        return;
    }

    if (channel_flags == 1) {
        snd_SetReverbDepth_L(core, depth, gReverbDepthR[core]);
    } else {
        snd_SetReverbDepth_L(core, gReverbDepthL[core], depth);
    }
}

/* 000081e8 0000825c */ void snd_KillAutoVerbForCore(/* 0x0(sp) */ SInt32 core) {
    gVerbFX[core][0].delta_value = 0;
    gVerbFX[core][1].delta_value = 0;
}

/* 0000825c 00008738 */ void snd_AutoVerbHandler() {
    /* -0x10(sp) */ SInt32 core;
    /* -0xc(sp) */ SInt32 newdepth;

    for (core = 0; core < 2; ++core) {
        if (gVerbFX[core][0].delta_value) {
            newdepth = gReverbDepthL[core] + gVerbFX[core][0].delta_value;

            if ((gVerbFX[core][0].delta_value > 0 && gVerbFX[core][0].destination < newdepth) ||
                (gVerbFX[core][0].delta_value < 0 && newdepth < gVerbFX[core][0].destination)) {
                newdepth = gVerbFX[core][0].destination;
            }

            snd_SetReverbDepth_L(core, newdepth, gReverbDepthR[core]);

            if (newdepth == gVerbFX[core][0].destination)
                gVerbFX[core][0].delta_value = 0;
        }

        if (gVerbFX[core][1].delta_value) {
            newdepth = gReverbDepthR[core] + gVerbFX[core][1].delta_value;

            if ((gVerbFX[core][1].delta_value > 0 && gVerbFX[core][1].destination < newdepth) ||
                (gVerbFX[core][1].delta_value < 0 && newdepth < gVerbFX[core][1].destination)) {
                newdepth = gVerbFX[core][1].destination;
            }

            snd_SetReverbDepth_L(core, gReverbDepthL[core], newdepth);

            if (newdepth == gVerbFX[core][1].destination)
                gVerbFX[core][1].delta_value = 0;
        }
    }
}
