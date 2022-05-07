#pragma once

#include "Input.hpp"
#include "MandelbrotSet.hpp"

#include <SFML/Graphics.hpp>

#include <imgui.h>
#include <imgui-SFML.h>

#include <memory>
#include <string>

class App
{
public:
    App();
    sf::RenderWindow win;
    sf::Time dt;
private:
    const std::string title = "Mandelbrot Set";
    const unsigned int framerate_limit = 30;
    sf::VideoMode prevMove{ 800, 800 };
    bool fullscreen = false;
    sf::Clock win_clock;
    Input input;
    std::unique_ptr<MandelbrotSet> set;
    void setupWin();
    void setupGui();
    void resizeEvent(const sf::Event& event);
    void toggleFullscreen();
    void winEvents();
    void run();
};
