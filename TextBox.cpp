#include "TextBox.h"

TextBox::TextBox(RenderWindow& window, Font& font, Vector2f textPos, Vector2f boxPos) : window(window), font(font)
{
    inputText.setFont(font);
    inputText.setCharacterSize(20);
    inputText.setFillColor(Color::Black);
    inputText.setPosition(textPos);

    inputRect.setSize(Vector2f(300.f, 40.f)); // Adjust the size of the text box
    inputRect.setFillColor(Color::White);
    inputRect.setOutlineColor(Color::Black);
    inputRect.setOutlineThickness(2.f);
    inputRect.setPosition(boxPos);
}

void TextBox::handleInput(Event& event)
{
    if (event.type == Event::TextEntered) {
        if (event.text.unicode < 128) {
            if (event.text.unicode == '\b' && !inputString.empty()) {
                inputString.pop_back();
            }
            else if (inputString.size() < maxCharacters) {
                inputString += static_cast<char>(event.text.unicode);
            }
            inputText.setString(inputString);
        }
    }
}

void TextBox::Draw()
{

    window.draw(inputRect);
    window.draw(inputText);

}
