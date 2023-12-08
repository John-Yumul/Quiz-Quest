#pragma once
#include <SFML/Graphics.hpp>
using namespace std;
using namespace sf;


class Collider
{

public:
	Collider(RectangleShape& body);
	~Collider();

	bool isColliding(Collider& other, float resistance);

	Vector2f GetPos() {
		return body.getPosition();
	}

	Vector2f GetSize() {
		return(body.getSize() / 2.0f);
	}

	void Move(float dx, float dy) {
		body.move(dx, dy);
	}

private:
	RectangleShape& body;

};
