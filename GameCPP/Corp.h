#pragma once
#include <SFML\Graphics.hpp>


using namespace sf;

class Corp
{
public:
	Clock onCreate_clock;
	Sprite body;
	float life_time;
	Corp(Texture& texture, Vector2f body_pos, float angle)
		:life_time(5.0f)
	{
		onCreate_clock.restart();
		body.setTexture(texture);
		body.setPosition(body_pos);
		body.setRotation(angle);
		body.setScale(0.35f, 0.35f);
	}
	~Corp()
	{}
	
};

