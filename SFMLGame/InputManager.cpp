#include "InputManager.h"


InputManager::InputManager()
{
}


InputManager::~InputManager()
{
}

void InputManager::Update(sf::Event event){
	this->event = event;
}

bool InputManager::KeyPressed(int key){
	if (event.key.code == key){
		return true;
	}
	else{
		return false;
	}
}

bool InputManager::KeyPressed(std::vector<int> keys){
	for (int i = 0; i < keys.size(); i++)
	{
		if (event.key.code == keys[i]){
			return true;
		}
		else{
			return false;
		}
	}
}

bool InputManager::KeyReleased(int key){
	if (event.key.code == key && sf::Event::KeyReleased)
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool InputManager::KeyReleased(std::vector<int> keys){
	for (int i = 0; i < keys.size(); i++){
		if (event.key.code == keys[i] && sf::Event::KeyReleased)
		{
			return true;
		}
		else{
			return false;
		}
	}
}
/*
bool InputManager::KeyDown(sf::RenderWindow &window, int key){
	if (sf::Keyboard::isKeyPressed){
		return true;
	}
	else{
		return false;
	}
}

bool InputManager::KeyDown(std::vector<int> keys){

}
*/