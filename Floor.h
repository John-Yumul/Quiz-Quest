#pragma once
#include <SFML/Graphics.hpp>
#include "Collider.h"
using namespace sf;

class Floor
{
public:
	Floor(Texture* tex, Vector2f size, Vector2f pos);
	~Floor();

	Collider GetCollider() {
		return Collider(body);
	}
	void setPos(Vector2f pos) {
		body.setPosition(pos);
	}
	
	void blacked();
	void Draw(RenderWindow& window);

	Vector2f getSize() {
		return body.getSize();
	}
protected:
	RectangleShape body;
};


class blocks :public Floor {
	public:
	blocks(Texture* tex, Vector2f size, Vector2f pos) :Floor(tex, size, pos) {

	}

	void transpa() {
		body.setFillColor(Color::Transparent);
	}
};