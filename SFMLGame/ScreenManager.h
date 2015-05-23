#pragma once
#include <string>
#include <iostream>
#include "GameScreen.h"
#include "SplashScreen.h"
#include "TitleScreen.h"

class ScreenManager
{
public:
	~ScreenManager();
	static ScreenManager &GetInstance();

	void Initialize();
	void LoadContent();
	void UnloadContent();
	void Update(sf::Event event);
	void Draw(sf::RenderWindow &window);

	void AddScreen(GameScreen *screen);
private:
	//GameScreen *currentScreen, *newScreen;

	ScreenManager();
	ScreenManager(ScreenManager const&);
	void operator = (ScreenManager const&);
};

