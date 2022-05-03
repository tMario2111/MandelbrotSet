#pragma once

#include "Input.hpp"

#include <SFML/Graphics.hpp>

#include <cmath>
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
    sf::FloatRect view;
    sf::VertexArray vertices;
    std::vector<unsigned int> points;
    bool needs_update = true;
    unsigned int max_iterations = 1000;
    unsigned int thread_count = 16;
    void control();
    void fractal(unsigned int c_thread);
};
