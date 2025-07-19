//
// Created by daniela on 7/18/25.
//

#ifndef COLORS_H
#define COLORS_H

#define ABGR(rgb) (0xFF000000 | (((rgb) & 0x0000FF) << 16) | ((rgb) & 0x00FF00) | (((rgb) & 0xFF0000) >> 16))

#define COLOR_BACKGROUND 0xFFD5D5D5
#define COLOR_SURFACE 0xFFF5F5F5

#define COLOR_BLACK 0xFF000000
#define COLOR_PINK ABGR(0xFF8FAB)
#define COLOR_BLUE ABGR(0x00B4D8)
#define COLOR_GREY ABGR(0x)

#define COLOR_DARK_PURPLE ABGR(0x4D2699)

#define COLOR_LIGHT_PURPLE ABGR(0xE66699)

#endif //COLORS_H
