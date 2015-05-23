#include "TitleScreen.h"


TitleScreen::TitleScreen()
{
}


TitleScreen::~TitleScreen()
{
}

void TitleScreen::LoadContent(){
	shape.setRadius(100.f);
	shape.setFillColor(sf::Color::Green);

	keys.push_back(sf::Keyboard::Return);
	keys.push_back(sf::Keyboard::Space);
}

void TitleScreen::UnloadContent(){
	GameScreen::UnloadContent();
}

void TitleScreen::Update(sf::Event event){
	input.Update(event);

	if (input.KeyPressed(keys)){
		ScreenManager::GetInstance().AddScreen(new SplashScreen);
	}
}

void TitleScreen::Draw(sf::RenderWindow &window){
	window.draw(shape);
}
