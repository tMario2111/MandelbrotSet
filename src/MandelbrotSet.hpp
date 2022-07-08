#pragma once

#include "HintText.hpp"
#include "Input.hpp"
#include "LocationManager.hpp"
#include "ThemeManager.hpp"

#include <SFML/Graphics.hpp>
#include <imgui.h>
#include <nlohmann/json.hpp>

#include <algorithm>
#include <cmath>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <optional>
#include <string>
#include <thread>
#include <vector>

using f_type = double;

class MandelbrotSet
{
public:
    MandelbrotSet(sf::RenderWindow& win, Input& input, sf::Font& font);
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

    sf::Clock render_clock;
    sf::Time render_time;

    sf::VertexArray vertices;

    HintText hint_text;
    ThemeManager themes;
    LocationManager locations;

    sf::Vector2<f_type> mandelbrot_coords;
    f_type zoom;

    std::vector<std::thread> threads;

    bool needs_update = true;
    int max_iterations = 100;
    int thread_count = 1;
    char screenshot_name[101] = "screenshot";
    bool black_accent_color = false;

    sf::Vector2<f_type> mapWinCoordsToMandelbrot(sf::Vector2<f_type> a);
    sf::Vector2<f_type> mapMandelbrotCoordsToWin(sf::Vector2<f_type> a);
    sf::Vector2<f_type> getCursorPosition();
    f_type getZoom();
    void setLocation(const Location location);

    void gui();
    void takeScreenshot();
    void control();
    void fractal(unsigned int c_thread);
};
