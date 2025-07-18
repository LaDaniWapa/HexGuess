//
// Created by daniela on 7/17/25.
//

#include "helper.h"
#include <3ds.h>

uint32_t getTodaysColor()
{
    // Try the original function first
    u64 timeMs = osGetTime();
    const time_t rawTime = (timeMs - EPOCH_MARGIN) / 1000; // ms to seconds
    const tm* dt = localtime(&rawTime); // expects seconds

    const int dateNumber = dt->tm_mday * 1000000 + (dt->tm_mon + 1) * 10000 + dt->tm_year + 1900;

    return dateToColor(dateNumber);
}