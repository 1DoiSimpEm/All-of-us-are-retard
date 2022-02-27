#pragma once
#include <SFML\Graphics.hpp>
#include <SFML\Audio.hpp>
#include "Player.h"
#include "Bullet.h"
#include "Zombie.h"
#include <Windows.h>
#include <string>
#include <iostream>
#include "Collision.h"
#include <string>
#include "Game.h"
#include <deque>
#include "Weapon.h"
#include "HP_Pickup.h"
#include "Ammo_Pickup.h"
#include "Corp.h"
class Engine
{
private:
	Player player;
	int* ptrRoundsSurvived;
	bool menu;
	bool isSprint;
	bool canSprint;
	bool generate;

	bool switch_weapon;
	float vx, vy;
	Text UI[7];// hp[0],round[1],gun_name[2],bullets_Left[3],zombie_left[4], reloading_info[5], reloading_reminder[6] 
	Image* image;
	sf::RenderWindow* window;
	View * view;
	Vector2f mouse;
	Clock clocker;
	Clock zombie_generator_timer;
	int max_zombie_per_round;
	int generated_zombie;
	int zombie_killed;
	Time times;
	///
	Weapon * gun;
	Weapon pistol;
	Weapon rifle;
	//texture
	Texture texture_map;
	Texture texture_floor;
	Texture texture_aim;
	Texture texture_wall;	
	Texture wall_t;
	Texture texture_zombie;
	Texture texture_corp;
	Texture entrance;
	Texture texture_ammo;
	Texture texture_hp;
	//rectangle shpae of an UI
	RectangleShape sprint_hud;
	//sound
	SoundBuffer round_buffer,round_buffer2;
	Sound round_sound;
	SoundBuffer bite_buffer;
	Sound bite_sound;
	SoundBuffer hit_bufer;
	Sound hit_sound;
	SoundBuffer ammo_pickup_buffer;
	Sound ammo_pickup;
	SoundBuffer hp_pickup_buffer;
	Sound hp_pickup;
	Music music;
	//object
	vector<Zombie> zombies;
	vector<RectangleShape> walls;
	deque<Vector2f> tab_players;
	vector<Corp> corps;
	vector < HP_Pickup> hps;
	vector<Ammo_Pickup> ammos;
	//sprite
	Sprite map;
	Sprite gun_hud1;
	Sprite gun_hud2;
	Sprite aim;
	Sprite floor;
	Event event;
	Font font;
	void draw();
	void collision();
	void bullet_destroy(int );
	void zombie_destroy(int);
	void corps_destroy(int);
	void ammo_destroy(int);
	void hp_destroy(int);
	void zombie_generator(vector <Zombie>& zombies, Vector2f pos, Texture& texture, int round, RectangleShape& target_wal,float );
	void generator_manager();
	void update();
	void HUD();
	void event_manager();
	float angle;
	int round;
	void sprint_manager();
public:
	Engine(sf::RenderWindow&, View&, Image&,int&);
		bool runEngine();
	bool init();


};

