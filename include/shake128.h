#ifndef SHAKE128
#define SHAKE128

#include <algorithm>
#include <stdint.h>
#include <string.h>
#include <bitset>
#include <fstream>
#include <vector>

using namespace std;

void shake128(vector<uint8_t> in, int d, vector<uint8_t>& out);

/**
 * Calculates a color from a date
 *
 * @param date number with "ddmmyyyy" date format
 * @return a color for that date
 */
uint32_t dateToColor(int date);

#endif
