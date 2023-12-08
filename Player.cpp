#include "Player.h"

Player::Player(Texture* tex, Vector2u imagecount, float switchtime, float speed) : //structure
	animation(tex, imagecount, switchtime) // assigning animation
{
	this->speed = speed; //speed of movement
	row = 0;
	faceRight = true;

	body.setSize(Vector2f(50.0f, 100.0f));	//size, position , image , cetralization
	body.setPosition(2242, 3254);
	body.setTexture(tex);
	body.setOrigin(body.getSize() / 2.0f);

}

Player::~Player()
{
}

void Player::Update(float deltaTime) // movement up,down,left,right
{
	Vector2f movement(0.0f, 0.0f);
	if (Keyboard::isKeyPressed(Keyboard::A) || Keyboard::isKeyPressed(Keyboard::Left)) {
		movement.x -= speed * deltaTime;
	}
	if (Keyboard::isKeyPressed(Keyboard::D) || Keyboard::isKeyPressed(Keyboard::Right)) {
		movement.x += speed * deltaTime;
	}
	if (Keyboard::isKeyPressed(Keyboard::W) || Keyboard::isKeyPressed(Keyboard::Up)) {
		movement.y -= speed * deltaTime;
	}
	if (Keyboard::isKeyPressed(Keyboard::S) || Keyboard::isKeyPressed(Keyboard::Down)) {
		movement.y += speed * deltaTime;
	}

	if (movement.x == 0.0f) {
		row = 0;
	}
	else {
		row = 3;
		if (movement.x > 0.0f) {
			faceRight = true;
		}
		else {
			faceRight = false;
		}
	}
	animation.Update(row, deltaTime, faceRight);
	body.setTextureRect(animation.uvRect);//setting position in the image
	body.move(movement);

	/*
	if (movement.y == 0.0f) {
		row = 0;
	}
	else{
		row = 1;
		if (movement.y > 0.0f) {
			faceUp = true;
		}
		else {
			faceUp = false;
		}
	}
	*/

}

void Player::Draw(RenderWindow& window)
{
	window.draw(body);
}

void Player::setSpeed(float x)
{
	this->speed = x;
}

FloatRect Player::bounds()
{
	return body.getGlobalBounds();
}

void Player::setPos(Vector2f pos)
{
	body.setPosition(pos);
}
