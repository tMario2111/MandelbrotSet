#pragma once

#include <SFML/Graphics.hpp>

#include <unordered_map>

class Input
{
public:
    void processEvent(const sf::Event& event);
    void update();
    bool isKeyPressed(sf::Keyboard::Key key);
    bool isKeyReleased(sf::Keyboard::Key key);
private:
    enum KeyState
    {
        None,
        Pressed,
        Released
    };
    std::unordered_map<sf::Keyboard::Key, KeyState> keyboard_input;
};
