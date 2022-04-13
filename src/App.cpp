#include "App.hpp"

App::App() :
    win{ sf::VideoMode{ 800, 800 }, "Mandelbrot set", sf::Style::Close },
    set{ win }
{
    setupWin();
    run();
}

void App::setupWin()
{
    win.setFramerateLimit(10);
}

void App::winEvents()
{
    for (sf::Event event{}; win.pollEvent(event);)
        switch (event.type)
        {
            case sf::Event::Closed:
                win.close();
                break;
            default:
                break;
        }
}

void App::run()
{
    while (win.isOpen())
    {
        winEvents();
        set.update();
        win.clear();
        set.render();
        win.display();
    }
}
