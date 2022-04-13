#include "Input.hpp"

void Input::processEvent(const sf::Event& event)
{
    switch (event.type)
    {
        case sf::Event::KeyPressed:
            keyboard_input[event.key.code] = KeyState::Pressed;
            break;
        case sf::Event::KeyReleased:
            keyboard_input[event.key.code] = KeyState::Released;
            break;
        default:
            break;
    }
}

void Input::update()
{
    for (auto& i : keyboard_input)
        if (i.second == KeyState::Released)
            i.second = KeyState::None;
}

bool Input::isKeyPressed(sf::Keyboard::Key key)
{
    return keyboard_input[key] == KeyState::Pressed;
}

bool Input::isKeyReleased(sf::Keyboard::Key key)
{
    return keyboard_input[key] == KeyState::Released;
}
