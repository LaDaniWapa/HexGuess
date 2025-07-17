#include <stdio.h>
#include <string.h>
#include <3ds.h>
#include <3ds/os.h>
#include <3ds/console.h>
#include <ctime>
#include "shake128.h"

#define EPOCH_MARGIN 2208988800000ULL

int main()
{
    gfxInitDefault();
    consoleInit(GFX_TOP, nullptr);

    printf("HexGuess\n");

    auto timeref = osGetTimeRef(); // NTP Epoch
    const time_t rawtime = (timeref.value_ms - EPOCH_MARGIN) / 1000; // ms to seconds
    const tm* dt = localtime(&rawtime); // spects seconds

    const int date_number = dt->tm_mday * 1000000 + (dt->tm_mon + 1) * 10000 + dt->tm_year + 1900;
    const uint32_t hexcolor = date_to_color(date_number);

    printf("%d => #%X\n", date_number, hexcolor);

    // Main loop
    while (aptMainLoop())
    {
        gspWaitForVBlank();
        gfxSwapBuffers();
        hidScanInput();

        // Your code goes here
        u32 kDown = hidKeysDown();
        if (kDown & KEY_START)
            break; // break in order to return to hbmenu
    }

    gfxExit();
    return 0;
}
