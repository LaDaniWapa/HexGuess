#include <cstdio>
#include <cstring>
#include <3ds.h>
#include <3ds/console.h>
#include <citro2d.h>

#include "colors.h"
#include "helper.h"
#include "shake128.h"


int main()
{
    // Init Libs
    gfxInitDefault();
    mcuHwcInit();
    C3D_Init(C3D_DEFAULT_CMDBUF_SIZE);
    C2D_Init(C2D_DEFAULT_MAX_OBJECTS);
    C2D_Prepare();

    // Create render targets
    C3D_RenderTarget* top = C2D_CreateScreenTarget(GFX_TOP, GFX_LEFT);
    C3D_RenderTarget* bottom = C2D_CreateScreenTarget(GFX_BOTTOM, GFX_LEFT);

    // ABGR
    const u32 todaysColor = getTodaysColor();

    C2D_Text txtHexGuess;
    C2D_TextBuf bufHexGuess = C2D_TextBufNew(20);
    C2D_TextParse(&txtHexGuess, bufHexGuess, "HexGuess");

    C2D_Text txtTime;
    C2D_Text txtBattery;

    // Main loop
    while (aptMainLoop())
    {
        hidScanInput();
        updateStatusBar(&txtTime, &txtBattery);

        // Respond to user input
        u32 kDown = hidKeysDown();

        if (kDown & KEY_START) break; // Exit app

        // Render to top screen
        C3D_FrameBegin(C3D_FRAME_SYNCDRAW);

        C2D_TargetClear(top, COLOR_BACKGROUND);
        C2D_SceneBegin(top);

        // Draw status bar
        C2D_DrawRectSolid(0, 0, 0, 400, 20, COLOR_SURFACE);
        C2D_DrawLine(0, 21, COLOR_BLACK, 400, 21, COLOR_BLACK, 1, 1);

        C2D_DrawText(&txtTime, 0, 10, 2, 0, 0.5, 0.5);
        C2D_DrawText(&txtHexGuess, C2D_AlignCenter, 200, 0, 0, 0.7, 0.7);
        C2D_DrawText(&txtBattery, C2D_AlignRight, 390, 2, 0, 0.5, 0.5);

        //C2D_Flush();
        C2D_TargetClear(bottom, COLOR_DARK_PURPLE);
        C2D_SceneBegin(bottom);

        // You can add animated elements here to simulate the star effect
        // For example, draw small rectangles with varying colors based on time
        static float time = 0.0f;
        time += 0.026f; // Approximate 60fps

        for (int x = 0; x < 320; x += 20)
        {
            for (int y = 0; y < 240; y += 20)
            {
                const float phase = time + (x * 0.1f) + (y * 0.2f);
                const float intensity = (sin(phase) + 1.0f) * 0.5f; // 0.0 to 1.0

                // Interpolate between dark and light purple
                const u8 r = static_cast<u8>(77 + (153 - 77) * intensity);
                const u8 g = static_cast<u8>(38 + (102 - 38) * intensity);
                const u8 b = static_cast<u8>(153 + (230 - 153) * intensity);
                const u32 color = C2D_Color32(r, g, b, 255);

                C2D_DrawRectSolid(x + 7, y + 7, 0, 4, 4, color);
            }
        }

        C3D_FrameEnd(0);
    }

    // De-init Libs
    C2D_Fini();
    C3D_Fini();
    gfxExit();
    return 0;
}
