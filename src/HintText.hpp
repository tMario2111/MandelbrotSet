#pragma once

#include "Input.hpp"

#include <nlohmann/json.hpp>
#include <SFML/Graphics.hpp>

#include <fstream>
#include <string>
#include <vector>

class HintText : public sf::Drawable
{
public:
    HintText(sf::Font& font);
    bool hidden = false;

    void draw(sf::RenderTarget& target, sf::RenderStates states) const;
    void update(sf::RenderWindow& win, Input& input);
private:
    sf::Text text;
};