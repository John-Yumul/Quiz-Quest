#pragma once
#include "Animation.h"
#include "Collider.h"

using namespace sf;


class Player
{
public:
	Player(Texture* tex, Vector2u imagecount, float switchtime, float speed); // player structure for movement and animation
	~Player();
	void Update(float deltaTime); // changes
	void Draw(RenderWindow& window);	//drawing player to the window
	void setSpeed(float x);
	FloatRect bounds();
	void setPos(Vector2f pos);
	Vector2f GetPosition() {
		return body.getPosition(); //retireve the position
	}

	Collider GetCollider() { return Collider(body); }

private:
	RectangleShape body;
	Animation animation;
	unsigned int row;
	float speed;
	bool faceRight;
	//bool faceUp;

};

