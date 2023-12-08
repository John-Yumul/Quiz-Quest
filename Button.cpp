#include "Button.h"

Button::Button(String FileLoc, Vector2f size, Vector2f pos):FileName(FileLoc),Original(FileName)
{
	textLength = FileLoc.getSize();
	texture.loadFromFile(FileName);
	button.setTexture(&texture);
	button.setSize(size);
	button.setPosition(pos);
	button.setOrigin(button.getSize().x/2, button.getSize().y/2);
}

FloatRect Button::bounds()
{
	return button.getGlobalBounds();
}

void Button::Draw(RenderWindow& window)
{
	window.draw(button);
}

void Button::Hover()
{
	FileName = Original;
	FileName.insert(Original.getSize() - 4, to_string(1));
	texture.loadFromFile(FileName);
	button.setTexture(&texture);
}

void Button::Click()
{
	FileName = Original;
	FileName.insert(Original.getSize() - 4, to_string(2));
	texture.loadFromFile(FileName);
	button.setTexture(&texture);
}

void Button::Def()
{
	FileName = Original;
	texture.loadFromFile(FileName);
	button.setTexture(&texture);
}

Vector2f Button::getpos()
{
	return button.getPosition();
}

Vector2f Button::getsize()
{
	return button.getSize();
}
