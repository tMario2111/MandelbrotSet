#pragma once

#include "Input.hpp"
#include "MandelbrotSet.hpp"

#include <SFML/Graphics.hpp>

#include <imgui.h>
#include <imgui-SFML.h>

#include <memory>

class App
{
public:
    App();
    sf::RenderWindow win;
    sf::Time dt;
private:
    sf::Clock win_clock;
    Input input;
    std::unique_ptr<MandelbrotSet> set;
    void setupWin();
    void setupGui();
    void winEvents();
    void run();
};
