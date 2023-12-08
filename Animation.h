#pragma once
#pragma once
#include <SFML/Graphics.hpp>
using namespace sf;

//declaration for animation // header file ng animation
class Animation
{
private:
	Vector2u ImageCount;	//size ng image
	Vector2u CurrentImage;  //for detection kung aling frame ba sa image yung ididisplay

	float TotalTime;		// total time for animation
	float SwitchTIme;		//how fast will the image change frame


public:
	IntRect uvRect;			//crop ng image

	Animation(Texture* tex, Vector2u imagecount, float switchtime); //instructor     //tex for image
	~Animation();

	//void Update(int row, float deltaTime, bool faceRight, bool FaceUp); 
	void Update(int row, float deltaTime, bool faceRight); // for changing the facing	
};

