#include <SFML/Graphics.hpp>
#include "main.h"
#include "AnimatedSprite.h"
#include <list>
#include <Windows.h>

class Player
{
public:
	sf::Sprite player;
	sf::RectangleShape rect;
	float bottom, left, right, top;

	std::vector<std::vector<bool> > mask;

	Player(sf::Vector2f position, sf::Texture &texture)
	{
		sf::Image image;
		image = texture.copyToImage();

		m_health = 100;
		m_birdBullet = 2.0f;

		for (int i = 0; i < image.getSize().x; i++)
		{
			std::vector<bool> tempMask;

			for (int j = 0; j < image.getSize().y; j++)
			{
				if (image.getPixel(i, j).a > 0)
					tempMask.push_back(true);
				else
					tempMask.push_back(false);
			}
			mask.push_back(tempMask);
		}
		player.setTexture(texture);
		player.setPosition(position);
	}

	int getPlayerHealth(){
		return m_health;
	}

	void setPlayerHealth(int health){
		m_health = health;
	}

	int getBulletSpeed(){
		return m_birdBullet;
	}

	void setBulletSpeed(int bulletSpeed)
	{
		m_birdBullet = bulletSpeed;
	}

	void Update_Speed(){

	}

	void Update()
	{
		bottom = player.getPosition().y + player.getTextureRect().height;
		left = player.getPosition().x;
		right = player.getPosition().x + player.getTextureRect().width;
		top = player.getPosition().y;
	}

	bool Collision(Player p)
	{
		if (right <= p.left || left >= p.right || top >= p.bottom || bottom <= p.top)
		{
		}
		else
		{
			float colBottom, colTop, colLeft, colRight;
			colBottom = std::min(bottom, p.bottom);
			colTop = std::max(top, p.top);
			colLeft = std::max(left, p.left);
			colRight = std::min(right, p.right);

			for (int i = colTop; i < colBottom; i++)
			{
				for (int j = colLeft; j < colRight; j++)
				{
					if (mask[j - left][i - top] && p.mask[j - p.left][i - p.top])
						return true;
				}
			}
		}
		return false;
	}
private:
	int m_health;
	int m_birdBullet;
};

class Bird
{
public:
	sf::Sprite bird;
	sf::RectangleShape rect;
	float bottom, left, right, top;

	std::vector<std::vector<bool> > mask;

	Bird(sf::Texture &texture)
	{
		sf::Image image;
		image = texture.copyToImage();

		m_bulletSpeed = 2.0f;

		for (int i = 0; i < image.getSize().x; i++)
		{
			std::vector<bool> tempMask;

			for (int j = 0; j < image.getSize().y; j++)
			{
				if (image.getPixel(i, j).a > 0)
					tempMask.push_back(true);
				else
					tempMask.push_back(false);
			}
			mask.push_back(tempMask);
		}
		bird.setTexture(texture);
	}


	void Update()
	{
		bottom = bird.getPosition().y + bird.getTextureRect().height;
		left = bird.getPosition().x;
		right = bird.getPosition().x + bird.getTextureRect().width;
		top = bird.getPosition().y;
	}

	bool Collision(Player p)
	{
		if (right <= p.left || left >= p.right || top >= p.bottom || bottom <= p.top)
		{
		}
		else
		{
			float colBottom, colTop, colLeft, colRight;
			colBottom = std::min(bottom, p.bottom);
			colTop = std::max(top, p.top);
			colLeft = std::max(left, p.left);
			colRight = std::min(right, p.right);

			for (int i = colTop; i < colBottom; i++)
			{
				for (int j = colLeft; j < colRight; j++)
				{
					if (mask[j - left][i - top] && p.mask[j - p.left][i - p.top])
						return true;
				}
			}
		}
		return false;
	}
private:
	float m_bulletSpeed;
};

const int ENEMY_COUNT = 20;

int main(){
	HWND hwnd_win = GetForegroundWindow();
	ShowWindow(hwnd_win, SW_HIDE);

	enum Direction{ Down, Left, Right, Up };
	enum Bird { Fly1, Fly2, Fly3 };


	sf::Vector2i bird(32, Fly1);

	int playerHealth = 100;

	//Map1
	std::ifstream openfile("maps/Map1.txt");
	sf::Texture tileTexture;
	sf::Sprite tiles;

	//sf::Vector2i map[100][100];
	std::vector<std::vector<sf::Vector2i>> map;
	std::vector<sf::Vector2i> tempMap;
	//sf::Vector2i loadCounter = sf::Vector2i(0, 0);

	if (openfile.is_open()){
		std::string tileLocation;
		openfile >> tileLocation;
		tileTexture.loadFromFile(tileLocation);
		tiles.setTexture(tileTexture);

		while (!openfile.eof()){
			std::string str;
			openfile >> str;
			char x = str[0], y = str[0];
			if (!isdigit(x) || !isdigit(y)){
				//map[loadCounter.x][loadCounter.y] = sf::Vector2i(0, 0);
				tempMap.push_back(sf::Vector2i(-1,-1));
			}
			else{
				//map[loadCounter.x][loadCounter.y] = sf::Vector2i(x - '0', y - '0');
				tempMap.push_back(sf::Vector2i(x - '0', y - '0'));
			}

			if (openfile.peek() == '\n'){
				//loadCounter.x;
				//loadCounter.y++;
				map.push_back(tempMap);
				tempMap.clear();
			}
			/*else{
				loadCounter.x++;
			}

			loadCounter.y++;*/
		}
	}
	//End Map1

	//Font & Text
	sf::Font font;
	font.loadFromFile("font/arial.ttf");

	sf::Text energy("Energy: ", font);
	energy.setCharacterSize(20);
	energy.setColor(sf::Color::Red);

	sf::Text energyOutOf("/500", font);
	energyOutOf.setCharacterSize(20);
	energyOutOf.setColor(sf::Color::Red);

	//End Font & Text

	//Begin Jumping -----------------------
	const float gravity = 0.1; //originally 0.5
	int groundHeight = 610;
	int flyTime = 0;


	bool isGrounded = true;
	bool keyPressed = false;
	sf::Vector2f velocity(sf::Vector2f(0, 0));

	float moveSpeed = 2.0f, jumpSpeed = 1.0; //jumpspeed originally 15.0
	//End Jumping --------------------------

	sf::Vector2i source;//stores 2 values under variable name 
	source.x = 1; //for animation and cropping
	source.y = Down;

	float frameCounter = 0, switchFrame = 100, frameSpeed = 500; //frame rate stuff

	sf::RenderWindow window;
	sf::Vector2i screenDimensions(800, 700);
	window.create(sf::VideoMode(screenDimensions.x, screenDimensions.y, 32), "Avoid The Poop!"); //setup window
	window.setFramerateLimit(120);
	
	sf::Clock clock;

	sf::Clock enemyTime;

	sf::Clock rTime;
	sf::Time elapsed;
	//loadimages
	sf::Texture Tplayer;
	Tplayer.loadFromFile("images/player.png");

	Player p1(Player(sf::Vector2f(100, 100), Tplayer));
		//p2(Player(sf::Vector2f(100, 100), Tplayer));
	

	sf::Texture Tcloud;
	sf::Sprite Scloud;
	if (!Tcloud.loadFromFile("images/Clouds_2.png")){
		std::cout << "Could not load cloud image" << std::endl;
	}

	Scloud.setTexture(Tcloud);

	sf::Texture TBpoop;

	if (!TBpoop.loadFromFile("images/bpoop.png")){
		std::cout << "Could not load cloud image" << std::endl;
	}



	sf::Texture Tbird;
	Tbird.loadFromFile("images/birdwxh/bird1.png");

	Bird b1(Bird(Tbird));

	sf::Texture birdTexture; //load bird texture
	sf::Sprite birdImage; //bird image

	if (!birdTexture.loadFromFile("images/birdwxh/bird1.png")){
		std::cout << "Could not load bird image\n";
	}

	std::vector<sf::Sprite> cloud(100, sf::Sprite(Tcloud));
	std::vector<sf::FloatRect> cloudBounds(100);

	std::vector<sf::Sprite> enemyBullet(100, sf::Sprite(TBpoop));
	std::vector<sf::FloatRect> enemyBulletBounds(100);

	std::vector<sf::Sprite> enemy(100, sf::Sprite(Tbird));
	std::vector<sf::FloatRect> enemybounds(100);

	for (int i = 0; i < 10; i++){
		enemybounds[i] = enemy[i].getGlobalBounds();
	}

	for (int i = 0; i < 10; i++){
		cloudBounds[i] = cloud[i].getGlobalBounds();
	}

	for (int i = 0; i < 10; i++){
		enemyBulletBounds[i] = enemyBullet[i].getGlobalBounds();
	
	}

	float eTime = enemyTime.restart().asSeconds();

	//float rTime = roundTime.getElapsedTime().asSeconds();


	sf::CircleShape projectile(5);
	projectile.setScale(1.0f, 2.0f);
	projectile.setFillColor(sf::Color(255, 255, 204));

	for (int i = 0; i<5; i++)
	{
		float x = std::rand() % 915 + 1; // dist is the visible width of the screen
		float y = std::rand() % 300 + 1;
		enemy[i].setPosition(x, y); //spawn location of bird

		int bulletSpawn = std::rand() % 10 + 3;
		if (bulletSpawn % 10 == 0){
			enemyBullet[i].setPosition(enemy[i].getPosition().x, enemy[i].getPosition().y);
		}
	}


	for (int i = 0; i<20; i++)
	{
		float x = std::rand() % 10000 + 1; // dist is the visible width of the screen
		float y = std::rand() % 300 + 1;
		cloud[i].setPosition(x, y); //spawn location of cloud
	}

	float enemyspeed = 1.0f;
	float bulletSpeed_1 = 2.0f;
	float bulletSpeed_2 = 3.0f;
	float bulletSpeed_3 = 4.0f;
	float bulletSpeed_4 = 5.0f;
	float bulletSpeed_5 = 6.0f;
	float bulletSpeed_6 = 7.0f;
	float bulletSpeed_7 = 8.0f;
	float bulletSpeed_8 = 9.0f;
	float bulletSpeed_9 = 10.0f;
	float bulletSpeed_10 = 11.0f;

	//end load images


	//camera view
	//sf::View view;

	//view.reset(sf::FloatRect(0, 0, screenDimensions.x, screenDimensions.y));
	//view.setViewport(sf::FloatRect(0, 0, 1.0f, 1.0f));

	sf::Vector2f position(0, 0);
	//end camera view

	while (window.isOpen()){ //if game is open run through this loop
		sf::Event event;


		while (window.pollEvent(event)){

			//	std::cout << "TEST TIME" << std::endl;

			

			switch (event.type)
			{

			case sf::Event::Closed:
				window.close();
				break;
			}
		}
		elapsed = rTime.getElapsedTime();
		
		//std::cout << elapsed.asSeconds() << std::endl;

		/*if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)){
			//source.y = Up;
			//playerImage.move(0, -1);

		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)){
			//source.y = Down;
			//playerImage.move(0, 1);
		}*/
		/*
		if (elapsed.asSeconds() <= 10.0f){
			p1.setBulletSpeed(p1.getBulletSpeed() + 0.01f);
			std::cout << p1.getBulletSpeed() << std::endl;
		}

		if (elapsed.asSeconds() >= 10.0f && <=){
			p1.setBulletSpeed(p1.getBulletSpeed() == 5.0f);
			//std::cout << p1.getBulletSpeed() << std::endl;
			std::cout << "hi";
		}
		if (elapsed.asSeconds() == 3.f){
			p1.setBulletSpeed(p1.getBulletSpeed() + 1.0f);
			std::cout << p1.getBulletSpeed() << std::endl;
		}
		if (elapsed.asSeconds() == 4.f){
			p1.setBulletSpeed(p1.getBulletSpeed() + 1.0f);
			std::cout << p1.getBulletSpeed() << std::endl;
		}
		if (elapsed.asSeconds() == 5.f){
			p1.setBulletSpeed(p1.getBulletSpeed() + 1.0f);
			std::cout << p1.getBulletSpeed() << std::endl;
		}*/


		if (sf::Keyboard::isKeyPressed(sf::Keyboard::D) && p1.player.getPosition().x < 768){
			source.y = Right;
			//playerImage.move(moveSpeed, 0);
			p1.player.move(moveSpeed,0);
			//velocity.x = moveSpeed;
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::A) && p1.player.getPosition().x > 0){
			source.y = Left;
			//playerImage.move(-moveSpeed, 0);
			p1.player.move(-moveSpeed, 0);
			//velocity.x =- moveSpeed;
		}
		else{
			source.x = 1;
			//velocity.x = 0;
		}
		//if (isGrounded){
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space) && flyTime < 500 && p1.player.getPosition().y >= 0){
				flyTime++;
				velocity.y = -jumpSpeed;
				//keyPressed = true;
		}
		//}
		else if (!sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
		{
			if (flyTime > 0){
				flyTime--;
			}
			//keyPressed = false;
			
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::BackSpace)){
			playerHealth--;

		}

		for (int i = 0; i < 10; i++){
			//enemyBulletBounds[i] = enemyBullet[i].getGlobalBounds();
			if (enemyBullet[i].getGlobalBounds().intersects(p1.player.getGlobalBounds())){
				enemyBullet[i].setPosition(enemy[i].getPosition().x, enemy[i].getPosition().y);
				p1.setPlayerHealth(p1.getPlayerHealth() - 10);
				std::cout << "hit";
			}
		}


		if (p1.player.getPosition().y + p1.player.getScale().y < groundHeight || velocity.y < 0)
		{
			velocity.y += gravity;
		}
		else{
			p1.player.setPosition(p1.player.getPosition().x, groundHeight - p1.player.getScale().y);
			velocity.y = 0;
		}
		
		p1.player.move(velocity.x, velocity.y);

		frameCounter += frameSpeed * clock.restart().asSeconds();
		if (frameCounter >= switchFrame){
			frameCounter = 0;
			source.x++;
			if (source.x * 32 >= Tplayer.getSize().x){
				source.x = 0;
			}
		}

		/*for (int i = 0; i < loadCounter.x; i++){
			for (int j = 0; j < loadCounter.y; j++){
				if (map[i][j].x != -1 && map[i][j].y != -1){
					tiles.setPosition(i * 32, j * 32);
					tiles.setTextureRect(sf::IntRect(map[i][j].x * 32, map[i][j].y * 32, 32, 32));
					window.draw(tiles);
				}
			}
		}*/

		window.clear();
		for (int i = 0; i < map.size(); i++){
			for (int j = 0; j < map[i].size(); j++){
				if (map[i][j].x != -1 && map[i][j].y != -1){
					tiles.setPosition(i * 32, j * 32);
					tiles.setTextureRect(sf::IntRect(map[i][j].x * 32, map[i][j].y * 32, 32, 32));
					window.draw(tiles);
				}
			}
		}

		p1.player.setTextureRect(sf::IntRect(source.x * 32, source.y * 32, 32, 32));

		//window.draw(playerImage);//draw the player

		//std::cout << beginT << std::endl;

		//enemy
		if (p1.getPlayerHealth() > 0){
			for (int i = 0; i < 100; i++){
				window.draw(cloud[i]);
			}

			if (elapsed.asSeconds() <= 10.0f){
				for (int i = 0; i < 5; i++){
					enemy[i].move(-enemyspeed * eTime, 0.f);
					enemyBullet[i].move(0.1f, bulletSpeed_1 * eTime);
				}
			}
			if (elapsed.asSeconds() >= 10.0f && elapsed.asSeconds() <= 20.0f){
				for (int i = 0; i < 5; i++){
					enemy[i].move(-enemyspeed * eTime, 0.f);
					enemyBullet[i].move(0.1f, bulletSpeed_2 * eTime);

				}
			}
			if (elapsed.asSeconds() >= 20.0f && elapsed.asSeconds() <= 30.0f){
				for (int i = 0; i < 5; i++){
					enemy[i].move(-enemyspeed * eTime, 0.f);
					enemyBullet[i].move(0.1f, bulletSpeed_3 * eTime);

				}
			}
			if (elapsed.asSeconds() >= 30.0f && elapsed.asSeconds() <= 40.0f){
				for (int i = 0; i < 5; i++){
					enemy[i].move(-enemyspeed * eTime, 0.f);
					enemyBullet[i].move(0.1f, bulletSpeed_4 * eTime);

				}
			}
			if (elapsed.asSeconds() >= 40.0f && elapsed.asSeconds() <= 50.0f){
				for (int i = 0; i < 5; i++){
					enemy[i].move(-enemyspeed * eTime, 0.f);
					enemyBullet[i].move(0.1f, bulletSpeed_5 * eTime);

				}
			}
			if (elapsed.asSeconds() >= 50.0f && elapsed.asSeconds() <= 60.0f){
				for (int i = 0; i < 5; i++){
					enemy[i].move(-enemyspeed * eTime, 0.f);
					enemyBullet[i].move(0.1f, bulletSpeed_7 * eTime);

				}
			}
			if (elapsed.asSeconds() >= 60.0f && elapsed.asSeconds() <= 70.0f){
				for (int i = 0; i < 5; i++){
					enemy[i].move(-enemyspeed * eTime, 0.f);
					enemyBullet[i].move(0.1f, bulletSpeed_8 * eTime);

				}
			}
			if (elapsed.asSeconds() >= 70.0f && elapsed.asSeconds() <= 80.0f){
				for (int i = 0; i < 5; i++){
					enemy[i].move(-enemyspeed * eTime, 0.f);
					enemyBullet[i].move(0.1f, bulletSpeed_9 * eTime);

				}
			}
			if (elapsed.asSeconds() >= 80.0f){
				for (int i = 0; i < 5; i++){
					enemy[i].move(-enemyspeed * eTime, 0.f);
					enemyBullet[i].move(0.1f, bulletSpeed_10 * eTime);

				}
			}
			

			for (int i = 0; i < 5; i++){
				window.draw(enemy[i]);
				//bird.x++;
				//float x = std::rand() % 915 + 1; // dist is the visible width of the screen
				//bool bulletActive = false;


				//bulletActive = true;
				window.draw(enemyBullet[i]);

				if (enemyBullet[i].getPosition().y > 710){
					//bulletActive = false;
					enemyBullet[i].setPosition(enemy[i].getPosition().x, enemy[i].getPosition().y);
				}


				float y = std::rand() % 300 + 1;

				if (enemy[i].getPosition().x < -90){
					enemy[i].setPosition(850, y); //spawn location of bird
				}

				//std::cout << bulletActive << std::endl;
			}

		}

		//end enemy


		//start camera
		position.x = p1.player.getPosition().x + 10 - (screenDimensions.x / 2);
		position.y = p1.player.getPosition().y + 10 - (screenDimensions.y / 1);

		if (position.x < 0){
			position.x = 0;
		}
		if (position.y < 0){
			position.y = 0;
		}

	//	view.reset(sf::FloatRect(position.x, position.y, screenDimensions.x, screenDimensions.y));

	
		//window.setView(view);
		//end camera


		//int to text
		sf::Text energyDisplay("", font);
		energyDisplay.setCharacterSize(20);
		energyDisplay.setColor(sf::Color::Red);
		energyDisplay.setPosition(75, 0);
		std::stringstream ss;
		ss << flyTime;

		energyDisplay.setString(ss.str().c_str());

		energyOutOf.setPosition(110, 0);
		energy.setPosition(0, 0);

		sf::Text healthDisplay("", font);
		healthDisplay.setCharacterSize(20);
		healthDisplay.setColor(sf::Color::Red);
		healthDisplay.setPosition( 750, 0);
		std::stringstream ss1;
		ss1 << p1.getPlayerHealth();
		healthDisplay.setString(ss1.str().c_str());


		sf::Text roundTimer("", font);
		roundTimer.setCharacterSize(20);
		roundTimer.setColor(sf::Color::Red);
		roundTimer.setPosition(400, 0);
		std::stringstream ss55;
		ss55 << elapsed.asSeconds();
		roundTimer.setString(ss55.str().c_str());
		//end int to text

		//health
		if (p1.getPlayerHealth() <= 0){
			window.clear(sf::Color::Black);
			sf::Text gameOverTxt("Game Over", font);
			gameOverTxt.setCharacterSize(20);
			gameOverTxt.setColor(sf::Color::Red);
			gameOverTxt.setPosition(350, 350);
			window.draw(gameOverTxt);
		}

		
		//end health
		if (p1.getPlayerHealth() <= 0 && sf::Keyboard::isKeyPressed(sf::Keyboard::R)){
			p1.setPlayerHealth(p1.getPlayerHealth() + 100);
			rTime.restart().asSeconds();
			
		}



		if (p1.getPlayerHealth() > 0){
			window.draw(energy);//draw the energy
			window.draw(energyDisplay);
			window.draw(energyOutOf);
			window.draw(healthDisplay);
			window.draw(roundTimer);

			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)){
				p1.player.move(-0.1f, 0);
			}
			else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)){
				p1.player.move(0.1f, 0);
			}
			else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)){
				p1.player.move(0, -0.1f);
			}
			else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)){
				p1.player.move(0, 0.1f);
			}


			//window.draw(projectile);

			p1.Update();
			//		p2.Update();


			/*	if (p1.Collision(p2)){
					std::cout << "Collision!" << std::endl;
					}*/

			window.draw(p1.player);
			//window.draw(p2.player);


		}


		window.display();
		window.clear();
	}

}
