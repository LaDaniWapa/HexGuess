//
// Created by daniela on 7/19/25.
//

#include "StatusBar.h"
#include "Colors.h"
#include "Helper.h"

StatusBar::StatusBar()
{
    titleBuffer = C2D_TextBufNew(20);
    timeBuffer = C2D_TextBufNew(128);
    batteryBuffer = C2D_TextBufNew(128);

    txtTitle = new C2D_Text;
    txtTime = new C2D_Text;
    txtBattery = new C2D_Text;

    C2D_TextParse(txtTitle, titleBuffer, "HexGuess");
    C2D_TextParse(txtTime, timeBuffer, "00:00");
    C2D_TextParse(txtBattery, batteryBuffer, "0%");
}

StatusBar::~StatusBar()
{
    if (titleBuffer) C2D_TextBufDelete(titleBuffer);
    if (timeBuffer) C2D_TextBufDelete(timeBuffer);
    if (batteryBuffer) C2D_TextBufDelete(batteryBuffer);
}

void StatusBar::update() const
{
    static u64 lastUpdateTime = 0;
    const u64 currentTime = osGetTime();

    // Should status update?
    if (currentTime - lastUpdateTime >= 5000) // Every 5s
    {
        getCurrentTime(currentTime);
        getBatteryLevel();
    }
}

void StatusBar::draw() const
{
    C2D_DrawRectSolid(0, 0, 0, 400, 20, COLOR_SURFACE);
    C2D_DrawLine(0, 21, COLOR_BLACK, 400, 21, COLOR_BLACK, 1, 1);

    C2D_DrawText(txtTime, 0, 10, 2, 0, 0.5, 0.5);
    C2D_DrawText(txtTitle, C2D_AlignCenter, 200, 0, 0, .7f, .7f);
    C2D_DrawText(txtBattery, C2D_AlignRight, 390, 2, 0, 0.5, 0.5);
}


void StatusBar::getCurrentTime(const u64& currentTime) const
{
    const time_t rawTime = (currentTime - EPOCH_MARGIN) / 1000;
    const tm* dt = localtime(&rawTime);

    char strTime[16];
    snprintf(strTime, sizeof(strTime), "%02d:%02d", dt->tm_hour, dt->tm_min);

    // Completely recreate the text
    C2D_TextBufClear(timeBuffer);
    C2D_TextParse(txtTime, timeBuffer, strTime);
}

void StatusBar::getBatteryLevel() const
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
    C2D_TextBufClear(batteryBuffer);
    C2D_TextParse(txtBattery, batteryBuffer, strLevel);
}
