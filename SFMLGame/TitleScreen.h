#pragma once
#include "GameScreen.h"

class TitleScreen : public GameScreen
{
public:
	TitleScreen();
	~TitleScreen();

	void LoadContent();
	void UnloadContent();

	void Update(sf::Event event);
	void Draw(sf::RenderWindow &window);

private:
	sf::CircleShape shape;
};