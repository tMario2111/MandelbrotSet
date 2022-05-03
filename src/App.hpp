#pragma once

#include "Input.hpp"
#include "MandelbrotSet.hpp"

#include <SFML/Graphics.hpp>

#include <memory>

class App
{
public:
    App();
    sf::RenderWindow win;
    sf::Font font;
private:
    Input input;
    std::unique_ptr<MandelbrotSet> set;
    void setupWin();
    void loadAssets();
    void winEvents();
    void run();
};
