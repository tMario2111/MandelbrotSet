#include "MandelbrotSet.hpp"

MandelbrotSet::MandelbrotSet(sf::RenderWindow& win, Input& input) :
    win{ win },
    input{ input },
    vertices{ sf::PrimitiveType::Points }
{
    onResize();
}

void MandelbrotSet::onResize()
{
    needs_update = true;

    points.resize(win.getSize().x * win.getSize().y);
    vertices.resize(points.size());

    view.left = static_cast<f_type>(win.getView().getCenter().x - win.getView().getSize().x / 2);
    view.top = static_cast<f_type>(win.getView().getCenter().y - win.getView().getSize().y / 2);
    view.width = static_cast<f_type>(win.getSize().x);
    view.height = static_cast<f_type>(win.getSize().y);

    auto k = 0u;
    for (auto i = 0u; i < win.getSize().x; i++)
        for (auto j = 0u; j < win.getSize().y; j++)
            vertices[k++].position = sf::Vector2f{ static_cast<float>(view.left) + static_cast<float>(i), 
            static_cast<float>(view.top) + static_cast<float>(j) };
}

void MandelbrotSet::gui()
{
    const auto spacing = []{ ImGui::Dummy(ImVec2{ 0.f, 10.f }); };
    const char* functions[] = { "sin", "cos", "tan" };
    ImGui::Begin("SETTINGS");

    if (ImGui::InputDouble("Red modifier", &r_modifier) || 
        ImGui::ListBox("Red function", reinterpret_cast<int*>(&r_func), functions, 3))
        needs_update = true;

    spacing();

    if (ImGui::InputDouble("Green modifier", &g_modifier) || 
        ImGui::ListBox("Green function", reinterpret_cast<int*>(&g_func), functions, 3))
        needs_update = true;

    spacing();

    if (ImGui::InputDouble("Blue modifier", &b_modifier) || 
        ImGui::ListBox("Blue function", reinterpret_cast<int*>(&b_func), functions, 3))
        needs_update = true;

    spacing();

    if (ImGui::SliderInt("Iterations", &max_iterations, 100, 1000))
        needs_update = true;

    spacing();

    ImGui::SliderInt("Threads", &thread_count, 1, 32);

    spacing();

    ImGui::InputText("Filename", screenshot_name.data(), 100);
    ImGui::SameLine();
    if (ImGui::Button("Save"))
        takeScreenshot();

    ImGui::End();
}

void MandelbrotSet::takeScreenshot()
{
    sf::RenderTexture texture{};
    texture.create(win.getSize().x, win.getSize().y);
    texture.setView(win.getView());
    texture.draw(vertices);
    texture.display();
    texture.getTexture().copyToImage().saveToFile(screenshot_name + ".png");
}

void MandelbrotSet::control()
{
    if (input.isKeyPressed(sf::Keyboard::Add))
    {
        view.left += view.width / 4.0;
        view.top += view.height / 4.0;
        view.width /= 2.0;
        view.height /= 2.0;
        needs_update = true;
    }
    else if (input.isKeyPressed(sf::Keyboard::Subtract))
    {
        view.left -= view.width / 2.0;
        view.top -= view.height / 2.0;
        view.width *= 2.0;
        view.height *= 2.0;
        needs_update = true;
    }
    else if (input.isKeyPressed(sf::Keyboard::W))
    {
        view.top -= view.height / 4.0;
        needs_update = true;
    }
    else if (input.isKeyPressed(sf::Keyboard::S))
    {
        view.top += view.height / 4.0;
        needs_update = true;
    }
    else if (input.isKeyPressed(sf::Keyboard::A))
    {
        view.left -= view.width / 4.0;
        needs_update = true;
    }
    else if (input.isKeyPressed(sf::Keyboard::D))
    {
        view.left += view.width / 4.0;
        needs_update = true;
    }
}

void MandelbrotSet::fractal(unsigned int c_thread)
{
    const unsigned int column_lenght = win.getSize().x / thread_count;

    const f_type win_size_x = win.getSize().x;
    const f_type win_size_y = win.getSize().y;

    const auto min_size = std::min(win_size_x, win_size_y);
    const auto domain_x_increment = (win_size_x / min_size - 1) * default_domain_x_s / 2.0;
    const auto domain_y_increment = (win_size_y / min_size - 1) * default_domain_y_s / 2.0;

    const f_type domain_x_1 = default_domain_x_1 + domain_x_increment;
    const f_type domain_x_s = default_domain_x_s + domain_x_increment;

    const f_type domain_y_1 = default_domain_y_1 + domain_y_increment;
    const f_type domain_y_s = default_domain_y_s + domain_y_increment;

    const auto increment_x = view.width / static_cast<f_type>(win_size_x);
    const auto increment_y = view.height / static_cast<f_type>(win_size_y);

    auto left = view.left + c_thread * (view.width / thread_count);
    auto top = view.top;

    unsigned int iterations;
    f_type x, y;
    f_type x0, y0;
    f_type x2, y2;
    
    const f_type a = 0.0;

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
    f_type it;
    unsigned int i, j;
    f_type r_aux, g_aux, b_aux;
    if (needs_update)
    {
        needs_update = false;
        for (i = 0; i < win.getSize().x; i++)
            for (j = 0; j < win.getSize().y; j++)
            {
                k = i * win.getSize().y + j;
                it = static_cast<f_type>(points[k]);

                if (r_func == ColorFunction::Sin)
                    r_aux = 0.5 * sin(it * a + r_modifier);
                else if (r_func == ColorFunction::Cos)
                    r_aux = 0.5 * cos(it * a + r_modifier);
                else 
                    r_aux = 0.5 * tan(it * a + r_modifier); 

                if (g_func == ColorFunction::Sin)
                    g_aux = 0.5 * sin(it * a + g_modifier);
                else if (g_func == ColorFunction::Cos)
                    g_aux = 0.5 * cos(it * a + g_modifier);
                else 
                    g_aux = 0.5 * tan(it * a + g_modifier); 

                if (b_func == ColorFunction::Sin)
                    b_aux = 0.5 * sin(it * a + b_modifier);
                else if (r_func == ColorFunction::Cos)
                    b_aux = 0.5 * cos(it * a + b_modifier);
                else 
                    b_aux = 0.5 * tan(it * a + b_modifier); 

                vertices[k].color.r = static_cast<sf::Uint8>(255.0 * (r_aux + 0.5));
                vertices[k].color.g = static_cast<sf::Uint8>(255.0 * (g_aux + 0.5));
                vertices[k].color.b = static_cast<sf::Uint8>(255.0 * (b_aux + 0.5));
            }
    }
    win.draw(vertices);
}
