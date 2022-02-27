#pragma once
#include <SFML\Graphics.hpp>
#include "Player.h"
#include "Bullet.h"
#include "Zombie.h"
#include <Windows.h>
#include <string>
#include <iostream>
#include "Collision.h"
#include <string>
#include "Weapon.h"
#include "EEDelayedText.h"
#include "Engine.h"
using namespace sf;
using namespace std;
class Game
{
private:
	Font font;
	int roundsSurvived;
	SoundBuffer dead_buffer;
	Sound dead_sound;
	SoundBuffer button_buffer;
	Sound button_sound;
	Music music;
public:
	RenderWindow window;
	Clock clock;
	View view;
	Image image;
	Texture texture;
	enum GameState { MENU, GAME, END, FAILURE,HELP,INTRO };
	GameState state;
	void run();
	void menu();
	void intro();
	void play();
	void help();
	void failure();
	Game(void);
	~Game(void);
};

