#include "Bullet.h"
Bullet::Bullet(float alpha, Vector2f pos, Texture& texture) :speed(1000.0f)
{
	clocker.restart();
	angle = alpha;
	body.setTexture(texture);
	body.setPosition(pos);
	body.setRotation(angle);
	body.setScale(0.02f, 0.02f);
	body.setOrigin(body.getLocalBounds().width / 2.0f, body.getLocalBounds().height / 2.0f);

}

void Bullet::move(Time time)
{
	body.move(Vector2f((float)-sin(angle * M_PI / 180) * speed * (float)time.asSeconds(), (float)cos(angle * M_PI / 180) * speed * (float)time.asSeconds()));
}
