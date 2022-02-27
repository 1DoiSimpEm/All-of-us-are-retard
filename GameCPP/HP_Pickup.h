#pragma once
#include "Pickup.h"
class HP_Pickup :public Pickup
{
public:
	float add_hp;
	HP_Pickup(sf::Texture& texture, sf::Vector2f pos) :add_hp(1)
	{
		onCreate_clocker.restart();
		body.setTexture(texture);
		body.setPosition(pos);
		body.setScale(0.15f, 0.15f);
	}
	~HP_Pickup(){}
};