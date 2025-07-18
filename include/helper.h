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
uint32_t getTodaysColor();

#endif //HELPER_H