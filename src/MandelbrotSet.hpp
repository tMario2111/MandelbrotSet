#pragma once

#include "Input.hpp"

#include <SFML/Graphics.hpp>
#include <imgui.h>
#include <nlohmann/json.hpp>

#include <algorithm>
#define _USE_MATH_DEFINES
#include <cmath>
#include <fstream>
#include <iomanip>
#include <string>
#include <thread>
#include <vector>

using f_type = double;

enum ColorFunction
{
    Sin,
    Cos,
    Tan
};

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

    f_type domain_x_1, domain_x_s;
    f_type domain_y_1, domain_y_s;

    sf::VertexArray vertices;
    std::vector<unsigned int> points;

    f_type r_modifier = 0.0;
    ColorFunction r_func = ColorFunction::Sin;

    f_type g_modifier = M_PI / 2;
    ColorFunction g_func = ColorFunction::Sin;

    f_type b_modifier = M_PI;
    ColorFunction b_func = ColorFunction::Sin;

    bool needs_update = true;
    int max_iterations = 100;
    int thread_count = 1;
    char screenshot_name[101] = "screenshot";

    void gui();
    void takeScreenshot();
    void control();
    void fractal(unsigned int c_thread);
};
