//
// Created by daniela on 7/19/25.
//

#include "HexGuess.h"
#include <cstdio>
#include <cstring>
#include <3ds.h>
#include <3ds/console.h>
#include "Colors.h"

void HexGuess::init()
{
    gfxInitDefault();
    mcuHwcInit();
    C3D_Init(C3D_DEFAULT_CMDBUF_SIZE);
    C2D_Init(C2D_DEFAULT_MAX_OBJECTS);
    C2D_Prepare();

    top = C2D_CreateScreenTarget(GFX_TOP, GFX_LEFT);
    bottom = C2D_CreateScreenTarget(GFX_BOTTOM, GFX_LEFT);

    targetBuffer = C2D_TextBufNew(20);
    guessBuffer = C2D_TextBufNew(20);

    txtTarget = new C2D_Text;
    txtGuess = new C2D_Text;

    C2D_TextParse(txtTarget, targetBuffer, "Target");
    C2D_TextParse(txtGuess, targetBuffer, "Guess");
}

void HexGuess::exitApplication()
{
    aptExit();
}

HexGuess::HexGuess()
{
    time = 0.f;
    statusBar = new StatusBar();
    init();
}

HexGuess::~HexGuess()
{
    if (targetBuffer) C2D_TextBufDelete(targetBuffer);
    if (guessBuffer) C2D_TextBufDelete(guessBuffer);

    C2D_Fini();
    C3D_Fini();
    gfxExit();
}


void HexGuess::update()
{
    hidScanInput();
    statusBar->update();

    // Respond to user input
    const u32 kDown = hidKeysDown();

    if (kDown & KEY_START) exitApplication();

    time += 0.026f;
}

void HexGuess::draw() const
{
    // ==================== TOP ========================
    C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
    C2D_TargetClear(top, COLOR_BACKGROUND);
    C2D_SceneBegin(top);

    // Draw
    statusBar->draw();

    C2D_DrawText(txtTarget, C2D_AlignCenter, 37.5f, 35, 0, 0.7f, 0.7f);
    C2D_DrawText(txtGuess, C2D_AlignCenter, 37.5f, 135, 0, 0.7f, 0.7f);

    C2D_DrawRectSolid(10, 55, 0, 55, 55, COLOR_PINK);
    C2D_DrawRectSolid(10, 155, 0, 55, 55, COLOR_BLUE);

    C2D_DrawRectSolid(135, 40, 0, 40, 40, COLOR_BLUE);
    C2D_DrawRectSolid(135, 110, 0, 40, 40, COLOR_BLUE);
    C2D_DrawRectSolid(135, 180, 0, 40, 40, COLOR_BLUE);

    C2D_DrawRectSolid(270, 40, 0, 40, 40, COLOR_BLUE);
    C2D_DrawRectSolid(270, 110, 0, 40, 40, COLOR_BLUE);
    C2D_DrawRectSolid(270, 180, 0, 40, 40, COLOR_BLUE);


    // ==================== BOTTOM ========================
    C2D_TargetClear(bottom, COLOR_DARK_PURPLE);
    C2D_SceneBegin(bottom);

    //Draw
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

    // ==================== END FRAME ========================
    C3D_FrameEnd(0);
}
