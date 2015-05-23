#include "ScreenManager.h"

GameScreen *currentScreen, *newScreen; //to not keep creating screens

ScreenManager::ScreenManager()
{
}

ScreenManager &ScreenManager::GetInstance() {
	static ScreenManager instance;
	return instance;
}

ScreenManager::~ScreenManager()
{
}

void ScreenManager::Initialize(){
	currentScreen = new SplashScreen();
}

void ScreenManager::LoadContent(){
	currentScreen->LoadContent();
}

void ScreenManager::Update(sf::Event event){
	currentScreen->Update(event);
}

void ScreenManager::Draw(sf::RenderWindow &window){
	currentScreen->Draw(window);
}

void ScreenManager::AddScreen(GameScreen *screen){
	currentScreen->UnloadContent();
	delete currentScreen;
	currentScreen = screen;
	currentScreen->LoadContent();
}
