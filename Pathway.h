#pragma once
#include <SFML/Graphics.hpp>
#include "Pathway.h"
#include "Collider.h"
using namespace sf;

class Pathway
{
public:
	Pathway(Texture* tex, Vector2f size, Vector2f pos);
	~Pathway();

	Collider GetCollider() {
		return Collider(body);
	}

	void Draw(RenderWindow& window);
private:
	RectangleShape body;
};
