#pragma once
#include <vector>
#include <cmath>
#include <SFML\Graphics.hpp>
#include <string>
#include <iostream>
#include <SFML/Audio.hpp>
#include "SoundPlayer.h"
#include "Bullet.h"
using namespace std;
class Weapon
{
public:
	string name;
	SoundBuffer reload_buffer;
	Sound reload_sound;
	vector<Bullet> bullets;
	float bullets_left;
	float magazine_size;
	float magazine;
	float fire_rate;
	float angle;
	float bullet_speed;
	float damage;
	Texture texture_bullet;
	Texture texture_gun;
	Texture texture_hud;
	Sprite body;
	float reload_time;
	Clock reload_timer;
	Clock fire_timer;
	Vector2f gun_scale;
	Vector2f bullet_scale;
	SoundBuffer buffer;
	Sound sound;
public:
	bool reloading;
	bool reload_demand;
	Weapon();
	Weapon(string, string,  string,float, float, float, float, Vector2f, Vector2f, Vector2f, float, float,string);
	~Weapon();
	void shoot(Vector2f);
	void reload();
	void update(float , Time , Vector2f );
};


