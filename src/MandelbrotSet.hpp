#pragma once

#include "Input.hpp"

#include <SFML/Graphics.hpp>

#include <imgui.h>

#define _USE_MATH_DEFINES
#include <cmath>
#include <string>
#include <thread>
#include <vector>

class MandelbrotSet
{
public:
    MandelbrotSet(sf::RenderWindow& win, Input& input);
    void update();
    void render();
private:
    sf::RenderWindow& win;
    Input& input;
    sf::Rect<double> view;
    sf::VertexArray vertices;
    std::vector<unsigned int> points;
    bool needs_update = true;
    int max_iterations = 100;
    int thread_count = 1;
    void gui();
    void control();
    void fractal(unsigned int c_thread);
};
