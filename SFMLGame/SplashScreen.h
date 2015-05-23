#pragma once
#include "GameScreen.h"

class SplashScreen : public GameScreen
{
public:
	SplashScreen();
	~SplashScreen();

	void LoadContent();
	void UnloadContent();

	void Update(sf::Event event);
	void Draw(sf::RenderWindow &window);

private:
	sf::CircleShape shape;
};

