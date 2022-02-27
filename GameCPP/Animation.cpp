#include"Animation.h"
Animation::Animation(string pathFolder, int frameNum, float frameTime)
{
	currentFrame = 0;
	currentTime = 0;
	this->frameTime = frameTime;
	for (int i = 1; i <= frameNum; i++)
	{
		sf::Texture tmp;
		tmp.loadFromFile(pathFolder + "/" + to_string(i) + ".png");
		listTexture.push_back(tmp);
	}
}
Animation::~Animation()
{

}
void Animation::update(float deltaTime)
{
	currentTime += deltaTime;
	if (currentTime >= frameTime)
	{
		currentTime -= frameTime;
		currentFrame++;
		if (currentFrame > listTexture.size() - 1)
			currentFrame = 0;
	}
	setTexture(listTexture[currentFrame]);
}
