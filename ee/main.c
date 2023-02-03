#include "989snd.h"

#include <debug.h>
#include <iopcontrol.h>
#include <libgs.h>
#include <loadfile.h>
#include <sifrpc.h>
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

    // SifLoadModule("host0:data/LIBSD.IRX", 0, NULL);
    // SifLoadModule("host0:iop/989SND_D.IRX", 0, NULL);
    // SifLoadModule("host0:iop/989SND_j2.IRX", 0, NULL);
    SifLoadModule("host0:989snd.irx", 0, NULL);
    SifLoadModule("rom0:SIO2MAN", 0, NULL);
    SifLoadModule("rom0:PADMAN", 0, NULL);

    printf("bla\n");
    scr_printf("starting sound system\n");

    printf("bla\n");
    printf("bla\n");
    printf("bla\n");
    printf("bla\n");

    snd_StartSoundSystemEx(3);

    return 0;
}
