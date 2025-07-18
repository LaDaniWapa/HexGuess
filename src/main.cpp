#include <cstdio>
#include <cstring>
#include <3ds.h>
#include <3ds/console.h>
#include <citro2d.h>

#include "helper.h"
#include "shake128.h"


int main()
{
    // Init Libs
    gfxInitDefault();
    C3D_Init(C3D_DEFAULT_CMDBUF_SIZE);
    C2D_Init(C2D_DEFAULT_MAX_OBJECTS);
    C2D_Prepare();
    consoleInit(GFX_BOTTOM, nullptr);

    C3D_RenderTarget* top = C2D_CreateScreenTarget(GFX_TOP, GFX_LEFT);

    // ABGR
    u32 clrClear = 0xffe4e4e4;
    u32 clrRed = getTodaysColor();
    printf("todays color = %x\n", clrRed);

    // Main loop
    while (aptMainLoop())
    {
        hidScanInput();

        // Respond to user input
        u32 kDown = hidKeysDown();

        if (kDown & KEY_START)
            break; // Exit app

        printf("\x1b[2;1HCPU:     %6.2f%%\x1b[K", C3D_GetProcessingTime()*6.0f);
        printf("\x1b[3;1HGPU:     %6.2f%%\x1b[K", C3D_GetDrawingTime()*6.0f);
        printf("\x1b[4;1HCmdBuf:  %6.2f%%\x1b[K", C3D_GetCmdBufUsage()*100.0f);

        // Render the scene
        C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
        C2D_TargetClear(top, clrClear);
        C2D_SceneBegin(top);

        C2D_DrawRectSolid(10, 10, 0, 50, 50, clrRed);

        C3D_FrameEnd(0);
    }

    // Deinit Libs
    C2D_Fini();
    C3D_Fini();
    gfxExit();
    return 0;
}