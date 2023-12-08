#pragma once
#include<SFML/Graphics.hpp>
using namespace sf;

class Enter
{

public:
	void Draw(RenderWindow& window) {
		window.draw(body);
	}
	Enter(Vector2f pos);
	FloatRect bounds();
private:
	CircleShape body;
};

