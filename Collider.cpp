#include "Collider.h"


Collider::Collider(RectangleShape& body) : body(body)
{
}

Collider::~Collider()
{
}

bool Collider::isColliding(Collider& other, float resistance)
{
	Vector2f otherPosition = other.GetPos();
	Vector2f otherSize = other.GetSize();
	Vector2f yourPosition = GetPos();
	Vector2f yourSize = GetSize();

	float dx = otherPosition.x - yourPosition.x;
	float dy = otherPosition.y - yourPosition.y;
	float intersectX = abs(dx) - (otherSize.x + yourSize.x);
	float intersectY = abs(dy) - (otherSize.y + yourSize.y);

	if (intersectX < 0.0f && intersectY < 0.0f) {

		resistance = min(max(resistance, 0.0f), 1.0f);
		if (intersectX > intersectY) {
			if (dx > 0.0f) {
				Move(intersectX * (1.0f - resistance), 0.0f);
				other.Move(-intersectX * resistance, 0.0f);
			}
			else {
				Move(-intersectX * (1.0f - resistance), 0.0f);
				other.Move(intersectX * resistance, 0.0f);
			}

		}
		else {
			if (dy > 0.0f) {
				Move(0.0f, intersectY * (1.0f - resistance));
				other.Move(0.0f, -intersectY * resistance);
			}
			else {
				Move(0.0f, -intersectY * (1.0f - resistance));
				other.Move(0.0f, intersectY * resistance);
			}

		}
		return true;
	}
	return false;
}