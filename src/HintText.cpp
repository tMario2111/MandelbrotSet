#include "HintText.hpp"

HintText::HintText(sf::Font& font)
{
    nlohmann::json json{};
    {
        std::ifstream file{ "data/HintText.json" };
        file >> json;
        file.close();
    }

    text.setFont(font);
    text.setCharacterSize(20);
    text.setString(std::string{ json["text"] });
    text.setPosition(0.f, 0.f);
}

void HintText::update(sf::RenderWindow& win, Input& input)
{
    const auto view = win.getView();
    text.setPosition(
        view.getCenter().x - view.getSize().x / 2,
        view.getCenter().y - view.getSize().y / 2
    );
    if (!ImGui::GetIO().WantCaptureKeyboard && input.isKeyReleased(sf::Keyboard::H))
    {
        if (hidden)
            hidden = false;
        else 
            hidden = true;
    }
}

void HintText::draw(sf::RenderTarget& target, sf::RenderStates states) const
{   
    if (!hidden)
        target.draw(text, states);
}