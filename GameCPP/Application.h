#pragma once
#include "GameStateMachine.h"
#include "ResourceManager.h"

class Application {
public:
	void Run();
	~Application();
private:
	void Init();
	void Update(float deltaTime);
	void Render();
private:
	sf::RenderWindow* m_window;
};