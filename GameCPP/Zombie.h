#pragma once
#include <SFML\Graphics.hpp>
#include <math.h>
#include "Collision.h"
#include <iostream>
using namespace sf;
using namespace std;
class Zombie {
	int texture_iterator;

	float angle;
	Clock colliding_timer;
	float pauseTime;
	Clock texture_timer;
	Vector2f building_center;
public:
	Zombie(Vector2f, Texture& texture, float v, float hp, RectangleShape& target_wal);
	float speed;
	Vector2f window_target;
	Vector2f boids_vector;
	float rotate_angle;
	float healthPoints;
	int collides_zombie;
	float collide_parameter;
	bool isInside;
	bool goingThrough;
	bool collidesWithZombie;
	bool haveAttacked;
	bool collidesWithPlayer;
	Sprite body;
	Clock playerCollisionTimer;
	float biteFrequency;
	RectangleShape target_wall;
	float rotation;
	Vector2f collision(vector <Zombie>& tab, Time time, vector <RectangleShape>& walls);
	void move(Time time, vector <Zombie>& tab, vector<RectangleShape>& walls);
	void setSpeed(float);
	void update(Vector2f cel, Time time, Vector2f recentPlayerPosition, vector <Zombie>& tab, vector <RectangleShape>& walls);

};