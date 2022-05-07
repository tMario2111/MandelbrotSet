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
    win.create(prevMove, title, sf::Style::Close | sf::Style::Resize);
    win.setFramerateLimit(framerate_limit);
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
            case sf::Event::Resized:
                win.setView(sf::View{ win.getView().getCenter(), 
                    sf::Vector2f{ static_cast<float>(event.size.width), 
                                  static_cast<float>(event.size.height) } });
                prevMove.width = event.size.width;
                prevMove.height = event.size.height;
                set->onResize();
                break;
            case sf::Event::KeyReleased:
                if (event.key.code == sf::Keyboard::F11)
                {
                    if (!fullscreen)
                    {
                        fullscreen = true;
                        win.create(sf::VideoMode{ sf::VideoMode::getDesktopMode().width, sf::VideoMode::getDesktopMode().height + 1 }, 
                            title, sf::Style::None);
                        win.setFramerateLimit(framerate_limit);
                        set->onResize();
                    }
                    else 
                    {
                        fullscreen = false;
                        win.create(prevMove, title, sf::Style::Close | sf::Style::Resize);
                        win.setFramerateLimit(framerate_limit);
                        set->onResize();
                    }
                }
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
