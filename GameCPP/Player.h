#pragma once
#include <SFML\Graphics.hpp>
#include "Animation.h"
using namespace sf;
class Player
{
public:
	Texture texture;
	Sprite body;
	bool canMoveUP;
	bool canMoveDown;
	bool canMoveLeft;
	bool canMoveRight;
	float healthPoints;
	float speed;
	Player();
	~Player();
	void update(Time ,float& ,float& , bool);
	void move(Time, float, float );
	void input(float&, float&, bool);


};

