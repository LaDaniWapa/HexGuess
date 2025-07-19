//
// Created by daniela on 7/17/25.
//

#ifndef HELPER_H
#define HELPER_H

#define EPOCH_MARGIN 2208988800000ULL

#include <3ds/types.h>


/**
 * Calculates the color of the day using the shake128 algorithm on the date
 *
 * @return Color of the day
 */
u32 getTodaysColor();

#endif //HELPER_H
