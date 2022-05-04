#include "MandelbrotSet.hpp"

MandelbrotSet::MandelbrotSet(sf::RenderWindow& win, Input& input) :
    win{ win },
    input{ input },
    view{ 0.0, 0.0, static_cast<double>(win.getSize().x), static_cast<double>(win.getSize().y) },
    vertices{ sf::PrimitiveType::Points, win.getSize().x * win.getSize().y }
{
    points.resize(win.getSize().x * win.getSize().y);

    auto k = 0u;
    for (auto i = 0u; i < win.getSize().x; i++)
        for (auto j = 0u; j < win.getSize().y; j++)
            vertices[k++].position = sf::Vector2f{ static_cast<float>(i), static_cast<float>(j) };
}

void MandelbrotSet::gui()
{
    ImGui::Begin("SETTINGS");
    ImGui::SliderInt("Iterations", &max_iterations, 100, 1000);
    ImGui::SliderInt("Threads", &thread_count, 1, 32);
    ImGui::End();
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

    const double win_size_x = win.getSize().x;
    const double win_size_y = win.getSize().y;

    const double domain_x_1 = -2.0;
    const double domain_x_s = 2.47;

    const double domain_y_1 = -1.12;
    const double domain_y_s = 2.24;

    const auto increment_x = view.width / static_cast<double>(win_size_x);
    const auto increment_y = view.height / static_cast<double>(win_size_y);

    auto left = view.left + c_thread * (view.width / thread_count);
    auto top = view.top;

    unsigned int iterations;
    double x, y;
    double x0, y0;
    double x2, y2;
    
    const double a = 0.0;

    unsigned int i, j;
    for (i = c_thread * column_lenght; i < (c_thread + 1) * column_lenght; i++)
    {
        top = view.top;
        for (j = 0; j < win.getSize().y; j++)
        {
            x0 = domain_x_1 + left / win_size_x * domain_x_s;
            y0 = domain_y_1 + top / win_size_y * domain_y_s;
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
            points[i * win.getSize().y + j] = iterations;
            top += increment_y;
        }
        left += increment_x;
    }
}

void MandelbrotSet::update()
{
    gui();
    control();

    if (!needs_update)
        return;

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
    double it;
    unsigned int i, j;
    if (needs_update)
    {
        needs_update = false;
        for (i = 0; i < win.getSize().x; i++)
            for (j = 0; j < win.getSize().y; j++)
            {
                k = i * win.getSize().y + j;
                it = static_cast<double>(points[k]);
                vertices[k].color.r = static_cast<sf::Uint8>(255.0 * (0.5 * sin(it * a) + 0.5));
                vertices[k].color.g = static_cast<sf::Uint8>(255.0 * (0.5 * sin(it * a + M_PI_4) + 0.5)); // Colors might change
                vertices[k].color.b = static_cast<sf::Uint8>(255.0 * (0.5 * sin(it * a + M_PI) + 0.5));
            }
    }
    win.draw(vertices);
}
