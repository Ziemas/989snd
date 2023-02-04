#include "989snd.h"

#include <dmaKit.h>
#include <gsKit.h>
#include <gsToolkit.h>

#include <debug.h>
#include <iopcontrol.h>
#include <loadfile.h>
#include <sifrpc.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>

GSGLOBAL *gsGlobal;
GSFONT *gsFont;
static u64 white = GS_SETREG_RGBAQ(0xFF, 0xFF, 0xFF, 0x80, 0x00);

struct Sound {
    bool in_use;
    u32 handle;
    u32 index;
    s32 timer;
};

struct Sound sounds[64];

static void print_bytes(char *buf) {
    for (int i = 0; i < 64; i++) {
        printf(" %02x", buf[i]);
        if ((i + 1) % 8 == 0)
            printf("\n");
    }
    printf("\n");
}

static struct Sound *get_free_sound() {
    for (int i = 0; i < 64; i++) {
        if (!sounds[i].in_use) {
            sounds[i].in_use = true;
            sounds[i].timer = 200;
            return &sounds[i];
        }
    }

    return NULL;
}

static void snd_play_return(unsigned ret_val, unsigned long long u_data) {
    struct Sound *sound = (struct Sound *)u_data;
    sound->handle = ret_val;
    if (ret_val == 0) {
        sound->in_use = 0;
        sound->index = 0;
    }
}

static void refresh_sound_list() {
    for (int i = 0; i < 64; i++) {
        if (sounds[i].handle != 0) {
            snd_SoundIsStillPlaying_CB(sounds[i].handle, snd_play_return, (unsigned long)&sounds[i]);
            if (sounds[i].timer-- <= 0) {
                snd_StopSound(sounds[i].handle);
            }
        }
    }
}

static void init_iop() {
    SifIopReboot("host0:data/ioprp300-fileio.img");

    while (!SifIopSync())
        ;

    SifInitRpc(0);

    SifLoadModule("host0:data/LIBSD.IRX", 0, NULL);
    // SifLoadModule("host0:data/989SND_D.IRX", 0, NULL);
    //     SifLoadModule("host0:iop/989SND_j2.IRX", 0, NULL);
    SifLoadModule("host0:989snd.irx", 0, NULL);
    // SifLoadModule("rom0:SIO2MAN", 0, NULL);
    // SifLoadModule("rom0:PADMAN", 0, NULL);

    snd_StartSoundSystemEx(0);
}

static void init_screen() {
    gsGlobal = gsKit_init_global();
    gsFont = gsKit_init_font(GSKIT_FTYPE_BMP_DAT, "host0:data/dejavu.bmp");

    dmaKit_init(D_CTRL_RELE_OFF, D_CTRL_MFD_OFF, D_CTRL_STS_UNSPEC, D_CTRL_STD_OFF, D_CTRL_RCYC_8, 1 << DMA_CHANNEL_GIF);
    dmaKit_chan_init(DMA_CHANNEL_GIF);

    gsGlobal->PrimAlpha = GS_BLEND_BACK2FRONT;
    gsGlobal->PSM = GS_PSM_CT16;
    gsGlobal->PSMZ = GS_PSMZ_16;

    gsKit_init_screen(gsGlobal);
    gsKit_mode_switch(gsGlobal, GS_ONESHOT);
    gsKit_font_upload(gsGlobal, gsFont);
    gsKit_clear(gsGlobal, GS_SETREG_RGBAQ(0, 0, 0, 0, 0));
}

static void draw_sound_list(int x, int y) {
    int off_x = 50, off_y = 20;
    char buff[32];

    gsKit_font_print(gsGlobal, gsFont, x + 10, y, 2, white, "Currently playing sounds");
    gsKit_prim_line(gsGlobal, x, y, x, y + 400, 0, white);

    for (int i = 0, p = 0; p < 27 && i < 64; i++) {
        if (sounds[i].handle == 0) {
            continue;
        }

        snprintf(buff, sizeof(buff), "%08x - %d", sounds[i].handle, sounds[i].index);
        gsKit_font_print(gsGlobal, gsFont, x + off_x, y + off_y, 2, white, buff);
        p++;
        off_y += 15;
    }
}

static void play_sound(SoundBankPtr bank, int index) {
    struct Sound *s = get_free_sound();
    if (s == NULL) {
        return;
    }

    SndPlayParams params;
    params.mask = (1 << 1) | (1 << 0);
    params.bank_spec.ptr = bank;
    params.snd_spec.index = index;

    s->index = index;

    snd_PlaySoundEx_CB(&params, snd_play_return, (unsigned long)s);
};

int main(int argc, char *argv[]) {
    char buff[32];

    init_screen();
    init_iop();
    SoundBankPtr bank = snd_BankLoad("host0:data/COMMON.SBK", 0);

    int sound = 0;
    int timer = 80;
    while (true) {
        gsKit_clear(gsGlobal, GS_SETREG_RGBAQ(0, 0, 0, 0, 0));
        gsKit_font_print(gsGlobal, gsFont, 10, 10, 2, white, "989play");

        snprintf(buff, sizeof(buff), "playing sound %d", sound);
        gsKit_font_print(gsGlobal, gsFont, 40, 40, 2, white, buff);
        if (!timer--) {
            play_sound(bank, sound++);
            timer = 40;
        }

        draw_sound_list(430, 10);
        refresh_sound_list();

        gsKit_queue_exec(gsGlobal);
        gsKit_sync_flip(gsGlobal);
        snd_FlushSoundCommands();
    }

    return 0;
}
