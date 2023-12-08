#include "Floor.h"

Floor::Floor(Texture* tex, Vector2f size, Vector2f pos)
{
	body.setSize(size);
	body.setTexture(tex);
	body.setPosition(pos);
	body.setOrigin(size / 2.0f);
}

Floor::~Floor()
{
}


void Floor::blacked()
{
	body.setFillColor(Color::Black);
}


void Floor::Draw(RenderWindow& window)
{
	window.draw(body);
}