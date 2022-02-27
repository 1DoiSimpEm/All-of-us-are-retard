#include "Game.h"
#include "Engine.h"
Game::Game(void):roundsSurvived(0)
{	
	ContextSettings settings;
	settings.antialiasingLevel = 8;
	view = View(FloatRect(0, 0, 1280, 720));
	window.setFramerateLimit(60);
	window.setVerticalSyncEnabled(true);
	window.create(VideoMode(1280, 720), "All of us are retard!",Style::Close);
	if (!font.loadFromFile("data/font.ttf")) {
		cout << "FOUND NO FONT!" << endl;
	}
	button_buffer.loadFromFile("data/button.wav");
	button_sound.setBuffer(button_buffer);
	state = INTRO;

}
Game::~Game(void)
{

}
void Game::run()
{
	while (state != END)
	{
		switch (state)
		{
		case GameState::INTRO:
				intro();
				break;
		case GameState::MENU:
			menu();
			break;
		
		case GameState::GAME:
			play();
			break;
		case GameState::HELP:
			help();
			break;
		case GameState::FAILURE:
			failure();
			break;
		}
	}
}
void Game::intro()
{
	Texture texture;
	texture.loadFromFile("data/intro.png");
	Sprite background;
	background.setTexture(texture);
	EEDelayedText text2("Press enter to continue",font,80);
	text2.setFillColor(Color(133,7,7));
	text2.setPosition((float)(1280 / 4.0 - text2.getGlobalBounds().width / 2), (float)(250 + 3 * 100.0));
	text2.setDelay(seconds(2.f / 10.f));
	while (state == INTRO)
	{
		window.setView(window.getDefaultView());
		Vector2f mouse(Mouse::getPosition(window));
		window.setMouseCursorVisible(true);
		Event event;
		while (window.pollEvent(event))
		{
			if (event.type == Event::Closed || event.type == Event::KeyPressed &&
				event.key.code == Keyboard::Enter)
				state = MENU;
		}
		window.clear();
		window.draw(background);
		window.draw(text2.update());
		window.display();
	}
}
void Game::failure()
{
	Texture texture;
	texture.loadFromFile("data/die.jpg");
	Sprite background;
	background.setTexture(texture);
	dead_buffer.loadFromFile("data/dead.wav");
	dead_sound.setBuffer(dead_buffer);
	dead_sound.setVolume(70);
	dead_sound.play();
	EEDelayedText text1("Play again",font,60);
	text1.setPosition((float)(1280 / 2.0 - text1.getGlobalBounds().width / 2-140), 225);
	text1.setFillColor(Color::Yellow);
	text1.setDelay(seconds(1.f / 10.f));
	EEDelayedText text2("exit to menu", font, 60);
	text2.setFillColor(Color::Yellow);
	text2.setPosition((float)(1280 / 2.0 - text2.getGlobalBounds().width / 2-140), 225 + 300);
	text2.setDelay(seconds(2.f / 10.f));
//	for (int i = 0; i < 2; i++)
	//{
		//text[i].setFont(font);
		//text[i].setCharacterSize(60);
		//text[i].setFillColor(Color::Yellow);
		//text[i].setString(str[i]);
		//text[i].setPosition((float)(1280 / 2.0 - text[i].getGlobalBounds().width / 2), 225 + (float)( i * 250.0));
	//}
	while (state == FAILURE)
	{
		clock.restart();
		window.setView(window.getDefaultView());
		Vector2f mouse(Mouse::getPosition(window));
		window.setMouseCursorVisible(true);
		Event event;
		while (window.pollEvent(event))
		{
			if (event.type == Event::Closed || event.type == Event::KeyPressed &&
				event.key.code == Keyboard::Escape)
			{
				state = MENU;
				button_sound.play();
			}
			else if (text1.getGlobalBounds().contains(mouse) &&
				event.type == Event::MouseButtonReleased && event.key.code == Mouse::Left)
			{
				state = GAME;
				button_sound.play();
			}
			else if (text2.getGlobalBounds().contains(mouse) &&
				event.type == Event::MouseButtonReleased && event.key.code == Mouse::Left)
			{
				state = MENU;
				button_sound.play();
			}
		}
			if (text1.getGlobalBounds().contains(mouse))
			{
				text1.setFillColor(Color::Green);
			}
			else text1.setFillColor(Color::White);
			if (text2.getGlobalBounds().contains(mouse))
			{
				text2.setFillColor(Color::Green);
			}
			else text2.setFillColor(Color::Red);

		
			window.clear();
			window.draw(background);
			window.draw(text1.update());
			window.draw(text2.update());
			window.display();
		
	}
}

void Game::help()
{
	Text tutorial("How to survive", font, 40);
	tutorial.setFillColor(Color::Yellow);
	tutorial.setPosition(1280 / 2 - tutorial.getGlobalBounds().width / 2, 125);
	Texture texture;
	texture.loadFromFile("data/help_menu.png");
	Sprite background;
	background.setTexture(texture);
	background.setPosition(Vector2f(0, 0));
	background.setOrigin(Vector2f(0, 0));
	background.setTextureRect(IntRect(0, 0, 1280, 720));
	Text text[5];
	String str[] = { "You have been trapped in a mysterious room full of retard zombies","All you gotta do is stay alive by killing all of them","Some zombies will drop supplies after being killed by your bullet","Press 1 to use pistol press 2 to use rifle","Do not get caught by zombie or you will be dead GOOD LUCK"};
	for (int i = 0; i < 5; i++)
	{
		text[i].setFont(font);
		text[i].setCharacterSize(30);
		text[i].setFillColor(Color::Black);
		text[i].setString(str[i]);
		text[i].setPosition((float)(1280 / 2.0 - text[i].getGlobalBounds().width / 2), (float)(250 + i * 100.0));
	}
	while (state == HELP)
	{
		Vector2f mouse(Mouse::getPosition(window));
		Event event;
		while (window.pollEvent(event))
		{
			if (event.type == Event::Closed || event.type == Event::KeyPressed &&
				event.key.code == Keyboard::Escape)
				state = MENU;
		}
		window.clear();
		window.draw(background);
		window.draw(tutorial);
		for (int i = 0; i < 5; i++)
			window.draw(text[i]);
		window.display();
	}
}

void Game::menu()
{
	if (!music.openFromFile("data/menu_music.wav")) cout << "Cant find music" << endl;
	music.play();
	music.setLoop(true);
	Text title("All of us are retard", font, 80);
	texture.loadFromFile("data/bg.png");
	texture.setRepeated(true);
	Sprite background;
	background.setTexture(texture);
	Texture texture;
	background.setPosition(Vector2f(0, 0));
	background.setOrigin(Vector2f(0, 0));
	background.setTextureRect(IntRect(0, 0, 1280, 720));
	Image image;
	image.loadFromFile("data/image.png");
	window.setIcon(498, 498, image.getPixelsPtr());
	title.setPosition(1280 / 2 - title.getGlobalBounds().width / 2, 10);
	title.setFillColor(Color::Red);
	Text text[4];
	String str[] = { "PLAY","HOW TO PLAY","EXIT","The game is still in beta"};
	for (int i = 0; i < 4; i++)
	{
		text[i].setFont(font);
		text[i].setCharacterSize(40);
		text[i].setFillColor(Color::Black);
		text[i].setString(str[i]);
		text[i].setPosition((float)(1280 / 5.0 - text[i].getGlobalBounds().width / 2), (float)(250 + i * 120.0));
	}
	while (state == MENU)
	{
		Vector2f mouse(Mouse::getPosition(window));
		Event event;
		while (window.pollEvent(event))
		{
			if (event.type == Event::Closed || event.type == Event::KeyPressed &&
				event.key.code == Keyboard::Escape)
			{
				state = END;
				button_sound.play();
			}

			else if (text[2].getGlobalBounds().contains(mouse) &&
				event.type == Event::MouseButtonReleased && event.key.code == Mouse::Left)
			{
				state = END;
				button_sound.play();
			}

			else if (text[0].getGlobalBounds().contains(mouse) &&
				event.type == Event::MouseButtonReleased && event.key.code == Mouse::Left)
			{
				state = GAME;
				button_sound.play();
			}
			else if (text[1].getGlobalBounds().contains(mouse) &&
				event.type == Event::MouseButtonReleased && event.key.code == Mouse::Left)
			{
				state = HELP;
				button_sound.play();
			}
		}
		Texture t1;
		t1.loadFromFile("data/image.png");
		Sprite sp1(t1);
		sp1.setPosition(1280 / 4 - sp1.getGlobalBounds().width / 2, 40);
		sp1.setScale(0.25f, 0.25f);
		for (int i = 0; i < 3; i++)
			if (text[i].getGlobalBounds().contains(mouse))
			{
				text[i].setFillColor(Color::Green);
			}
			else text[i].setFillColor(Color(133,7,7));
		window.clear();
		window.draw(background);
		window.draw(sp1);
		window.draw(title);

		for (int i = 0; i < 4; i++)
			window.draw(text[i]);
		
		window.display();

	}



}

void Game::play()
{
	music.stop();
	Engine engine(window, view, image, roundsSurvived);
	if (engine.init())
	{
		if (engine.runEngine())
		{
			state = FAILURE;
		}
		
	}
}
