#include "MandelbrotSet.hpp"

MandelbrotSet::MandelbrotSet(sf::RenderWindow& win, Input& input) :
    win{ win },
    input{ input },
    view{ 0.0, 0.0, static_cast<long double>(win.getSize().x), static_cast<long double>(win.getSize().y) },
    vertices{ sf::PrimitiveType::Points, win.getSize().x * win.getSize().y }
{
    auto k = 0u;
    for (auto i = 0u; i < win.getSize().x; i++)
        for (auto j = 0u; j < win.getSize().y; j++)
            vertices[k++].position = sf::Vector2f{ static_cast<float>(i), static_cast<float>(j) };
}

void MandelbrotSet::update()
{
    if (input.isKeyReleased(sf::Keyboard::Add))
    {
        view.left += view.width / 4;
        view.top += view.height / 4;
        view.width /= 2;
        view.height /= 2;
        needs_update = true;
    }
    else if (input.isKeyPressed(sf::Keyboard::Subtract))
    {
        view.left -= view.width / 2;
        view.top -= view.height / 2;
        view.width *= 2;
        view.height *= 2;
        needs_update = true;
    }
    else if (input.isKeyPressed(sf::Keyboard::W))
    {
        view.top -= view.height / 2;
        needs_update = true;
    }
    else if (input.isKeyPressed(sf::Keyboard::S))
    {
        view.top += view.height / 2;
        needs_update = true;
    }
    else if (input.isKeyPressed(sf::Keyboard::A))
    {
        view.left -= view.width / 2;
        needs_update = true;
    }
    else if (input.isKeyPressed(sf::Keyboard::D))
    {
        view.left += view.width / 2;
        needs_update = true;
    }

    if (!needs_update)
        return;

    needs_update = false;

    const int max_iterations = 1000;

    const long double win_size_x = win.getSize().x;
    const long double win_size_y = win.getSize().y;

    const long double domain_x_1 = -2.0;
    const long double domain_x_s = 2.47;

    const long double domain_y_1 = -1.12;
    const long double domain_y_s = 2.24;

    const auto increment_x = view.width / static_cast<float>(win_size_x);
    const auto increment_y = view.height / static_cast<float>(win_size_y);

    auto left = view.left;
    auto top = view.top;

    auto k = 0u;
    unsigned int iterations;
    long double x, y;
    long double x0, y0;
    long double x2, y2;

    for (auto i = 0u; i < win.getSize().x; i++)
    {
        top = view.top;
        for (auto j = 0u; j < win.getSize().y; j++)
        {
            x0 = domain_x_1 + static_cast<long double>(left) / win_size_x * domain_x_s;
            y0 = domain_y_1 + static_cast<long double>(top) / win_size_y * domain_y_s;
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
            vertices[k].color = iterations == max_iterations ? sf::Color::Black : sf::Color::White;
            k++;
            top += increment_y;
        }
        left += increment_x;
    }
}

void MandelbrotSet::render()
{
    win.draw(vertices);
}
