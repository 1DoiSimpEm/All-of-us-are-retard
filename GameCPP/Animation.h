#pragma once
#include "SFML/Graphics.hpp"
#include<vector>
using namespace std;
class Animation : public sf::Sprite
{
private:
	vector<sf::Texture> listTexture;
	int  currentFrame;
	float currentTime, frameTime;
public:
	Animation(string pathFolder, int frameNum, float frameTime);
	~Animation();
	void update(float deltaTime);
};
