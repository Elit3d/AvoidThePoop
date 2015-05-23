#include "SplashScreen.h"


SplashScreen::SplashScreen()
{
}


SplashScreen::~SplashScreen()
{
}

void SplashScreen::LoadContent(){
	shape.setRadius(100.f);
	shape.setFillColor(sf::Color::Yellow);

	keys.push_back(sf::Keyboard::Return);
	keys.push_back(sf::Keyboard::Space);
}

void SplashScreen::UnloadContent(){
	GameScreen::UnloadContent();
}

void SplashScreen::Update(sf::Event event){
	input.Update(event);

	if (input.KeyPressed(keys)){
		ScreenManager::GetInstance().AddScreen(new TitleScreen);
	}
}

void SplashScreen::Draw(sf::RenderWindow &window){
	window.draw(shape);
}
