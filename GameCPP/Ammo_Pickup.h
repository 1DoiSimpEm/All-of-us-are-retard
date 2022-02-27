#pragma once
#include "Pickup.h"
class Ammo_Pickup :
	public Pickup
{
public:
	Ammo_Pickup(sf::Texture& texture, sf::Vector2f pos)
	{

		onCreate_clocker.restart();
		body.setTexture(texture);
		body.setPosition(pos);

		body.setScale(0.06f, 0.06f);

	}
	~Ammo_Pickup() {

	}
};
