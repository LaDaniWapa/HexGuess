//
// Created by daniela on 7/19/25.
//

#ifndef HEXGUESS_H
#define HEXGUESS_H

#include "StatusBar.h"

class HexGuess
{
    float time;

    C3D_RenderTarget *top = nullptr, *bottom = nullptr;
    C2D_TextBuf_s *targetBuffer = nullptr, *guessBuffer = nullptr;
    C2D_Text *txtTarget = nullptr, *txtGuess = nullptr;
    StatusBar *statusBar = nullptr;

    void init();
    static void exitApplication();
public:
    HexGuess();  // Implement to initialize pointers
    ~HexGuess(); // Implement to free resources

    // Disable copying (common with raw pointers)
    HexGuess(const HexGuess&) = delete;
    HexGuess& operator=(const HexGuess&) = delete;


    void update();
    void draw() const;
};

#endif //HEXGUESS_H
