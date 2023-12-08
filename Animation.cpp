#include "Animation.h"

Animation::Animation(Texture* tex, Vector2u imagecount, float switchtime) { //instructor
	this->ImageCount = imagecount; //declaring variables
	this->SwitchTIme = switchtime;
	TotalTime = 0.0f;
	CurrentImage.x = 0;

	uvRect.width = tex->getSize().x / float(ImageCount.x); // cropping image
	uvRect.height = tex->getSize().y / float(ImageCount.y);
}

//void Animation::Update(int row, float deltaTime, bool faceRight, bool faceUp)
void Animation::Update(int row, float deltaTime, bool faceRight) {
	CurrentImage.y = 0;
	TotalTime += deltaTime; // using tick

	if (TotalTime >= SwitchTIme) { // changing position
		TotalTime -= SwitchTIme;
		CurrentImage.x++;

		if (CurrentImage.x >= ImageCount.x) {
			CurrentImage.x = 0;
		}
	}


	if (faceRight) { // facing
		uvRect.left = CurrentImage.x * uvRect.width;
		uvRect.width = abs(uvRect.width);
	}
	else {
		uvRect.left = (CurrentImage.x + 1) * abs(uvRect.width);
		uvRect.width = -abs(uvRect.width);
	}

	/*
	if (faceUp) {
		uvRect.top = CurrentImage.y * uvRect.height;
		uvRect.height = abs(uvRect.height);
	}
	else {
		uvRect.top = (CurrentImage.y + 1) * abs(uvRect.height);
		uvRect.height = -abs(uvRect.height);
	}*/

}

Animation::~Animation() {

}