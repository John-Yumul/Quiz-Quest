#include "Plant.h"

Plant::Plant(Texture* tex, Vector2f size, Vector2f pos)
{
	body.setSize(size);
	body.setTexture(tex);
	body.setPosition(pos);
	body.setOrigin(size / 2.0f);
}

Plant::~Plant()
{
}
void Plant::Transparent()
{
	Color color = body.getFillColor();
	color.a = 128; // Set alpha value (0 = transparent, 255 = opaque)
	body.setFillColor(color);
}
void Plant::Draw(RenderWindow& window)
{
	window.draw(body);
}