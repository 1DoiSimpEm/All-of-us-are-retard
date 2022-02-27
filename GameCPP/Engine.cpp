#include "Engine.h"

Engine::Engine(sf::RenderWindow& _window, View& _view, Image& _image,int &roundS) :
	vx(0), vy(0), angle(0), menu(false), isSprint(false), canSprint(true), window(&_window), view(&_view), image(&_image),
	pistol(Weapon("data/bullet.png", "data/gun.png", "Pistol", 80.0f, 16.0f, 2000.0f, 1.0f, Vector2f(2, 2), Vector2f(2, 2), player.body.getPosition(), 0.1f, 1.0f, "data/de_sound.wav")),
	rifle(Weapon("data/bullet.png", "data/rifle.png", "Rifle", 120.0f, 30.0f, 2800.0f, 2.0f, Vector2f(2, 2), Vector2f(2, 2), player.body.getPosition(), 0.08f, 2.0f, "data/ak_sound.wav")),
	gun(&pistol),
	switch_weapon(false),
	generated_zombie(0),
	round(1),
	generate(true),
	ptrRoundsSurvived(&roundS)
{}
bool Engine::runEngine()
{
	srand((unsigned int)time(NULL));
	while (!menu)
	{
		generator_manager();
		event_manager();
		update();
		collision();
		HUD();
		draw();	
	}
	return true;
}

void Engine::draw()
{
	window->setView(*view);
	window->clear();
	window->draw(map);
	window->draw(floor);
	//bullet drawing
	window->draw(gun->body);
	for (unsigned int i = 4; i < walls.size(); i++)
		window->draw(walls[i]);
	for (unsigned int i = 0; i < gun->bullets.size(); i++)
	{
		if (!gun->bullets[i].body.getGlobalBounds().intersects(player.body.getGlobalBounds()))
		{
			window->draw(gun->bullets[i].body);
		}
	}
	//draw UI
	if (gun->magazine <= gun->magazine_size / 3.0f && !gun->reloading)
	{
		UI[3].setFillColor(Color::Red);
		window->draw(UI[5]);
	}
	else if(!gun->reloading)
	{
		UI[3].setFillColor(Color::Green);
	}
	if (gun->reloading) window->draw(UI[6]);
	for (unsigned int i = 0; i < corps.size(); i++)// dead bodies
	{
		bool drawing = true;
		for (unsigned int j = 5; j < 10; j++)
		{

			if (corps[i].body.getGlobalBounds().intersects(walls[j].getGlobalBounds()))
				drawing = false;

		}
		if (drawing)window->draw(corps[i].body);
	}
	window->draw(player.body);

	//ammo drawing
	for (int i = 0; i < ammos.size(); i++)
	{
		if (ammos[i].onCreate_clocker.getElapsedTime().asSeconds() < ammos[i].lifeTime - 8.0f)
		{
			window->draw(ammos[i].body);
		}
		if (ammos[i].onCreate_clocker.getElapsedTime().asSeconds() >= ammos[i].lifeTime - 8.0f)
		{
			if (ammos[i].onCreate_clocker.getElapsedTime().asMilliseconds() % 500 >= 0 && ammos[i].onCreate_clocker.getElapsedTime().asMilliseconds() % 500 <= 125)
			{
				//disappearing effect
			}
			else window->draw(ammos[i].body);
		}
	}
	// health
	for (int i = 0; i < hps.size(); i++)
	{
		if (hps[i].onCreate_clocker.getElapsedTime().asSeconds() < hps[i].lifeTime - 8.0f)
		{
			window->draw(hps[i].body);
		}
		if (hps[i].onCreate_clocker.getElapsedTime().asSeconds() >= hps[i].lifeTime - 8.0f)
		{
			if (hps[i].onCreate_clocker.getElapsedTime().asMilliseconds() % 500 >= 0 && hps[i].onCreate_clocker.getElapsedTime().asMilliseconds() % 500 <= 125)
			{
				//disappearing effect
			}
			else window->draw(hps[i].body);
		}
	}

	for (unsigned int i = 0; i < zombies.size(); i++)//zombie
		window->draw(zombies[i].body);

	
	for (int i = 0; i < 5; i++)
	{
		window->draw(UI[i]);
	}
	window->draw(aim);
	//window->draw(sprint_hud);
	window->display();
}

void Engine::generator_manager()
{
	if (generate)max_zombie_per_round = round * 10;
	if (generated_zombie >= max_zombie_per_round) generate = false;
	if (!generate && zombies.empty())
	{
		zombie_killed = 0;
		generated_zombie = 0;
		generate = true;
		round++;
		round_sound.play();
	}
	if (generate && (zombie_generator_timer.getElapsedTime().asSeconds() >= 1.0f) && zombies.size() <= 20)
	{
		zombie_generator_timer.restart();
		generated_zombie += 5;
		zombie_generator(zombies, Vector2f((walls[0].getPosition().x + rand() % ((int)walls[0].getGlobalBounds().width + 1)), (walls[0].getPosition().y + walls[0].getGlobalBounds().height)), texture_zombie, round, walls[8],50.0f);
		zombie_generator(zombies, Vector2f((walls[1].getPosition().x + rand() % ((int)walls[1].getGlobalBounds().width + 1)), (walls[1].getPosition().y + walls[1].getGlobalBounds().height)), texture_zombie, round, walls[9],50.0f);
		zombie_generator(zombies, Vector2f((walls[2].getPosition().x + walls[2].getGlobalBounds().width), (walls[2].getPosition().y + rand() % ((int)walls[2].getGlobalBounds().height + 1))), texture_zombie, round, walls[10],50.0f);
		zombie_generator(zombies, Vector2f((walls[3].getPosition().x + walls[3].getGlobalBounds().width), (walls[3].getPosition().y + rand() % ((int)walls[3].getGlobalBounds().height + 1))), texture_zombie, round, walls[11],80.0f);
		zombie_generator(zombies, Vector2f((walls[3].getPosition().x + walls[3].getGlobalBounds().width), (walls[3].getPosition().y + rand() % ((int)walls[3].getGlobalBounds().height + 1))), texture_zombie, round, walls[11],100.0f);
	}

}


void Engine::update()
{
	*ptrRoundsSurvived = round - 1;
	//player
	view->setCenter(player.body.getPosition());
	//mouse coordinate
	mouse.x = (float)Mouse::getPosition(*window).x + view->getCenter().x - 1280 / 2, mouse.y = (float)Mouse::getPosition(*window).y + view->getCenter().y - 720 / 2;
	//crosshari
	aim.setPosition(mouse);
	angle = (float)atan2((float)(mouse.y - player.body.getPosition().y), (float)(mouse.x - player.body.getPosition().x)) * 180.0f / (float)3.14159265359 - 90.0f;
	player.body.setRotation(angle);
	if (tab_players.size() != 30) tab_players.push_back(player.body.getPosition());
	if (tab_players.size() == 30)
	{
		tab_players.pop_front();
		tab_players.push_back(player.body.getPosition());
	}
	sprint_manager();
	player.update(times,vx, vy, isSprint);	
	gun->update(angle, times, player.body.getPosition());
	for (int i = 0; i < zombies.size(); i++)
	{
		if (!zombies[i].goingThrough)
		{
			zombies[i].update(player.body.getPosition(), times, tab_players[0], zombies, walls);
		}
		else {
			zombies[i].update(Vector2f(0, 0), times, tab_players[0], zombies, walls);
		}
	}
	// zombie isInside update 
	for (unsigned int i = 0; i < zombies.size(); i++)
	{
		if (!zombies[i].goingThrough && zombies[i].body.getGlobalBounds().contains(zombies[i].target_wall.getPosition()))
			zombies[i].goingThrough = true;

	}

	for (unsigned int i = 0; i < zombies.size(); i++)
		if (zombies[i].goingThrough && !zombies[i].body.getGlobalBounds().intersects(zombies[i].target_wall.getGlobalBounds()))
		{
			zombies[i].goingThrough = false;
			zombies[i].isInside = true;
		}

	

	for (unsigned int i = 0; i < corps.size(); i++)
	{
		if (corps[i].onCreate_clock.getElapsedTime().asSeconds() >= corps[i].life_time)
		{
			 corps_destroy(i);
		}
	}
	for (unsigned int i = 0; i < ammos.size(); i++)
	{
		if (ammos[i].onCreate_clocker.getElapsedTime().asSeconds() >= ammos[i].lifeTime)
		{
			ammo_destroy(i);
		}
	}
	for (unsigned int i = 0; i < hps.size(); i++)
	{
		if (hps[i].onCreate_clocker.getElapsedTime().asSeconds() >= hps[i].lifeTime)
		{
			ammo_destroy(i);
		}
	}
	times = clocker.getElapsedTime();
	clocker.restart();

}

void Engine::event_manager()
{
	while (window->pollEvent(event))
	{
		if (gun == &pistol)
		{
			if (event.type == Event::MouseButtonPressed && Mouse::isButtonPressed(Mouse::Left))
			{
				cout << "SHOT"<<endl;
				cout << "BULLET LEFT:" << gun->bullets_left << endl;

				if (gun->magazine == 0)
				{
					if (gun->bullets_left != 0 && !gun->reload_demand && !gun->reloading)
					{
						gun->reload_demand = true;
					}
				}
				else if (!gun->reloading)
				{
					gun->shoot(player.body.getPosition());
					gun->magazine--;
				}
			}
		}
		if (event.type == Event::KeyPressed && event.key.code == Keyboard::R &&  gun->magazine != gun->magazine_size &&!gun->reloading && !gun->reload_demand)
		{
			cout << "RELOADING"<<endl;
			gun->reload_demand = true;
		}
		if (event.type == Event::KeyPressed && event.key.code == Keyboard::Num2)
		{
			cout << "SWTICH TO AK" << endl;
			gun = &rifle;
			switch_weapon = true;
		}
		if (event.type == Event::KeyPressed && event.key.code == Keyboard::Num1)
		{
			cout << "SWTICH TO PISTOL"<<endl;
			gun = &pistol;
			switch_weapon = false;
		}
		if (gun != &pistol)
		{
			if (event.type == Event::MouseButtonPressed && Mouse::isButtonPressed(Mouse::Left))
			{
				cout << "SHOT" << endl;
				cout << "BULLET LEFT:" << gun->magazine << endl;
				if (gun->magazine == 0)
				{
					if (gun->bullets_left != 0 && !gun->reload_demand && !gun->reloading)
					{
						gun->reload_demand = true;
					}
				}
				else if (!gun->reloading)
				{
					gun->shoot(player.body.getPosition());
					gun->magazine--;
				}
			}
		}
	}
}


void Engine::corps_destroy(int i)
{

	corps[i] = corps[corps.size() - 1];
	corps.pop_back();

}

void Engine::sprint_manager()
{
	if (Keyboard::isKeyPressed(Keyboard::LShift) && canSprint)
	{
		if (sprint_hud.getSize().x < 1) canSprint = false;
		isSprint = true;
	}
	else
	{
		if (sprint_hud.getSize().x > 15)
			canSprint = true;
		isSprint = false;
	}
}
bool Engine::init()
{
	window->setMouseCursorVisible(false);
	//load
	texture_floor.loadFromFile("data/metal_floor_prev.jpg");
	texture_aim.loadFromFile("data/aim.png");
	texture_map.loadFromFile("data/ground.png");\
	texture_corp.loadFromFile("data/corps.png");
	texture_ammo.loadFromFile("data/ammo_pickup.png");
	texture_hp.loadFromFile("data/hp_pickup.png");
	texture_floor.setRepeated(true);
	texture_map.setRepeated(true);
	floor.setTextureRect(IntRect(0, 0, 1000, 1000));
	//set
	aim.setScale(0.04f, 0.04f);
	aim.setTexture(texture_aim);
	aim.setOrigin(aim.getLocalBounds().width / 2.0f, aim.getLocalBounds().height / 2.0f);
	floor.setPosition(-500, -500);
	floor.setTexture(texture_floor);
	map.setTexture(texture_map);
	map.setTextureRect(IntRect(0, 0, 10000, 10000));
	map.setOrigin(5000, 5000);
	map.setPosition(0, 0);
	player.body.setPosition(0.f, 0.0f);
	//sound
	round_buffer.loadFromFile("data/round.wav");
	round_buffer2.loadFromFile("data/round2.wav");
	round_sound.setBuffer(round_buffer2);
	round_sound.setVolume(30);
	bite_buffer.loadFromFile("data/bite.wav");
	bite_sound.setBuffer(bite_buffer);
	bite_sound.setVolume(40);
	hit_bufer.loadFromFile("data/hit.wav");
	hit_sound.setBuffer(hit_bufer);
	hit_sound.setVolume(50);
	if (!music.openFromFile("data/music.wav"))
		cout << "CANNOT LOAD MUSIC" << "\n";
	music.setVolume(40);
	music.play();
	music.setLoop(true);
	hp_pickup_buffer.loadFromFile("data/hp.wav");
	hp_pickup.setBuffer(hp_pickup_buffer);
	hp_pickup.setVolume(50);
	ammo_pickup_buffer.loadFromFile("data/ammo.wav");
	ammo_pickup.setBuffer(ammo_pickup_buffer);
	ammo_pickup.setVolume(50);



	// HUD
	if (!font.loadFromFile("data/font_ui.otf"))
	{
		cout << "ERROR LOADING UI FONT" << endl;
		return false;
	}
	if (!texture_zombie.loadFromFile("data/zombies.png"))
	{
		return false;
	}
	//wall and gate
	entrance.loadFromFile("data/floor_entrance.png");
	wall_t.loadFromFile("data/wall.png");


	for (int i = 0; i < 12; i++)
	{
		RectangleShape wall;


		if (i == 0)
		{
			wall.setPosition(-1000, -1000);
		}
		else if (i == 1) {
			wall.setPosition(-1000, 1000);
		}
		else if (i == 2) {
			wall.setPosition(-1100, -1000);
		}
		else if (i == 3) {
			//wall.setOrigin(100, 0);
			wall.setPosition(1000, -1000);
			//wall.setFillColor(Color::Blue);
		}
		if (i == 0 || i == 1)wall.setSize(Vector2f(2000, 100));
		else if (i == 2 || i == 3)wall.setSize(Vector2f(100, 2100));
		if (i == 4 || i == 5)wall.setSize(Vector2f(1020, 20));
		else if (i == 6 || i == 7)wall.setSize(Vector2f(20, 1020));
		if (i == 4)

		{
			wall.setOrigin(Vector2f(wall.getGlobalBounds().width / 2.0f, wall.getGlobalBounds().height / 2.0f));
			wall.setPosition(0, -500);
			wall.setTexture(&wall_t);
			wall.setTextureRect(IntRect(0, 0, wall.getSize().x, wall.getSize().y));

		}
		else if (i == 5)
		{
			wall.setOrigin(Vector2f(wall.getGlobalBounds().width / 2.0f, wall.getGlobalBounds().height / 2.0f));
			wall.setPosition(-20, 500);
			wall.setTexture(&wall_t);
			wall.setTextureRect(IntRect(0, 0, wall.getSize().x, wall.getSize().y));
		}
		else if (i == 6)
		{
			wall.setOrigin(Vector2f(wall.getGlobalBounds().width / 2.0f, wall.getGlobalBounds().height / 2.0f));
			wall.setPosition(-520, 0);
			wall.setTexture(&wall_t);
			wall.setTextureRect(IntRect(0, 0, wall.getSize().x, wall.getSize().y));
		}
		else if (i == 7)
		{
			wall.setOrigin(Vector2f(wall.getGlobalBounds().width / 2.0f, wall.getGlobalBounds().height / 2.0f));
			wall.setPosition(500, 0);
			wall.setTexture(&wall_t);
			wall.setTextureRect(IntRect(0, 0, wall.getSize().x, wall.getSize().y));
		}

		if (i == 8 || i == 9)wall.setSize(Vector2f(100, 20));
		else if (i == 10 || i == 11)wall.setSize(Vector2f(20, 100));
		if (i == 8)
		{
			wall.setOrigin(Vector2f(wall.getGlobalBounds().width / 2.0f, wall.getGlobalBounds().height / 2.0f));
			wall.setPosition(0, -500);
			wall.setTexture(&entrance);
			wall.setTextureRect(IntRect(0, 0, wall.getSize().x, wall.getSize().y));
		}
		else if (i == 9)
		{
			wall.setOrigin(Vector2f(wall.getGlobalBounds().width / 2.0f, wall.getGlobalBounds().height / 2.0f));
			wall.setPosition(-20, 500);
			wall.setTexture(&entrance);
			wall.setTextureRect(IntRect(0, 0, wall.getSize().x, wall.getSize().y));
		}
		else if (i == 10)
		{
			wall.setOrigin(Vector2f(wall.getGlobalBounds().width / 2.0f, wall.getGlobalBounds().height / 2.0f));
			wall.setPosition(-520, 0);
			wall.setTexture(&entrance);
			wall.setTextureRect(IntRect(0, 0, wall.getSize().x, wall.getSize().y));
		}
		else if (i == 11)
		{
			wall.setOrigin(Vector2f(wall.getGlobalBounds().width / 2.0f, wall.getGlobalBounds().height / 2.0f));
			wall.setPosition(500, 0);
			wall.setTexture(&entrance);
			wall.setTextureRect(IntRect(0, 0, wall.getSize().x, wall.getSize().y));
		}



		walls.push_back(wall);

	}



	//sound process;
	
	zombie_generator_timer.restart();
	window->setMouseCursorVisible(false);




	return true;
}
void Engine::collision()
{
	//zombie colliding
	for (unsigned int i = 0; i < zombies.size(); i++)
	{
		if (Collision::PixelPerfectTest(player.body, zombies[i].body))
		{
			zombies[i].collidesWithPlayer = true;

			if (((zombies[i].body.getPosition().y > player.body.getPosition().y) && player.body.getPosition().y > 0) || (player.body.getPosition().y < zombies[i].body.getPosition().y && zombies[i].body.getPosition().y < 0))
			{
				player.canMoveDown = false;
			}

			if ((player.body.getPosition().y > zombies[i].body.getPosition().y && zombies[i].body.getPosition().y > 0) || (zombies[i].body.getPosition().y < player.body.getPosition().y && player.body.getPosition().y < 0))
			{
				player.canMoveUP = false;
			}

			if ((zombies[i].body.getPosition().x > player.body.getPosition().x && player.body.getPosition().x > 0) || (player.body.getPosition().x < zombies[i].body.getPosition().x && zombies[i].body.getPosition().x < 0))
			{
				player.canMoveRight = false;
			}

			if ((player.body.getPosition().x > zombies[i].body.getPosition().x && zombies[i].body.getPosition().x > 0) || (zombies[i].body.getPosition().x < player.body.getPosition().x && player.body.getPosition().x < 0))
			{
				player.canMoveLeft = false;
			}


			if (zombies[i].playerCollisionTimer.getElapsedTime().asSeconds() >= zombies[i].biteFrequency) {

				zombies[i].playerCollisionTimer.restart();

				bite_sound.play();
				player.healthPoints--;

				if (player.healthPoints == 0)
				{
					cout << "DEAD" << "\n";
					menu = true;
					
				}
				break;
			}
		}
		else zombies[i].collidesWithPlayer = false;
	}

	if (walls[4].getGlobalBounds().intersects(player.body.getGlobalBounds()) || walls[4].getGlobalBounds().intersects(gun->body.getGlobalBounds()))
	{
		player.canMoveUP = false;
	}
	else
	{
		player.canMoveUP = true;
	}

	if (walls[5].getGlobalBounds().intersects(player.body.getGlobalBounds()) || walls[5].getGlobalBounds().intersects(gun->body.getGlobalBounds()))
	{
		player.canMoveDown = false;
	}
	else
	{
		player.canMoveDown = true;
	}

	if (walls[6].getGlobalBounds().intersects(player.body.getGlobalBounds()) || walls[6].getGlobalBounds().intersects(gun->body.getGlobalBounds()))
	{
		player.canMoveLeft = false;
	}
	else
	{
		player.canMoveLeft = true;
	}

	if (walls[7].getGlobalBounds().intersects(player.body.getGlobalBounds()) || walls[7].getGlobalBounds().intersects(gun->body.getGlobalBounds()))
	{
		player.canMoveRight = false;
	}
	else
	{
		player.canMoveRight = true;
	}



//zombie dead

	if (!zombies.empty() && !gun->bullets.empty())
	{
		for (unsigned int i = 0; i < gun->bullets.size(); i++)
		{
			for (unsigned int j = 0; j < zombies.size(); j++)
			{
				if (gun->bullets[i].body.getGlobalBounds().intersects(zombies[j].body.getGlobalBounds()))
				{

					bullet_destroy(i);
					zombies[j].healthPoints -= gun->damage;
					zombies[j].speed -= 10;
					hit_sound.play();
					if (zombies[j].healthPoints <= 0)
					{	
						Corp corpses(texture_corp, zombies[j].body.getPosition(), zombies[j].rotate_angle);
						corps.push_back(corpses);

						if (zombies[j].isInside)
						{
							Ammo_Pickup ammo(texture_ammo, zombies[j].body.getPosition());
							HP_Pickup hp(texture_hp, zombies[j].body.getPosition());
							for (unsigned int i = 4; i < 8; i++)
							{
								if (corpses.body.getGlobalBounds().intersects(walls[i].getGlobalBounds())) {

									corpses.body.move((Vector2f(0, 0) - corpses.body.getPosition()) * 0.3f);
								}
								if (ammo.body.getGlobalBounds().intersects(walls[i].getGlobalBounds()))
									ammo.body.move((floor.getPosition() - ammo.body.getPosition()) * 0.4f);
								if (hp.body.getGlobalBounds().intersects(walls[i].getGlobalBounds()))
									hp.body.move((floor.getPosition() - hp.body.getPosition()) * 0.4f);
							}
							switch (rand() % 15)
							{
							case 0:
								if (gun == &pistol)
								{
									if (gun->bullets_left < 96 && ammos.size() < 2)ammos.push_back(ammo);

								}
								else
								{
									if (gun->bullets_left < 150 && ammos.size() < 2)ammos.push_back(ammo);
								}
								break;
							case 1:
								if (gun == &pistol)
								{
									if (gun->bullets_left < 96 && ammos.size() < 2)ammos.push_back(ammo);

								}
								else
								{
									if (gun->bullets_left < 150 && ammos.size() < 2)ammos.push_back(ammo);
								}								break;
							case 2:
								if (player.healthPoints + hps.size() <= 3 && hps.size() == 0) hps.push_back(hp);
								break;
							case 3:
								if (player.healthPoints + hps.size() <= 3 && hps.size() == 0) hps.push_back(hp);
								break;
							case 4:
								if (player.healthPoints + hps.size() <= 3 && hps.size() == 0) hps.push_back(hp);
								break;
							}
							
						}
						zombie_destroy(j);
						zombie_killed++;
						break;
					}
					break;
				}
			}
		}
	}
	//pick up
	for (int i = 0; i < ammos.size(); i++)
	{
		if (player.body.getGlobalBounds().intersects(ammos[i].body.getGlobalBounds()))
		{
			ammo_pickup.play();
			gun->bullets_left += (int)gun->magazine_size / 2+(round*(int)gun->magazine_size/3);
			ammo_destroy(i);
		}
	}
	for (int i = 0; i < hps.size(); i++)
	{
		if (player.body.getGlobalBounds().intersects(hps[i].body.getGlobalBounds()))
		{
			if (player.healthPoints + hps[i].add_hp <= 5)
			{
				hp_pickup.play();
				player.healthPoints += hps[i].add_hp;
				hp_destroy(i);
			}
		}
	}
	for (unsigned int j = 4; j < walls.size() - 4; j++)
		for (unsigned int i = 0; i < gun->bullets.size(); i++)
		{

			if (gun->bullets[i].body.getGlobalBounds().intersects(walls[j].getGlobalBounds()) || !gun->bullets[i].body.getGlobalBounds().intersects(floor.getGlobalBounds()))	
			{
				bullet_destroy(i);
			}
		}

	//sprint hud
	sprint_hud.setSize(Vector2f(15 * 5, 15));
	sprint_hud.setFillColor(Color(138, 7, 7));


}
void Engine::HUD()
{
	for (int i = 0; i < 5; i++)
	{
		UI[i].setFont(font);
		UI[i].setCharacterSize(25);
		UI[i].setFillColor(Color(138, 7, 7));
	}
	UI[0].setFillColor(Color::Green);
	if (player.healthPoints == 3 || player.healthPoints == 2) UI[0].setFillColor(Color::Yellow);
	if (player.healthPoints == 1) UI[0].setFillColor(Color::Red);
	UI[5].setFont(font);
	UI[3].setFillColor(Color::Green);
	UI[1].setString("round: " + to_string(round));
	if (zombie_killed == 0) UI[1].setFillColor(Color::Yellow);
	UI[1].setPosition(view->getCenter().x - 1280 / 2 + 10, view->getCenter().y + 360 - 40);
	UI[5].setCharacterSize(20);
	UI[5].setFillColor(Color::Yellow);
	UI[6].setFont(font);
	UI[6].setCharacterSize(20);
	UI[6].setFillColor(Color(138, 7, 7));
	UI[6].setString("reloading...");
	UI[5].setString("press 'r' to reload");
	UI[0].setString("hp: " + to_string((int)player.healthPoints));
	UI[3].setString("ammo: " + to_string((int)gun->magazine) + " / " + to_string((int)gun->bullets_left));
	UI[0].setPosition(view->getCenter().x + 1280 / 2 - 80, view->getCenter().y - 360 + 20);
	UI[3].setPosition(view->getCenter().x + 1280 / 2 - 220, view->getCenter().y + 360 - 40);
	UI[2].setString(gun->name);
	UI[2].setPosition(view->getCenter().x + 1280 / 2 - 220, view->getCenter().y + 360 - 80);
	UI[4].setString("zombies left to kill: " + to_string(max_zombie_per_round - zombie_killed));
	UI[4].setPosition(view->getCenter().x - 1280 / 2 + 10, view->getCenter().y - 360 + 20);
	UI[5].setPosition(view->getCenter().x + 1280 / 2 - 550, view->getCenter().y + 360 - 40);
	UI[6].setPosition(view->getCenter().x + 1280 / 2 - 450, view->getCenter().y + 360 - 40);
	sprint_hud.setPosition(view->getCenter().x + 1280 / 2 - 90, view->getCenter().y - 360 + 70);
	if (canSprint && tab_players[0] != player.body.getPosition() && sprint_hud.getSize().x > 0)
	{
		sprint_hud.setSize(Vector2f(sprint_hud.getGlobalBounds().width - (15.0f * times.asSeconds()), sprint_hud.getGlobalBounds().height));
	}
	else
	{
		if (sprint_hud.getSize().x <= 15 * 5) sprint_hud.setSize(Vector2f(sprint_hud.getGlobalBounds().width + (7.5f * 0.5f * times.asSeconds()), sprint_hud.getGlobalBounds().height));
	}
}
void Engine::zombie_generator(vector<Zombie>& zombies, Vector2f pos, Texture& m_texture, int m_round, RectangleShape& target_wal,float speed)
{
	Zombie zombie( pos,m_texture,(m_round*0.25f)+speed, rand() % (m_round)+1, target_wal);
	zombies.push_back(zombie);
}

void Engine::bullet_destroy(int i)
{
	gun->bullets[i] = gun->bullets[gun->bullets.size() - 1];
	gun->bullets.pop_back();
}
void Engine::zombie_destroy(int i)
{
	zombies[i] = zombies[zombies.size() - 1];
	zombies.pop_back();
}

void Engine::ammo_destroy(int i)
{
	ammos[i] = ammos[ammos.size() - 1];
	ammos.pop_back();
}

void Engine::hp_destroy(int i)
{
	hps[i] = hps[hps.size() - 1];
	hps.pop_back();
}