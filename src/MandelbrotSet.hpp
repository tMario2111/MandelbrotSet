#pragma once

#include "Input.hpp"

#include <SFML/Graphics.hpp>

#include <cmath>
#include <string>
#include <thread>
#include <vector>

class MandelbrotSet
{
public:
    MandelbrotSet(sf::RenderWindow& win, Input& input, sf::Font& font);
    void update();
    void render();
private:
    sf::RenderWindow& win;
    Input& input;
    sf::Font& font;
    sf::Rect<double> view;
    sf::VertexArray vertices;
    std::vector<unsigned int> points;
    bool needs_update = true;
    unsigned int max_iterations = 100;
    unsigned int thread_count = 1;
    sf::Text iterations_text;
    sf::Text threads_text;
    void setupText();
    void control();
    void fractal(unsigned int c_thread);
};
