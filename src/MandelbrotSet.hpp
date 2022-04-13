#pragma once

#include <SFML/Graphics.hpp>

#include <vector>

class MandelbrotSet
{
public:
    explicit MandelbrotSet(sf::RenderWindow& win);
    void update();
    void render();
private:
    sf::RenderWindow& win;
    sf::VertexArray vertices;
    bool needs_update = true;
};
