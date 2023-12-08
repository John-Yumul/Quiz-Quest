#pragma once
#include <SFML/graphics.hpp>
#include <String>
#include<iostream>
using namespace std;
using namespace sf;

class Button
{
public:
	Button(String FileLoc, Vector2f size, Vector2f pos);
	FloatRect bounds();
	void Draw(RenderWindow& window);
	void Hover();
	void Click();
	void Def();
	Vector2f getpos();
	Vector2f getsize();

private:
	RectangleShape button;
	Texture texture;
	int textLength;
	String FileName, Original;
};

