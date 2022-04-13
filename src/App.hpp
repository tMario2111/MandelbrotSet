#pragma once

#include <SFML/Graphics.hpp>

#include "MandelbrotSet.hpp"

class App
{
public:
    App();
    sf::RenderWindow win;
private:
    MandelbrotSet set;
    void setupWin();
    void winEvents();
    void run();
};
