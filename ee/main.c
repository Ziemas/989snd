#include "989snd.h"

#include <debug.h>
#include <iopcontrol.h>
#include <libgs.h>
#include <loadfile.h>
#include <sifrpc.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>

int fioInit(void);
void fioExit(void);

int main(int argc, char *argv[]) {
    init_scr();
    GsVSync(1);
    scr_printf("989play\n");

    SifIopReboot("host0:data/IOPRP221.IMG");

    while (!SifIopSync())
        ;

    SifInitRpc(0);

    scr_printf("Iop synced, loading modules\n");

    SifLoadModule("host0:data/LIBSD.IRX", 0, NULL);
    // SifLoadModule("host0:iop/989SND_D.IRX", 0, NULL);
    // SifLoadModule("host0:iop/989SND_j2.IRX", 0, NULL);
    SifLoadModule("host0:989snd.irx", 0, NULL);
    SifLoadModule("rom0:SIO2MAN", 0, NULL);
    SifLoadModule("rom0:PADMAN", 0, NULL);

    scr_printf("starting sound system\n");

    snd_StartSoundSystemEx(0);
    SoundBankPtr bank = snd_BankLoad("host0:data/COMMON.SBK", 0);
    scr_printf("loaded bank %p\n", bank);

    SndPlayParams params;
    params.bank_spec.ptr = bank;
    params.snd_spec.index = 0;

    params.mask = (1 << 1) | (1 << 0);

    unsigned int sound = snd_PlaySoundEx(&params);
    scr_printf("playing sound %08x", sound);

    while (true)
        ;

    return 0;
}
