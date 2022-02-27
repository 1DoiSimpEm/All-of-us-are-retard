#include "Weapon.h"

Weapon::Weapon()
{
}
Weapon::~Weapon()
{
}
Weapon::Weapon(string bullet_path, string gun_path,string _name, float bullets_number, float _magazine_size, float _bullet_speed, float _reload_time, Vector2f _gun_scale, Vector2f _bullet_scale,Vector2f player_pos, float _fire_rate, float _damage,string sound_path)
	:reload_demand(false),angle(0),name(_name),bullets_left(bullets_number),magazine(_magazine_size), magazine_size(_magazine_size), bullet_speed(_bullet_speed), reload_time(_reload_time), gun_scale(_gun_scale), bullet_scale(_bullet_scale), fire_rate(_fire_rate), reloading(false), damage(_damage)
{
	buffer.loadFromFile(sound_path);
	sound.setBuffer(buffer);
	sound.setVolume(50.0f);
	texture_bullet.loadFromFile(bullet_path);
	texture_gun.loadFromFile(gun_path);
	body.setTexture(texture_gun);
	body.setOrigin(body.getLocalBounds().width, 0);
	body.setPosition(player_pos);
	body.setScale(gun_scale / 10.0f);
	reload_buffer.loadFromFile("data/reload.wav");
	reload_sound.setBuffer(reload_buffer);
}


void Weapon::shoot(Vector2f player_position)
{
	sound.play();
	body.setOrigin(body.getLocalBounds().width / 2.0f, body.getLocalBounds().height / 2.0f);
	Bullet bullet(angle, player_position, texture_bullet);
	bullets.push_back(bullet);
}


void Weapon::reload()
{
	reload_sound.play();
	if (magazine != magazine_size)
	{
		bullets_left -= magazine_size;
		magazine += magazine_size;
		if (bullets_left < 0)
		{
			magazine += bullets_left;
			bullets_left = 0;
		}
		if (magazine > magazine_size)
		{
			bullets_left += magazine - magazine_size;
			magazine = magazine_size;
		}
		if (magazine < 0) magazine = 0;
	}
}

void Weapon::update(float Angle, Time time, Vector2f player_pos)
{
	angle = Angle;
	body.setOrigin(body.getLocalBounds().width, 0);
	body.setPosition(player_pos);
	body.setRotation(angle);
	for (size_t i = 0; i < bullets.size(); i++)
	{
		bullets[i].body.move(Vector2f((float)-sin(bullets[i].angle * M_PI / 180) * bullet_speed * (float)time.asSeconds(), (float)cos(bullets[i].angle * M_PI / 180) * bullet_speed * (float)time.asSeconds()));
	}
	if (reload_demand)
	{
		if (!reloading)
		{
			reload_timer.restart();
			reloading = true;
		}
		if (reload_timer.getElapsedTime().asSeconds() >= reload_time)
		{
			reload();
			reloading = false;
			reload_demand = false;
		}
	}
}

