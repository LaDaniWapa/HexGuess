//
// Created by daniela on 7/19/25.
//

#ifndef STATUSBAR_H
#define STATUSBAR_H

#include <citro2d.h>

class StatusBar
{
    C2D_TextBuf_s *timeBuffer = nullptr, *batteryBuffer = nullptr, *titleBuffer = nullptr;
    C2D_Text *txtTime = nullptr, *txtBattery = nullptr, *txtTitle = nullptr;

    void getBatteryLevel() const;
    void getCurrentTime(const u64& currentTime) const;

public:
    StatusBar();
    ~StatusBar();

    // Disable copying (common with raw pointers)
    StatusBar(const StatusBar&) = delete;
    StatusBar& operator=(const StatusBar&) = delete;

    void update() const;
    void draw() const;
};

#endif //STATUSBAR_H
