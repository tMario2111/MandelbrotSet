#include "MandelbrotSet.hpp"
#include <imgui.h>

MandelbrotSet::MandelbrotSet(sf::RenderWindow& win, Input& input, sf::Font& font) :
    win{ win },
    input{ input },
    vertices{ sf::PrimitiveType::Points },
    hint_text{ font }
{
    onResize();

    thread_count = std::thread::hardware_concurrency();
    threads.resize(thread_count);
    std::cout << "Number of threads: " << thread_count << '\n';
}

sf::Vector2<f_type> MandelbrotSet::mapWinCoordsToMandelbrot(sf::Vector2<f_type> a)
{
    sf::Vector2<f_type> b{};
    b.x = domain_x_1 + a.x / static_cast<f_type>(win.getSize().x) * domain_x_s;
    b.y = domain_y_1 + a.y / static_cast<f_type>(win.getSize().y) * domain_y_s;
    return b;
}

sf::Vector2<f_type> MandelbrotSet::mapMandelbrotCoordsToWin(sf::Vector2<f_type> a)
{
    sf::Vector2<f_type> b{};
    b.x = (a.x - domain_x_1) * static_cast<f_type>(win.getSize().x) / domain_x_s;
    b.y = (a.y - domain_y_1) * static_cast<f_type>(win.getSize().y) / domain_y_s;
    return b;
}

sf::Vector2<f_type> MandelbrotSet::getCursorPosition()
{
    auto win_pos = sf::Vector2<f_type>{ sf::Mouse::getPosition(win) };
    win_pos.x /= static_cast<f_type>(win.getSize().x);
    win_pos.y /= static_cast<f_type>(win.getSize().y);
    
    sf::Vector2<f_type> view_pos{};
    view_pos.x = view.left + view.width * win_pos.x;
    view_pos.y = view.top + view.height * win_pos.y;
    return view_pos;
}

f_type MandelbrotSet::getZoom()
{
    return static_cast<f_type>(win.getSize().x) / view.width;
}

void MandelbrotSet::setLocation(const Location location)
{
    // Set position
    const auto coord = mapMandelbrotCoordsToWin(location.position);
    view.left = coord.x - view.width / 2.0;
    view.top = coord.y - view.height / 2.0;

    // Set zoom
    const auto center = sf::Vector2<f_type>{ view.left + view.width / 2.0, view.top + view.height / 2.0 };
    view.width = static_cast<f_type>(win.getSize().x) / location.zoom;
    view.height = static_cast<f_type>(win.getSize().y) / location.zoom;
    view.left = center.x - view.width / 2.0;
    view.top = center.y - view.height / 2.0;
}

void MandelbrotSet::onResize()
{
    needs_update = true;

    vertices.resize(static_cast<std::vector<unsigned int>::size_type>(win.getSize().x) * win.getSize().y);

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
    
    ImGui::Begin("SETTINGS");

    ImGui::BeginTabBar("tabBar");

    if (ImGui::BeginTabItem("CONTROL"))
    {
        mandelbrot_coords = mapWinCoordsToMandelbrot(getCursorPosition());
        zoom = getZoom();
        if (ImGui::InputDouble("x", &mandelbrot_coords.x, 0.0, 0.0, "%.16f", 
            ImGuiInputTextFlags_::ImGuiInputTextFlags_EnterReturnsTrue))
        {
            const auto coord = mapMandelbrotCoordsToWin(mandelbrot_coords);
            view.left = coord.x - view.width / 2.0;
            needs_update = true;
        }

        if (ImGui::InputDouble("y", &mandelbrot_coords.y, 0.0, 0.0, "%.16f", 
            ImGuiInputTextFlags_::ImGuiInputTextFlags_EnterReturnsTrue))
        {
            const auto coord = mapMandelbrotCoordsToWin(mandelbrot_coords);
            view.top = coord.y - view.height / 2.0;
            needs_update = true;
        }

        if (ImGui::InputDouble("zoom", &zoom, 0.0, 0.0, "%.2f", 
            ImGuiInputTextFlags_::ImGuiInputTextFlags_EnterReturnsTrue))
        {
            const auto center = sf::Vector2<f_type>{ view.left + view.width / 2.0, view.top + view.height / 2.0 };
            view.width = static_cast<f_type>(win.getSize().x) / zoom;
            view.height = static_cast<f_type>(win.getSize().y) / zoom;
            view.left = center.x - view.width / 2.0;
            view.top = center.y - view.height / 2.0;
            needs_update = true;
        }

        spacing();

        if (ImGui::SliderInt("Iterations", &max_iterations, 100, 1000))
            needs_update = true;

        spacing();

        if (ImGui::Checkbox("Black accent color", &black_accent_color))
            needs_update = true;

        ImGui::EndTabItem();
    }
    if (ImGui::BeginTabItem("THEMES"))
    {
        needs_update = themes.gui() ? true : needs_update;

        ImGui::EndTabItem();
    }
    if (ImGui::BeginTabItem("LOCATIONS"))
    {
        const auto center = sf::Vector2<f_type>{ view.left + view.width / 2.0, view.top + view.height / 2.0 };
        const auto location_optional = locations.gui(Location{ mapWinCoordsToMandelbrot(center), getZoom() });
        if (location_optional.has_value())
        {
            setLocation(location_optional.value());
            needs_update = true;
        }
        ImGui::EndTabItem();
    }
    if (ImGui::BeginTabItem("SCREENSHOT"))
    {
        ImGui::InputText("Filename", screenshot_name, 100);
        if (ImGui::Button("Save"))
            takeScreenshot();

        ImGui::EndTabItem();
    }
    if (ImGui::BeginTabItem("INFO"))
    {
        auto str = "Render time: " + std::to_string(render_time.asSeconds()) + " seconds";
        ImGui::Text("%s", str.c_str());

        ImGui::EndTabItem();
    }

    ImGui::EndTabBar();
    ImGui::End();
}

void MandelbrotSet::takeScreenshot()
{
    const auto name = "screenshots/" + std::string{ screenshot_name };

    sf::RenderTexture texture{};
    texture.create(win.getSize().x, win.getSize().y);
    texture.setView(win.getView());
    texture.draw(vertices);
    texture.display();
    texture.getTexture().copyToImage().saveToFile(name + ".png");

    const auto lt = mapWinCoordsToMandelbrot(sf::Vector2<f_type>{ view.left, view.top });
    const auto rb = mapWinCoordsToMandelbrot(sf::Vector2<f_type>{
        view.left + view.width,
        view.top + view.height
    });

    nlohmann::json json{};
    json["x"] = (lt.x + rb.x) / 2.0;
    json["y"] = (lt.y + rb.y) / 2.0;
    json["zoom"] = zoom;

    std::ofstream file{ name + ".json" };
    file << std::setw(4) << json;
    file.close();
}

void MandelbrotSet::control()
{
    if (ImGui::GetIO().WantCaptureKeyboard)
        return;
    if (input.isKeyPressed(sf::Keyboard::Add) || input.isKeyPressed(sf::Keyboard::E))
    {
        view.left += view.width / 4.0;
        view.top += view.height / 4.0;
        view.width /= 2.0;
        view.height /= 2.0;
        needs_update = true;
    }
    else if (input.isKeyPressed(sf::Keyboard::Subtract) || input.isKeyPressed(sf::Keyboard::Q))
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
    auto column_lenght = win.getSize().x / thread_count;
    if (thread_count * column_lenght < win.getSize().x)
        column_lenght++;

    const f_type win_size_x = win.getSize().x;
    const f_type win_size_y = win.getSize().y;

    const auto min_size = std::min(win_size_x, win_size_y);
    const auto domain_x_increment = (win_size_x / min_size - 1) * default_domain_x_s / 2.0;
    const auto domain_y_increment = (win_size_y / min_size - 1) * default_domain_y_s / 2.0;

    domain_x_1 = default_domain_x_1 + domain_x_increment;
    domain_x_s = default_domain_x_s + domain_x_increment;

    domain_y_1 = default_domain_y_1 + domain_y_increment;
    domain_y_s = default_domain_y_s + domain_y_increment;

    const auto increment_x = view.width / static_cast<f_type>(win_size_x);
    const auto increment_y = view.height / static_cast<f_type>(win_size_y);

    auto left = view.left + c_thread * (view.width / thread_count);
    auto top = view.top;

    unsigned int iterations{};
    f_type x, y;
    f_type x0, y0;
    f_type x2, y2;

    unsigned int k;
    const auto a = 0.1f;
    f_type it;
    f_type r_aux, g_aux, b_aux;
    const auto selected_theme = themes.getSelectedTheme();

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

            // The heavy worker
            while (x2 + y2 <= 4.0 && iterations < max_iterations)
            {
                y = 2 * x * y + y0;
                x = x2 - y2 + x0;
                x2 = x * x;
                y2 = y * y;
                iterations++;
            }

            it = static_cast<f_type>(iterations);

            if (selected_theme.r_func == ColorFunction::Sin)
                r_aux = 0.5 * sin(it * a + selected_theme.r_modifier);
            else if (selected_theme.r_func == ColorFunction::Cos)
                r_aux = 0.5 * cos(it * a + selected_theme.r_modifier);
            else
                r_aux = 0.5 * tan(it * a + selected_theme.r_modifier);

            if (selected_theme.g_func == ColorFunction::Sin)
                g_aux = 0.5 * sin(it * a + selected_theme.g_modifier);
            else if (selected_theme.g_func == ColorFunction::Cos)
                g_aux = 0.5 * cos(it * a + selected_theme.g_modifier);
            else
                g_aux = 0.5 * tan(it * a + selected_theme.g_modifier);

            if (selected_theme.b_func == ColorFunction::Sin)
                b_aux = 0.5 * sin(it * a + selected_theme.b_modifier);
            else if (selected_theme.b_func == ColorFunction::Cos)
                b_aux = 0.5 * cos(it * a + selected_theme.b_modifier);
            else
                b_aux = 0.5 * tan(it * a + selected_theme.b_modifier);

            k = i * win.getSize().y + j;
            if (it == max_iterations && black_accent_color)
            {
                vertices[k].color.r = 0;
                vertices[k].color.b = 0;
                vertices[k].color.g = 0;
            }
            else 
            {
                vertices[k].color.r = static_cast<sf::Uint8>(255.0 * (r_aux + 0.5));
                vertices[k].color.g = static_cast<sf::Uint8>(255.0 * (g_aux + 0.5));
                vertices[k].color.b = static_cast<sf::Uint8>(255.0 * (b_aux + 0.5));
            }

            top += increment_y;
        }
        left += increment_x;
    }
}

void MandelbrotSet::update()
{
    gui();
    control();
    hint_text.update(win, input);

    if (!needs_update)
        return;
    
    render_clock.restart();

    for (auto i = 0u; i < thread_count; i++)
        threads[i] = std::thread{ &MandelbrotSet::fractal, this, i };

    for (auto& thread : threads)
        thread.join();

    needs_update = false;
    render_time = render_clock.getElapsedTime();
}

void MandelbrotSet::render()
{
    win.draw(vertices);
    win.draw(hint_text);
}