#include "FailureState.h"

FailureState::FailureState(RenderWindow& _window, Font& _font, int _rounds) :
	menu(false),
	window(&_window),
	font(&_font),
	playAgain(false),
	failure(true),
	rounds(_rounds)
{
	window->setMouseCursorVisible(true);
}

FailureState::~FailureState()
{
}

bool FailureState::run(Image  &_image)
{
	clock.restart();
	window->setView(window->getDefaultView());
	Text title("You died", *font, 80);
	title.setFillColor(Color(138, 7, 7));
	title.setPosition(1280 / 2 - title.getGlobalBounds().width / 2, 10);
	Text infor("You have survived" + to_string(rounds) + " rounds", *font, 40);
	infor.setFillColor(Color(138, 7, 7));
	infor.setOrigin(Vector2f(infor.getGlobalBounds().width / 2.f, infor.getGlobalBounds().height / 2.f));
	infor.setPosition(1280 / 2, 110);
	image = &_image;
	texture.loadFromImage(*image);
	Sprite background;
	background.setTexture(texture);
	background.setPosition(Vector2f(0, 0));
	background.setOrigin(Vector2f(0, 0));
	background.setTextureRect(IntRect(0, 0, 1280, 720));
	Text text[2];
 	string str={"play again", "return to menu"};
	for (int i = 0; i < 2; i++)
	{
		text[i].setFont(*font);
		text[i].setCharacterSize(50);
		text[i].setString(str[i]);
		text[i].setPosition((float)(1280 / 2.0 - text[i].getGlobalBounds().width / 2), (float)(250 + i * 120.0));
	}
	while (failure)
	{
		Vector2f mouse(Mouse::getPosition(*window));
		while (window->pollEvent(event))
		{
			if (clock.getElapsedTime().asSeconds() >= 1.0f)
			{
				if (text[0].getGlobalBounds().contains(mouse) && event.type == Event::MouseButtonReleased && event.key.code == Mouse::Left)
				{
					failure = false;
					playAgain = true;
				}
				else if (text[1].getGlobalBounds().contains(mouse) && event.type == Event::MouseButtonReleased && event.key.code == Mouse::Left)
				{
					failure = false;
					menu = true;
				}
			}
		}
		for (int i = 0; i < 2; i++)
			if (text[i].getGlobalBounds().contains(mouse))
			{
				text[i].setFillColor(Color::Green);
			}
			else text[i].setFillColor(Color::White);
		window->clear();
		window->draw(background);
		window->draw(title);
		window->draw(infor);
		for (int i = 0; i < 2; i++) window->draw(text[i]);
		window->display();

	}
	if (menu) return false;
	else if (playAgain) return true;
}
