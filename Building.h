#pragma once
#include <SFML/Graphics.hpp>
#include "Collider.h"
using namespace sf;

class Building
{
public:
	Building(Texture* tex, Vector2f size, Vector2f pos);
	~Building();

	Collider GetCollider() {
		return Collider(body);
	}
	void setPos(Vector2f pos) {
		body.setPosition(pos);
	}

	void Draw(RenderWindow& window);
private:
	RectangleShape body;
};
