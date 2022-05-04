#include "App.hpp"

App::App()
{
    setupWin();
    setupGui();
    set = std::make_unique<MandelbrotSet>(win, input);
    run();
}

void App::setupWin()
{
    win.create(sf::VideoMode{ 800, 800 }, "Mandelbrot Set", sf::Style::Close);
    win.setFramerateLimit(30);
}

void App::setupGui()
{
    ImGui::SFML::Init(win);
    ImGui::GetIO().IniFilename = nullptr;
}

void App::winEvents()
{
    dt = win_clock.restart();
    input.update();
    for (sf::Event event{}; win.pollEvent(event);)
    {
        ImGui::SFML::ProcessEvent(event);
        input.processEvent(event);
        switch (event.type)
        {
            case sf::Event::Closed:
                win.close();
                break;
            default:
                break;
        }
    }
}

void App::run()
{
    while (win.isOpen())
    {
        winEvents();
        
        ImGui::SFML::Update(win, dt);
        set->update();

        win.clear();
        set->render();
        ImGui::SFML::Render(win);
        win.display();
    }
    ImGui::SFML::Shutdown();
}
