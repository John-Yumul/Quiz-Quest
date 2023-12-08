#include "Enter.h"

Enter::Enter(Vector2f pos)
{
	body.setPosition(pos);
	body.setRadius(30);
	body.setFillColor(Color::Blue);
	body.setOrigin(body.getRadius(), body.getRadius());
	Color color = body.getFillColor();
	color.a = 128; // Set alpha value (0 = transparent, 255 = opaque)
	body.setFillColor(color);
}

FloatRect Enter::bounds()
{
	return body.getGlobalBounds();
}
