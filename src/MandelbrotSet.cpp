#include "MandelbrotSet.hpp"

MandelbrotSet::MandelbrotSet(sf::RenderWindow& win, Input& input) :
    win{ win },
    input{ input },
    view{ 0.f, 0.f, static_cast<float>(win.getSize().x), static_cast<float>(win.getSize().y) },
    vertices{ sf::PrimitiveType::Points, win.getSize().x * win.getSize().y }
{
    points.resize(win.getSize().x * win.getSize().y);

    auto k = 0u;
    for (auto i = 0u; i < win.getSize().x; i++)
        for (auto j = 0u; j < win.getSize().y; j++)
            vertices[k++].position = sf::Vector2f{ static_cast<float>(i), static_cast<float>(j) };
}

void MandelbrotSet::control()
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
        view.top -= view.height / 4;
        needs_update = true;
    }
    else if (input.isKeyPressed(sf::Keyboard::S))
    {
        view.top += view.height / 4;
        needs_update = true;
    }
    else if (input.isKeyPressed(sf::Keyboard::A))
    {
        view.left -= view.width / 4;
        needs_update = true;
    }
    else if (input.isKeyPressed(sf::Keyboard::D))
    {
        view.left += view.width / 4;
        needs_update = true;
    }
}

void MandelbrotSet::fractal(unsigned int c_thread)
{
    const unsigned int column_lenght = win.getSize().x / thread_count;

    const float win_size_x = win.getSize().x;
    const float win_size_y = win.getSize().y;

    const auto domain_x_1 = -2.0f;
    const auto domain_x_s = 2.47f;

    const auto domain_y_1 = -1.12f;
    const auto domain_y_s = 2.24f;

    const auto increment_x = view.width / static_cast<float>(win_size_x);
    const auto increment_y = view.height / static_cast<float>(win_size_y);

    auto left = view.left + c_thread * (view.width / thread_count);
    auto top = view.top;

    unsigned int iterations;
    float x, y;
    float x0, y0;
    float x2, y2;
    
    const auto a = 0.1f;

    unsigned int i, j;
    for (i = c_thread * column_lenght; i < (c_thread + 1) * column_lenght; i++)
    {
        top = view.top;
        for (j = 0; j < win.getSize().y; j++)
        {
            x0 = domain_x_1 + left / win_size_x * domain_x_s;
            y0 = domain_y_1 + top / win_size_y * domain_y_s;
            x = 0.f, y = 0.f;
            x2 = 0.f, y2 = 0.f;
            iterations = 0;
            while (x2 + y2 <= 4.f && iterations < max_iterations)
            {
                y = 2 * x * y + y0;
                x = x2 - y2 + x0;
                x2 = x * x;
                y2 = y * y;
                iterations++;
            }
            points[i * win.getSize().y + j] = iterations;
            top += increment_y;
        }
        left += increment_x;
    }
}

void MandelbrotSet::update()
{
    control();

    if (!needs_update)
        return;
    needs_update = false;

    std::vector<std::thread> threads;
    threads.resize(thread_count);

    for (auto i = 0u; i < thread_count; i++)
        threads[i] = std::thread{ &MandelbrotSet::fractal, this, i };

    for (auto& thread : threads)
        thread.join();
}

void MandelbrotSet::render()
{
    const auto a = 0.1f;
    unsigned int k;
    float it;
    unsigned int i, j;
    for (i = 0; i < win.getSize().x; i++)
        for (j = 0; j < win.getSize().y; j++)
        {
            k = i * win.getSize().y + j;
            it = static_cast<float>(points[k]);
            vertices[k].color.r = static_cast<sf::Uint8>(255.f * (0.5f * sinf(it * a) + 0.5f));
            vertices[k].color.g = static_cast<sf::Uint8>(255.f * (0.5f * sinf(it * a + 2.094f) + 0.5f));
            vertices[k].color.b = static_cast<sf::Uint8>(255.f * (0.5f * sinf(it * a + 4.188f) + 0.5f));
        }
    win.draw(vertices);
}
