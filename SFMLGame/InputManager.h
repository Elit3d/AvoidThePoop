#pragma once
#include <vector>
#include "GameScreen.h"

class InputManager
{
public:
	InputManager();
	~InputManager();

	void Update(sf::Event event);

	bool KeyPressed(int key);
	bool KeyPressed(std::vector<int> keys);
	bool KeyReleased(int key);
	bool KeyReleased(std::vector<int> keys);
	/*
	bool KeyDown(sf::RenderWindow &window, int key);
	bool KeyDown(std::vector<int> keys); */
private:
	sf::Event event;
};

