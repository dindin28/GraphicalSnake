#include <SFML/Graphics.hpp>
#include <string>
#include <windows.h>
#include <fstream>
#include <vector>
#include <algorithm>

#ifndef NDEBUG
	#include <iostream>
#endif

struct Records {
	std::string nickname;
	int score = 0;
	friend bool operator>(Records &a, Records &b) {
		if (a.score > b.score) {
			return true;
		}
		else {
			return false;
		}
	}
	friend bool operator<(Records& a, Records& b) {
		if (a.score < b.score) {
			return true;
		}
		else {
			return false;
		}
	}
	Records& operator= (const Records& a)
	{
		nickname = a.nickname;
		score = a.score;
		return *this;
	}
};

namespace snake {
	struct SnakeNode {
		int x, y;
		SnakeNode* next = NULL;
		SnakeNode* prev = NULL;
	};
	enum Tiles {
		HEADMOVINGUP,
		HEADMOVINGRIGHT,
		HEADMOVINGDOWN,
		HEADMOVINGLEFT,
		BODY,
		FOOD,
		TILE1,
		TILE2
	};
	sf::IntRect setTile(Tiles tile) {
		switch (tile) {
			case(HEADMOVINGUP): {
				return (sf::IntRect(0, 0, 20, 20));
			}
			case(HEADMOVINGRIGHT): {
				return (sf::IntRect(20, 0, 20, 20));
			}
			case(HEADMOVINGDOWN): {
				return (sf::IntRect(40, 0, 20, 20));
			}
			case(HEADMOVINGLEFT): {
				return (sf::IntRect(60, 0, 20, 20));
			}
			case(BODY): {
				return (sf::IntRect(80, 0, 20, 20));
			}
			case(FOOD): {
				return (sf::IntRect(100, 0, 20, 20));
			}
			case(TILE1): {
				return (sf::IntRect(120, 0, 20, 20));
			}
			case(TILE2): {
				return (sf::IntRect(140, 0, 20, 20));
			}
		}
	}
	struct Point{
		int x, y;
	};

	struct Node {
		int x, y;
		Node* next = NULL;
		Node* prev = NULL;
	};


	class Snake {
	private:
		//GF
		int sizeGF = 9;
		int borderThickness = 5;
		char* headOfGF;
		sf::RenderWindow *window;
		sf::Vector2u sizeOfRender;
		sf::Texture tileTexture;
		sf::Sprite tileSprite;
		sf::Vector2f tileSize;
		bool finished = false;
		//snake
		Node* headOfSnake;
		int sizeOfSnake = 3;
		//movement
		/*
	   * Up-arrow => return 1
	   * Right-arrow => return 2
	   * Down-arrow => return 3
	   * Left-arrow => return 4
	   * else return 0
	   */
		int movement = 1;
		int prevMovement = 1;
	public:
		Snake(sf::RenderWindow& window, sf::Vector2u sizeOfRender) {
			//GF
			this->sizeOfRender = sizeOfRender;
			this->window = &window;
			headOfGF = new char[sizeGF * sizeGF];
			clearGF();
#ifndef NDEBUG
			tileTexture.loadFromFile("Debug\\content\\tiles.png");
#else
			tileTexture.loadFromFile("content\\tiles.png");
#endif
			tileSize.x = static_cast<float>(sizeOfRender.x - 2 * borderThickness) / sizeGF;
			tileSize.y = static_cast<float>(sizeOfRender.y - 2 * borderThickness) / sizeGF;
			tileSprite.setTexture(tileTexture);
			tileSprite.setScale(tileSize.x  / tileTexture.getSize().y, tileSize.y / tileTexture.getSize().y);
			//Snake
			//creating snake
			headOfSnake = new Node;
			Node* temp;
			for (int i = 0; i < sizeOfSnake - 1; i++) {
				temp = new Node;
				temp->next = headOfSnake;
				headOfSnake->prev = temp;
				headOfSnake = temp;
			}
			temp = headOfSnake;
			for (int i = 0; i < sizeOfSnake; i++) {
				temp->x = sizeGF / 2;
				temp->y = sizeGF / 2 + i;
				*(headOfGF + temp->y * sizeGF + temp->x) = 'o';
				temp = temp->next;
			}
			*(headOfGF + headOfSnake->y * sizeGF + headOfSnake->x) = 'O';
			//end of creating snake
			spawnFood();
		}
		void showGF() {
			int i = 0; //for even rows
			for (float positionY = borderThickness; positionY < borderThickness + tileSize.y * sizeGF - 1; positionY += tileSize.y, i++) {
				if (i % 2 == 0) {
					tileSprite.setTextureRect(setTile(TILE1));
					for (float positionX = borderThickness; positionX < borderThickness + tileSize.x * sizeGF - 1; positionX += tileSize.x) {
						if (tileSprite.getTextureRect() == setTile(TILE1)) {
							tileSprite.setTextureRect(setTile(TILE2));
						}
						else {
							tileSprite.setTextureRect(setTile(TILE1));
						}
						tileSprite.setPosition(positionX, positionY);
						window->draw(tileSprite);
					}
				}
				else {
					tileSprite.setTextureRect(setTile(TILE2));
					for (float positionX = borderThickness; positionX < borderThickness + tileSize.x * sizeGF - 1; positionX += tileSize.x) {
						if (tileSprite.getTextureRect() == setTile(TILE1)) {
							tileSprite.setTextureRect(setTile(TILE2));
						}
						else {
							tileSprite.setTextureRect(setTile(TILE1));
						}
						tileSprite.setPosition(positionX, positionY);
						window->draw(tileSprite);
					}
				}
			}

			for (int i = 0; i < sizeGF; i++) {
				for (int j = 0; j < sizeGF; j++) {
					if (*(headOfGF + sizeGF * i + j) == 'X' || *(headOfGF + sizeGF * i + j) == 'O' || *(headOfGF + sizeGF * i + j) == 'o') {
						if (*(headOfGF + sizeGF * i + j) == 'X') {
							tileSprite.setTextureRect(setTile(FOOD));
						}
						else if (*(headOfGF + sizeGF * i + j) == 'O') {
							if (prevMovement == 1) {
								tileSprite.setTextureRect(setTile(HEADMOVINGUP));
							}
							else if(prevMovement == 2){
								tileSprite.setTextureRect(setTile(HEADMOVINGRIGHT));
							}
							else if (prevMovement == 3) {
								tileSprite.setTextureRect(setTile(HEADMOVINGDOWN));
							}
							else if (prevMovement == 4) {
								tileSprite.setTextureRect(setTile(HEADMOVINGLEFT));
							}
						}
						else if (*(headOfGF + sizeGF * i + j) == 'o') {
							tileSprite.setTextureRect(setTile(BODY));
						}
						tileSprite.setPosition(borderThickness + tileSize.x * j, borderThickness + tileSize.y * i);
						window->draw(tileSprite);
					}
				}
			}
		}

		void clearGF() {
			for (char* iter = headOfGF; iter != (headOfGF + sizeGF * sizeGF); iter++) {
				*(iter) = ' ';
			}
		}

		void spawnFood() {
			int counter = 0;
			Point* ptr = new Point[sizeGF * sizeGF - getSizeOfSnake()];
			for (int i = 0; i < sizeGF; i++) {
				for (int j = 0; j < sizeGF; j++) {
					if (*(headOfGF + i * sizeGF + j) != 'o' && *(headOfGF + i * sizeGF + j) != 'O') {
						ptr[counter].x = j; ptr[counter].y = i;
						counter++;
					}
				}
			}
			int randBuff = rand() % (sizeGF * sizeGF - getSizeOfSnake());
			*(headOfGF + ptr[randBuff].y * sizeGF + ptr[randBuff].x) = 'X';
		}

		int getSizeOfSnake() {
			Node* temp = headOfSnake;
			int counter = 0;
			while (temp) {
				temp = temp->next;
				counter++;
			}
			return counter;
		}

		void moveSnake() {
			int x = headOfSnake->x, y = headOfSnake->y;
			switch (movement) {
			case(1): {
				y--;
				break;
			}
			case(2): {
				x++;
				break;
			}
			case(3): {
				y++;
				break;
			}
			case(4): {
				x--;
				break;
			}
			}
			prevMovement = movement;
			switch (checkBorders(x, y)) {
				case(0): {
					finished = true;
					break;
				}
				case(1): {
					*(headOfGF + headOfSnake->y * sizeGF + headOfSnake->x) = 'o';
					Node* temp = headOfSnake;
					while (temp->next)
						temp = temp->next;
					temp->prev->next = NULL;
					*(headOfGF + temp->y * sizeGF + temp->x) = ' ';
					temp->x = x; temp->y = y;
					*(headOfGF + temp->y * sizeGF + temp->x) = 'O';
					headOfSnake->prev = temp;
					temp->next = headOfSnake; temp->prev = NULL;
					headOfSnake = temp;
					break;
				}
				case(2): {
					*(headOfGF + headOfSnake->y * sizeGF + headOfSnake->x) = 'o';
					Node* temp = new Node;
					temp->next = headOfSnake;
					headOfSnake->prev = temp;
					temp->x = x; temp->y = y;
					*(headOfGF + temp->y * sizeGF + temp->x) = 'O';
					spawnFood();
					headOfSnake = temp;
					break;
				}
			}
		}

		int checkBorders(int x, int y) {
			if (x >= sizeGF || x < 0 || y >= sizeGF || y < 0 || *(headOfGF + sizeGF * y + x) == 'o') {
				return 0;
			}
			else if (*(headOfGF + sizeGF * y + x) == 'X') {
				return 2;
			}
			else return 1;
		}

		//movement
		void changeMovement(int i) {
			if (i == 1 && prevMovement != 3 || i == 2 && prevMovement != 4 || i == 3 && prevMovement != 1 || i == 4 && prevMovement != 2) {
				movement = i;
			}
		}

		bool getFinished() {
			return finished;
		}
	};
}

enum Menu {
	mainMenu,
	choosingDifficult,
	records,
	game,
	preGame,
	postGame
};


int main(){
	srand(time(NULL));
	sf::Texture texture_background;
#ifndef NDEBUG
	texture_background.loadFromFile("Debug//content//main_menu_background.jpg");
#else
	texture_background.loadFromFile("content//main_menu_background.jpg");
#endif // DEBUG
	sf::RenderWindow window(sf::VideoMode(texture_background.getSize().x, texture_background.getSize().y), "Snake");
	window.setSize(sf::Vector2u(GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN)));
	sf::Sprite sprite_background;
	sprite_background.setTexture(texture_background);
	sf::Event event;
	Menu menu = Menu::mainMenu;
	//icon
	sf::Image icon;
#ifndef NDEBUG
	icon.loadFromFile("Debug\\content\\icon.png");
#else
	icon.loadFromFile("content\\icon.png");
#endif
	window.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());

	sf::Font font;
#ifndef NDEBUG
	font.loadFromFile("Debug\\content\\Imperial.otf");
#else
	font.loadFromFile("content\\Imperial.otf");
#endif

	sf::Text text;
	text.setFillColor(sf::Color::White);
	text.setOutlineColor(sf::Color::Black);
	text.setOutlineThickness(2);
	text.setFont(font);
	sf::Clock eventTimer;
	eventTimer.restart();

	int difficult = 1;
	std::string nickname = "default";

	while (window.isOpen()) {
		if (menu == Menu::mainMenu && window.isOpen()) {
			eventTimer.restart();
			int main_menu_position = 1;
			/*
			1 - Start
			2 - Records
			3 - Exit
			*/
			while (menu == Menu::mainMenu && window.isOpen()) {
				window.clear();
				while (window.pollEvent(event)) {
					if (event.type == sf::Event::Closed) {
						window.close();
					}
					if (eventTimer.getElapsedTime().asMilliseconds() > 200) {
						if (event.key.code == sf::Keyboard::Down) {
							main_menu_position++;
								if (main_menu_position > 4) main_menu_position = 1;
								eventTimer.restart();
						}
						if (event.key.code == sf::Keyboard::Up) {
							main_menu_position--;
							if (main_menu_position < 1) main_menu_position = 4;
							eventTimer.restart();
						}
						if (event.key.code == sf::Keyboard::Enter) {
							switch (main_menu_position){
								case(1): {
									menu = Menu::choosingDifficult;
									break;
								}
								case(2): {
									menu = Menu::records;
									break;
								}
								case(3): {
									window.close();
									break;
								}
								case(4): {
									eventTimer.restart();
									bool flag = true;
									int blinkCounter = 0;
									while (flag && window.isOpen()) {
										window.clear();
										while (window.pollEvent(event)) {
											if (event.type == sf::Event::Closed) {
												window.close();
											}
											if (eventTimer.getElapsedTime().asMilliseconds() > 150) {
												if (event.key.code == sf::Keyboard::Escape || event.key.code == sf::Keyboard::Enter) {
													flag = false;
													eventTimer.restart();
													break;
												}
												if (event.key.code == sf::Keyboard::BackSpace && nickname.length()!= 0) {
													nickname.pop_back();
												}
												if (event.key.code >= 0 && event.key.code <= 25 && nickname.length() < 10) {
													nickname += static_cast<char>(event.key.code + 97);
												}
												eventTimer.restart();
											}
										}
										window.draw(sprite_background);
										text.setString("WELCOME TO THE JUNGLE");
										text.setCharacterSize(50);
										text.setOrigin(text.getLocalBounds().left + text.getLocalBounds().width / 2, 0);
										text.setPosition(texture_background.getSize().x / 2, 10);
										window.draw(text);

										text.setCharacterSize(40);
										text.setString("START");
										text.setOrigin(text.getLocalBounds().left + text.getLocalBounds().width / 2, 0);
										text.setPosition(texture_background.getSize().x * 0.5, texture_background.getSize().y * 0.5);
										window.draw(text);

										text.setString("RECORDS");
										text.setOrigin(text.getLocalBounds().left + text.getLocalBounds().width / 2, 0);
										text.setPosition(texture_background.getSize().x * 0.5, texture_background.getSize().y * 0.6);
										window.draw(text);

										text.setString("EXIT");
										text.setOrigin(text.getLocalBounds().left + text.getLocalBounds().width / 2, 0);
										text.setPosition(texture_background.getSize().x * 0.5, texture_background.getSize().y * 0.7);
										window.draw(text);

										if (blinkCounter < 250) {
											text.setString("Your nick: " + nickname + " >");
										}
										else {
											text.setString("Your nick: " + nickname);
										}
										text.setOrigin(0, 0);
										text.setPosition(10, texture_background.getSize().y * 0.9);
										window.draw(text);

										text.setCharacterSize(20);
										text.setString("Type your nickname\nLimit for nickname is 10 characters\nUse Enter or Escape to quit typing");
										text.setOrigin(text.getLocalBounds().left * 2 + text.getLocalBounds().width, text.getLocalBounds().top * 2 + text.getLocalBounds().height);
										text.setPosition(texture_background.getSize().x - 10, texture_background.getSize().y * 0.98);
										window.draw(text);
										text.setFont(font);

										blinkCounter++;
										if (blinkCounter > 500) blinkCounter = 0;

										window.display();
									}
									break;
								}
							}
						}
					}
				}
				window.draw(sprite_background);
				text.setString("WELCOME TO THE JUNGLE");
				text.setCharacterSize(50);
				text.setOrigin(text.getLocalBounds().left + text.getLocalBounds().width / 2, 0);
				text.setPosition(texture_background.getSize().x / 2, 10);
				window.draw(text);

				text.setCharacterSize(40);
				if (main_menu_position == 1) {
					text.setString("< START >");
				}
				else {
					text.setString("START");
				}
				text.setOrigin(text.getLocalBounds().left + text.getLocalBounds().width / 2, 0);
				text.setPosition(texture_background.getSize().x * 0.5, texture_background.getSize().y * 0.5);
				window.draw(text);
				if (main_menu_position == 2) {
					text.setString("< RECORDS >");
				}
				else {
					text.setString("RECORDS");
				}
				text.setOrigin(text.getLocalBounds().left + text.getLocalBounds().width / 2, 0);
				text.setPosition(texture_background.getSize().x * 0.5, texture_background.getSize().y * 0.6);
				window.draw(text);
				if (main_menu_position == 3) {
					text.setString("< EXIT >");
				}
				else {
					text.setString("EXIT");
				}
				text.setOrigin(text.getLocalBounds().left + text.getLocalBounds().width / 2, 0);
				text.setPosition(texture_background.getSize().x * 0.5, texture_background.getSize().y * 0.7);
				window.draw(text);

				if (main_menu_position == 4) {
					text.setString("Your nick: " + nickname + " >");
				}
				else {
					text.setString("Your nick: " + nickname);
				}
				text.setOrigin(0, 0);
				text.setPosition(10, texture_background.getSize().y * 0.9);
				window.draw(text);

				text.setCharacterSize(20);
				text.setString("Use arrows to move cursor\nUse Enter to choose option");
				text.setOrigin(text.getLocalBounds().left * 2 + text.getLocalBounds().width, text.getLocalBounds().top * 2 + text.getLocalBounds().height);
				text.setPosition(texture_background.getSize().x - 10, texture_background.getSize().y * 0.98);
				window.draw(text);
				text.setFont(font);

				window.display();
			}
		}
		else if (menu == Menu::records && window.isOpen()) {
#ifndef NDEBUG
		std::ifstream fin("Debug\\content\\records.txt");
#else
		std::ifstream fin("content\\records.txt");
#endif
		std::vector<Records> vector;
		std::string string;
		fin >> string;
		int counter = 0;
		while (string != "0") {
			vector.resize(counter + 1);
			vector[counter].nickname = string;
			fin >> string;
			vector[counter].score = atoi(&string[0]);
			counter++;
			fin >> string;
		}
			while (menu == Menu::records && window.isOpen()) {
				window.clear();
				while (window.pollEvent(event)) {
					if (event.type == sf::Event::Closed) {
						window.close();
					}
					if (event.key.code == sf::Keyboard::Escape) {
						menu = Menu::mainMenu;
					}
				}
				window.draw(sprite_background);
				text.setString("LEADERBOARD");
				text.setCharacterSize(50);
				text.setOrigin(text.getLocalBounds().left + text.getLocalBounds().width / 2, 0);
				text.setPosition(texture_background.getSize().x / 2, 10);
				window.draw(text);

				text.setCharacterSize(35);
				for (int i = 0; i < counter; i++) {
					text.setString(std::to_string(i + 1) + ") " + vector[i].nickname + ": " + std::to_string(vector[i].score));
					text.setOrigin(0, 0);
					text.setPosition(texture_background.getSize().x * 0.37, texture_background.getSize().y * (i + 1) / 10);
					switch (i){
						case(0): {
							text.setFillColor(sf::Color(255, 215, 0, 255));
							break;
						}
						case(1): {
							text.setFillColor(sf::Color(192, 192, 192, 255));
							break;
						}
						case(2): {
							text.setFillColor(sf::Color(205, 127, 50, 255));
							break;
						}
						default: {
							text.setFillColor(sf::Color::White);
							break;
						}
					}
					window.draw(text);
				}
				text.setFillColor(sf::Color::White);
				text.setCharacterSize(20);
				text.setString("Escape to quit");
				text.setOrigin(text.getLocalBounds().left * 2 + text.getLocalBounds().width, text.getLocalBounds().top * 2 + text.getLocalBounds().height);
				text.setPosition(texture_background.getSize().x - 10, texture_background.getSize().y * 0.98);
				window.draw(text);
				text.setFont(font);

				window.display();
			}
		}
		else if (menu == Menu::choosingDifficult && window.isOpen()) {
			eventTimer.restart();
			while (menu == Menu::choosingDifficult && window.isOpen()) {
				window.clear();
				while (window.pollEvent(event)) {
					if (event.type == sf::Event::Closed) {
						window.close();
					}
					if (eventTimer.getElapsedTime().asMilliseconds() > 200) {
						if (event.key.code == sf::Keyboard::Escape) {
							menu = Menu::mainMenu;
						}
						if (event.key.code == sf::Keyboard::Down) {
							difficult++;
							if (difficult > 3) difficult = 1;
							eventTimer.restart();
						}
						if (event.key.code == sf::Keyboard::Up) {
							difficult--;
							if (difficult < 1) difficult = 3;
							eventTimer.restart();
						}
						if (event.key.code == sf::Keyboard::Enter) {
							menu = Menu::game;
						}
					}
				}

				window.draw(sprite_background);
				text.setString("CHOOSE DIFFICULT");
				text.setCharacterSize(50);
				text.setOrigin(text.getLocalBounds().left + text.getLocalBounds().width / 2, 0);
				text.setPosition(texture_background.getSize().x / 2, 10);
				window.draw(text);

				text.setCharacterSize(40);
				if (difficult == 1) {
					text.setString("< EASY >");
				}
				else {
					text.setString("EASY");
				}
				text.setOrigin(text.getLocalBounds().left + text.getLocalBounds().width / 2, 0);
				text.setPosition(texture_background.getSize().x * 0.5, texture_background.getSize().y * 0.5);
				window.draw(text);
				if (difficult == 2) {
					text.setString("< MEDIUM >");
				}
				else {
					text.setString("MEDIUM");
				}
				text.setOrigin(text.getLocalBounds().left + text.getLocalBounds().width / 2, 0);
				text.setPosition(texture_background.getSize().x * 0.5, texture_background.getSize().y * 0.6);
				window.draw(text);
				if (difficult == 3) {
					text.setString("< HARD >");
				}
				else {
					text.setString("HARD");
				}
				text.setOrigin(text.getLocalBounds().left + text.getLocalBounds().width / 2, 0);
				text.setPosition(texture_background.getSize().x * 0.5, texture_background.getSize().y * 0.7);
				window.draw(text);

				text.setCharacterSize(20);
				text.setString("Use arrows to move cursor\nUse Enter to choose option\nEscape to quit");
				text.setOrigin(text.getLocalBounds().left * 2 + text.getLocalBounds().width, text.getLocalBounds().top * 2 + text.getLocalBounds().height);
				text.setPosition(texture_background.getSize().x - 10, texture_background.getSize().y * 0.98);
				window.draw(text);

				window.display();
			}
		}
		else 
		if (menu == Menu::game && window.isOpen()) {
			snake::Snake GF(window, texture_background.getSize());
			text.setString("To move snake use arrows\nPress ENTER to countinue");
			text.setOrigin(text.getLocalBounds().left + text.getLocalBounds().width / 2, 0);
			text.setPosition(texture_background.getSize().x * 0.5, texture_background.getSize().y * 0.5);
			eventTimer.restart();
			menu = Menu::preGame;
			while (window.isOpen() && menu == Menu::preGame) {
				while (window.pollEvent(event)) {
					if (event.type == sf::Event::Closed) {
						window.close();
					}
					if (eventTimer.getElapsedTime().asMilliseconds() > 500) {
						if (event.key.code == sf::Keyboard::Enter) {
							menu = Menu::game;
						}
					}
				}
				window.clear(sf::Color::Black);
				GF.showGF();
				window.draw(text);
				window.display();
			}
			eventTimer.restart();
			/*
				Easy - 9 * 9
				Medium - 10 * 10
				Hard - 11 * 11
			*/
			while(window.isOpen() && !GF.getFinished()){
				while (window.pollEvent(event)) {
					if (event.type == sf::Event::Closed) {
						window.close();
					}
					if (event.key.code == sf::Keyboard::Escape) {
						menu = Menu::mainMenu;
					}
					if (event.key.code == sf::Keyboard::Up) {
						GF.changeMovement(1);
					}
					if (event.key.code == sf::Keyboard::Right) {
						GF.changeMovement(2);
					}
					if (event.key.code == sf::Keyboard::Down) {
						GF.changeMovement(3);
					}
					if (event.key.code == sf::Keyboard::Left) {
						GF.changeMovement(4);
					}
				}
				if (eventTimer.getElapsedTime().asMilliseconds() * difficult > 500) {
					GF.moveSnake();
					eventTimer.restart();
				}
				window.clear();
				GF.showGF();
				window.display();
			}
#ifndef NDEBUG
			std::ifstream fin("Debug\\content\\records.txt");
#else
			std::ifstream fin("content\\records.txt");
#endif
			std::vector<Records> vector;
			std::string string;
			fin >> string;
			int counter = 0;
			while (string != "0") {
				vector.resize(counter + 1);
				vector[counter].nickname = string;
				fin >> string;
				vector[counter].score = atoi(&string[0]);
				counter++;
				fin >> string;
			}
			vector.resize(counter + 1);
			vector[counter].nickname = nickname;
			vector[counter].score = GF.getSizeOfSnake() * difficult;
			counter++;
			for (int i = 0; i < counter - 1; i++)
				for (int j = 0; j < counter - i - 1; j++)
					if (vector[j] < vector[j + 1]) {
						Records buff = vector[j];
						vector[j] = vector[j + 1];
						vector[j + 1] = buff;
					}
#ifndef NDEBUG
			std::ofstream fout("Debug\\content\\records.txt");
#else
			std::ofstream fout("content\\records.txt");
#endif
			if (vector.size() > 9) {
				for (int i = 0; i < 9; i++) {
					fout << vector[i].nickname << " " << vector[i].score << std::endl;
				}
			}else{
				for (int i = 0; i < vector.size(); i++) {
					fout << vector[i].nickname << " " << vector[i].score << std::endl;
				}
			}
			
			fout << "0";
			text.setString("Your score is " + std::to_string(GF.getSizeOfSnake() * difficult) + "\nPress ENTER to continue");
			text.setOrigin(text.getLocalBounds().left + text.getLocalBounds().width / 2, 0);
			text.setPosition(texture_background.getSize().x * 0.5, texture_background.getSize().y * 0.5);
			eventTimer.restart();
			menu = Menu::postGame;
			while (window.isOpen() && menu == Menu::postGame) {
				while (window.pollEvent(event)) {
					if (event.type == sf::Event::Closed) {
						window.close();
					}
					if (eventTimer.getElapsedTime().asMilliseconds() > 500) {
						if (event.key.code == sf::Keyboard::Enter) {
							menu = Menu::mainMenu;
						}
					}
				}
				window.clear(sf::Color::Black);
				GF.showGF();
				window.draw(text);
				window.display();
			}
		}
	}
	return 0;
}