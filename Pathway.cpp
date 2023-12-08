#include "Pathway.h"

Pathway::Pathway(Texture* tex, Vector2f size, Vector2f pos)
{
	body.setSize(size);
	body.setTexture(tex);
	body.setPosition(pos);
	body.setOrigin(size / 2.0f);
}

Pathway::~Pathway()
{
}

void Pathway::Draw(RenderWindow& window)
{
	window.draw(body);
}