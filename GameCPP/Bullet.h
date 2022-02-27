#pragma once
#include <SFML\Graphics.hpp>
using namespace sf;
#define M_PI 3.14159265359
using namespace sf;
class Bullet
{
public:
	float speed;
	float angle;
	Clock clocker;
	Sprite body;
	Bullet(float, Vector2f, Texture&);
	void move(Time);


};


