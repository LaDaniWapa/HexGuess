//
// Created by daniela on 7/17/25.
//

#include "Helper.h"
#include "Shake128.h"
#include <3ds.h>

u32 dateToColor(int const date)
{
    uint32_t color = 0x0;
    constexpr u8 bytes = 3;

    vector<u8> in(sizeof(date));
    memcpy(in.data(), &date, sizeof(date));

    vector<u8> out;

    shake128(in, bytes, out);

    for (int i = 0; i < bytes; i++)
    {
        color = color << 8; // 8bits = 2 bytes = 2 numbers
        color += out[i];
    }

    return color + (0xff << 24);;
}

u32 getTodaysColor()
{
    u64 timeMs = osGetTime();
    const time_t rawTime = (timeMs - EPOCH_MARGIN) / 1000; // ms to seconds
    const tm* dt = localtime(&rawTime); // expects seconds

    const int dateNumber = dt->tm_mday * 1000000 + (dt->tm_mon + 1) * 10000 + dt->tm_year + 1900;

    return dateToColor(dateNumber);
}
