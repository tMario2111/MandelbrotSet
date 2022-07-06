#include "App.hpp"

App::App()
{
    setupWin();
    loadAssets();
    setupGui();

    set = std::make_unique<MandelbrotSet>(win, input, font);
    run();
}

void App::setupWin()
{
    win.create(prevMove, title, sf::Style::Close | sf::Style::Resize);
    win.setFramerateLimit(framerate_limit);
}

void App::loadAssets()
{
    font.loadFromFile("assets/VCR_OSD_MONO_1.001.ttf");
}

void App::setupGui()
{
    ImGui::SFML::Init(win);
    ImGui::GetIO().IniFilename = nullptr;
}

void App::resizeEvent(const sf::Event& event)
{
    auto view = win.getView();
    view.setSize(static_cast<float>(event.size.width), static_cast<float>(event.size.height));
    win.setView(view);
    prevMove.width = event.size.width;
    prevMove.height = event.size.height;
    set->onResize();
}

void App::toggleFullscreen()
{
    if (!fullscreen)
    {
        fullscreen = true;
        auto video_mode = sf::VideoMode::getDesktopMode();
        video_mode.height++;
        win.create(video_mode, title, sf::Style::None);
    }
    else 
    {
        fullscreen = false;
        win.create(prevMove, title, sf::Style::Close | sf::Style::Resize);
    }
    win.setFramerateLimit(framerate_limit);
    set->onResize();
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
                resizeEvent(event);
                break;
            case sf::Event::KeyReleased:
                if (event.key.code == sf::Keyboard::F11)
                    toggleFullscreen();
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
