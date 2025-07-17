#include <algorithm>
#include <stdint.h>
#include <string.h>
#include <bitset>
#include <fstream>
#include <vector>
using namespace std;

void shake128(vector<uint8_t> in, int d, vector<uint8_t>& out);
uint32_t date_to_color(int date);
