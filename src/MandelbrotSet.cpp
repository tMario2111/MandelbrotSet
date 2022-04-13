#include "MandelbrotSet.hpp"

MandelbrotSet::MandelbrotSet(sf::RenderWindow& win) :
    win{ win },
    vertices{ sf::PrimitiveType::Points, win.getSize().x * win.getSize().y }
{
    auto k = 0u;
    for (auto i = 0u; i < win.getSize().x; i++)
        for (auto j = 0u; j < win.getSize().y; j++)
            vertices[k++].position = sf::Vector2f{ static_cast<float>(i), static_cast<float>(j) };
}

void MandelbrotSet::update()
{
    if (!needs_update)
        return;

    needs_update = false;

    const int max_iterations = 100;

    const long double win_size_x = win.getSize().x;
    const long double win_size_y = win.getSize().y;

    const long double domain_x_1 = -2.0;
    const long double domain_x_s = 2.47;

    const long double domain_y_1 = -1.12;
    const long double domain_y_s = 2.24;

    auto k = 0u;
    unsigned int iterations;
    long double x, y;
    long double x0, y0;
    long double x2, y2;

    for (auto i = 0u; i < win.getSize().x; i++)
        for (auto j = 0u; j < win.getSize().y; j++)
        {
            x0 = domain_x_1 + static_cast<long double>(i) / win_size_x * domain_x_s;
            y0 = domain_y_1 + static_cast<long double>(j) / win_size_y * domain_y_s;
            x = 0.0, y = 0.0;
            x2 = 0.0, y2 = 0.0;
            iterations = 0;
            while (x2 + y2 <= 4.0 && iterations < max_iterations)
            {
                y = 2 * x * y + y0;
                x = x2 - y2 + x0;
                x2 = x * x;
                y2 = y * y;
                iterations++;
            }
            vertices[k++].color = iterations == max_iterations ? sf::Color::Black : sf::Color::White;
        }
}

void MandelbrotSet::render()
{
    win.draw(vertices);
}
