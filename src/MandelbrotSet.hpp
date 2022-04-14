#pragma once

#include "Input.hpp"

#include <SFML/Graphics.hpp>

#include <vector>

class MandelbrotSet
{
public:
    MandelbrotSet(sf::RenderWindow& win, Input& input);
    void update();
    void render();
private:
    sf::RenderWindow& win;
    Input& input;
    sf::Rect<long double> view;
    sf::VertexArray vertices;
    bool needs_update = true;
};
