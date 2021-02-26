#include <SFML/Graphics.hpp>
#include <string>
#include <windows.h>
#include <fstream>
#include <vector>
#include <cassert>

#ifndef NDEBUG
	#include <iostream>
#endif

struct Records {
	std::string nickname;
	int score = 0;
};

namespace snake {
	struct SnakeNode {
		int x, y;
		SnakeNode* next = NULL;
		SnakeNode* prev = NULL;
	};
	enum Tiles {
		HEAD,
		BODY,
		FOOD,
		TILE1,
		TILE2
	};
	sf::IntRect setTile(Tiles tile) {
		switch (tile) {
			case(SNAKE_HEAD): {
				return (sf::IntRect(0, 0, 20, 20));
			}
			case(SNAKE_BODY): {
				return (sf::IntRect(20, 0, 20, 20));
			}
			case(FOOD): {
				return (sf::IntRect(40, 0, 20, 20));
			}
			case(TILE1): {
				return (sf::IntRect(60, 0, 20, 20));
			}
			case(TILE2): {
				return (sf::IntRect(80, 0, 20, 20));
			}
		}
	}
	struct Point{
		int x, y;
	};

	class GamingField {
	private:
		int sizeGF;
		int borderThickness = 5;
		char* head;
		sf::RenderWindow *window;
		sf::Vector2u sizeOfRender;
		sf::Texture tileTexture;
		sf::Sprite tileSprite;
		sf::Vector2f tileSize;
	public:
		GamingField(sf::RenderWindow& window, int sizeGF, sf::Vector2u sizeOfRender) {
			this->sizeOfRender = sizeOfRender;
			this->window = &window;
			this->sizeGF = sizeGF;
			head = new char[sizeGF * sizeGF];
			clearGF();
#ifndef NDEBUG
			tileTexture.loadFromFile("Debug\\content\\tiles.png");
#else
			tileTexture.loadFromFile("content\\tiles.png");
#endif
			tileSize.x = static_cast<float>(sizeOfRender.x - 2 * borderThickness - sizeGF - 1) / sizeGF;
			tileSize.y = static_cast<float>(sizeOfRender.y - 2 * borderThickness - sizeGF - 1) / sizeGF;
			tileSprite.setTexture(tileTexture);
			tileSprite.setScale(tileSize.x  / tileTexture.getSize().y, tileSize.y / tileTexture.getSize().y);
		}
		void showGF() {
			window->clear(sf::Color::White);
			sf::RectangleShape border(sf::Vector2f(borderThickness, sizeOfRender.y));
			border.setFillColor(sf::Color::Black);
			window->draw(border);
			border.setPosition(sizeOfRender.x - borderThickness, 0);
			window->draw(border);
			border.setSize(sf::Vector2f(sizeOfRender.x, borderThickness));
			border.setPosition(0, 0);
			window->draw(border);
			border.setPosition(0, sizeOfRender.y - borderThickness);
			window->draw(border);

			border.setSize(sf::Vector2f(sizeOfRender.x, 1));
			for (float position = borderThickness + tileSize.y; position <= borderThickness + (tileSize.y + 1) * (sizeGF - 1) - 1; position += tileSize.y + 1) {
				border.setPosition(0, position);
				window->draw(border);
			}

			border.setSize(sf::Vector2f(1, sizeOfRender.y));
			for (float position = borderThickness + tileSize.x; position <= borderThickness + (tileSize.x + 1) * (sizeGF - 1) - 1; position += tileSize.x + 1) {
				border.setPosition(position, 0);
				window->draw(border);
			}
			int i = 0; //for even rows
			for (float positionY = borderThickness; positionY < borderThickness + (tileSize.y + 1) * sizeGF - 1; positionY += tileSize.y + 1, i++) {
				if (i % 2 == 0) {
					tileSprite.setTextureRect(setTile(TILE1));
					for (float positionX = borderThickness; positionX < borderThickness + (tileSize.x + 1) * sizeGF - 1; positionX += tileSize.x + 1) {
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
					for (float positionX = borderThickness; positionX < borderThickness + (tileSize.x + 1) * sizeGF - 1; positionX += tileSize.x + 1) {
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

			/*tileSprite.setTextureRect(setTile(HEAD));
			window->draw(tileSprite);*/

			window->display();
		}
		void clearGF() {
			for (char* iter = head; iter != (head + sizeGF * sizeGF); iter++) {
				*(iter) = ' ';
			}
		}
	};
}

enum Menu {
	mainMenu,
	choosingDifficult,
	records,
	game
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
											text.setString("Your nick " + nickname + " >");
										}
										else {
											text.setString("Your nick " + nickname);
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
					text.setString("Your nick " + nickname + " >");
				}
				else {
					text.setString("Your nick " + nickname);
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
+			counter++;
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
		else if (menu == Menu::game && window.isOpen()) {
			snake::GamingField GF(window, difficult + 8, texture_background.getSize());
			/*
				Easy - 9 * 9
				Medium - 10 * 10
				Hard - 11 * 11
			*/
			//headOfSnake
			//GF.clearGF();
			//GF.spawnFood();
			int movement = 1;
			while(window.isOpen()){
				while (window.pollEvent(event)) {
					if (event.type == sf::Event::Closed) {
						window.close();
					}
				}
				GF.showGF();
			}
		}
	}
	return 0;
}