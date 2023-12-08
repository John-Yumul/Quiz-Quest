#pragma once
#include<SFML/Graphics.hpp>
#include <string>
#include <iostream>
using namespace std;
using namespace sf;

class TextBox
{
public:
    TextBox(RenderWindow& window, Font& font, Vector2f textPos, Vector2f boxPos);

    void handleInput(Event& event);

    void Draw();

    void reset() {
        inputString.clear();
        inputText.setString(inputString);
    }

    string getInput() const {
        return inputString;
    }


private:
    RenderWindow& window;
    Font& font;
    Text inputText;
    RectangleShape inputRect;
    string inputString;
    const int maxCharacters = 25;
    const int maxDream = 50;
};

