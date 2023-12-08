#include "Building.h"

Building::Building(Texture* tex, Vector2f size, Vector2f pos)
{
	body.setSize(size);
	body.setTexture(tex);
	body.setPosition(pos);
	body.setOrigin(size / 2.0f);
}

Building::~Building()
{
}

void Building::Draw(RenderWindow& window)
{
	window.draw(body);
}