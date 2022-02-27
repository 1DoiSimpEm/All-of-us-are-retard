#include "Player.h"

Player::Player() :
	canMoveUP(true),
	canMoveDown(true),
	canMoveLeft(true),
	canMoveRight(true),
	healthPoints(5),
	speed(100.0f)
{
	texture.loadFromFile("data/player_empty.png");
	body.setTexture(texture),
	body.setScale(0.2f, 0.2f);
	body.setOrigin(Vector2f(body.getLocalBounds().width / 2.0f, body.getLocalBounds().height / 2.0f - 36.0f));
}
Player::~Player()
{

}

void Player::update(Time time, float& vx, float& vy, bool isSprint)
{
	input(vx, vy, isSprint);
	move(time, vx, vy);
}

void Player::move(Time time,float x, float y)
{
	body.move(Vector2f(x * time.asSeconds(), y * time.asSeconds()));
	//printf("%f %f\n", x * time.asSeconds(), y * time.asSeconds());
}

void Player::input(float &vx, float &vy,bool isSprint)
{
	vx = 0, vy = 0;
	if (Keyboard::isKeyPressed(Keyboard::W) && canMoveUP)
	{
		vy = -speed;
	}
	if (Keyboard::isKeyPressed(Keyboard::S)  && canMoveDown)
	{
		vy = speed;
	}
	if (Keyboard::isKeyPressed(Keyboard::A) && canMoveLeft)
	{

		vx=- speed;
	}
	if (Keyboard::isKeyPressed(Keyboard::D) && canMoveRight)
	{
		vx = speed;
	}

	if (isSprint)
	{
		vx *= 1.5f;
		vy *= 1.5f;
	}
}
