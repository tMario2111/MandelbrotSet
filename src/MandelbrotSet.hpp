#pragma once

#include "Input.hpp"

#include <SFML/Graphics.hpp>

#include <imgui.h>

#define _USE_MATH_DEFINES
#include <cmath>
#include <string>
#include <thread>
#include <vector>

using f_type = double;

class MandelbrotSet
{
public:
    MandelbrotSet(sf::RenderWindow& win, Input& input);
    void onResize();
    void update();
    void render();
private:
    sf::RenderWindow& win;
    Input& input;

    sf::Rect<f_type> view;

    const f_type default_domain_x_1 = -2.0;
    const f_type default_domain_x_s = 2.47;
    const f_type default_domain_y_1 = -1.12;
    const f_type default_domain_y_s = 2.24;

    sf::VertexArray vertices;
    std::vector<unsigned int> points;

    bool needs_update = true;
    int max_iterations = 100;
    int thread_count = 1;

    void gui();
    void control();
    void fractal(unsigned int c_thread);
};
