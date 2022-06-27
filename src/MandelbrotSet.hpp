#pragma once

#include "Input.hpp"

#include <SFML/Graphics.hpp>
#include <imgui.h>
#include <nlohmann/json.hpp>

#include <algorithm>
#define _USE_MATH_DEFINES // TODO: Remove
#include <cmath>
#include <filesystem>
#include <fstream>
#include <iomanip>
#include <memory>
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

    sf::Vector2<f_type> mandelbrot_coords;
    f_type zoom;

    struct Theme
    {
        std::string name;

        f_type r_modifier;
        ColorFunction r_func;

        f_type g_modifier;
        ColorFunction g_func;

        f_type b_modifier;
        ColorFunction b_func;
    };
    std::vector<std::unique_ptr<Theme>> themes;
    Theme* selected_theme = nullptr;

    bool needs_update = true;
    int max_iterations = 100;
    int thread_count = 1;
    char screenshot_name[101] = "screenshot";

    sf::Vector2<f_type> mapWinCoordsToMandelbrot(sf::Vector2<f_type> a);
    sf::Vector2<f_type> mapMandelbrotCoordsToWin(sf::Vector2<f_type> a);
    sf::Vector2<f_type> getCursorPosition();
    f_type getZoom();

    void loadThemes();
    void gui();
    void takeScreenshot();
    void control();
    void fractal(unsigned int c_thread);
};
