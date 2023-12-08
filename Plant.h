#pragma once
#include "Collider.h"
#include "Plant.h"
using namespace sf;

class Plant
{
public:
	Plant(Texture* tex, Vector2f size, Vector2f pos);
	~Plant();

	Collider GetCollider() {
		return Collider(body);
	}
	void Transparent();
	void Draw(RenderWindow& window);
private:
	RectangleShape body;
};
