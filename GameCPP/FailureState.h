#pragma once
#include <SFML\Graphics.hpp>
#include "Engine.h"
#include "Game.h"
using namespace sf;
using namespace std;
class FailureState
{
	RenderWindow* window;
	Event event;
	bool menu;
	bool playAgain;
	bool failure;
	View view;
	Font* font;
	Image* image;
	int rounds;
	Clock clock;
	Texture texture;
public:
	FailureState(RenderWindow& _window, Font& _font, int _rounds);
	~FailureState();
	bool run(Image& _image);
};

