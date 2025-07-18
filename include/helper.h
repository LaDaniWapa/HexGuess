//
// Created by daniela on 7/17/25.
//

#ifndef HELPER_H
#define HELPER_H

#include <cstdint>
#include <ctime>
#include <3ds.h>
#include <citro2d.h>
#include "shake128.h"

#define EPOCH_MARGIN 2208988800000ULL

/**
 * Calculates the color of the day using the shake128 algorithm on the date
 *
 * @return Color of the day
 */
u32 getTodaysColor();

/**
 * Updates the info in the status bar once every 30s
 *
 * @param txtTime Text to be updated with the current time
 * @param txtBattery Text to be updated with the current battery level
 */
void updateStatusBar(C2D_Text* txtTime, C2D_Text* txtBattery);

#endif //HELPER_H
