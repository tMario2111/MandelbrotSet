#pragma once

#include "Input.hpp"
#include "MandelbrotSet.hpp"

#include <SFML/Graphics.hpp>

class App
{
public:
    App();
    sf::RenderWindow win;
private:
    Input input;
    MandelbrotSet set;
    void setupWin();
    void winEvents();
    void run();
};
