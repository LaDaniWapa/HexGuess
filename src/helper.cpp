//
// Created by daniela on 7/17/25.
//

#include "helper.h"
#include <3ds.h>
#include <cstring>

// Global buffers because text used to disappear
static C2D_TextBuf g_timeBuffer = nullptr;
static C2D_TextBuf g_BatteryBuffer = nullptr;
static bool g_InitBuffers = false;

u32 getTodaysColor()
{
    // Try the original function first
    u64 timeMs = osGetTime();
    const time_t rawTime = (timeMs - EPOCH_MARGIN) / 1000; // ms to seconds
    const tm* dt = localtime(&rawTime); // expects seconds

    const int dateNumber = dt->tm_mday * 1000000 + (dt->tm_mon + 1) * 10000 + dt->tm_year + 1900;

    return dateToColor(dateNumber);
}

void getCurrentTime(C2D_Text* txtTime, const u64& currentTime)
{
    const time_t rawTime = (currentTime - EPOCH_MARGIN) / 1000;
    const tm* dt = localtime(&rawTime);

    char strTime[16];
    snprintf(strTime, sizeof(strTime), "%02d:%02d", dt->tm_hour, dt->tm_min);

    // Completely recreate the text
    C2D_TextBufClear(g_timeBuffer);
    C2D_TextParse(txtTime, g_timeBuffer, strTime);
}

void getBatteryLevel(C2D_Text* txtBattery)
{
    u8 level = 0;
    Result ret = 0;
    char strLevel[16];

    ret = MCUHWC_GetBatteryLevel(&level);

    if (R_SUCCEEDED(ret))
        snprintf(strLevel, sizeof(strLevel), "%d%%", level);
    else
        snprintf(strLevel, sizeof(strLevel), "ERROR %%");

    // Completely recreate the text
    C2D_TextBufClear(g_BatteryBuffer);
    C2D_TextParse(txtBattery, g_BatteryBuffer, strLevel);
}

void updateStatusBar(C2D_Text* txtTime, C2D_Text* txtBattery)
{
    // Initialize buffers only once
    if (!g_InitBuffers)
    {
        g_timeBuffer = C2D_TextBufNew(256);
        g_BatteryBuffer = C2D_TextBufNew(256);
        g_InitBuffers = true;
    }

    static u64 lastUpdateTime = 0;
    const u64 currentTime = osGetTime();

    // Should status update?
    if (currentTime - lastUpdateTime >= 3000) // Every 30s
    {
        getCurrentTime(txtTime, currentTime);
        getBatteryLevel(txtBattery);
    }
}
