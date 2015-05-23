#ifndef GAMESCREEN_H
#define GAMESCREEN_H
#include <SFML/Graphics.hpp>
#include <iostream>
#include "InputManager.h"
#include "ScreenManager.h"

class GameScreen
{
public:
	GameScreen();
	~GameScreen();

	virtual void LoadContent();
	virtual void UnloadContent();

	virtual void Update(sf::Event event);
	virtual void Draw(sf::RenderWindow &window);
protected:
	InputManager input;
	std::vector<int> keys;
};

#endif // GAMESCREEN_H
