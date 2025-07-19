#include <cstdio>
#include <3ds.h>
#include <citro2d.h>
#include "HexGuess.h"


int main()
{
    auto* hexGuess = new HexGuess();

    // Main loop
    while (aptMainLoop())
    {
        hexGuess->update();
        hexGuess->draw();
    }

    return 0;
}
