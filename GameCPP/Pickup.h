#pragma once
#include <SFML\Graphics.hpp>
class Pickup
{

public:
	sf::Clock onCreate_clocker;
	sf::Sprite body;
	float lifeTime;
	Pickup():lifeTime(20.0f){}
	~Pickup(){}
};

