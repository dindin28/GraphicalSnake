#include <SFML/Graphics.hpp>
#include <string>
#include <windows.h>
#include <fstream>
#include <vector>

#define DEBUG
#ifdef DEBUG
	#include <iostream>
#endif

struct Records {
	std::string nickname;
	int score;
};

enum Menu {
	mainMenu,
	choosingDifficult,
	records,
	game
};

int main(){
	sf::Texture texture_background;
#ifdef DEBUG
	texture_background.loadFromFile("Debug//content//main_menu_background.jpg");
#else
	texture_background.loadFromFile("content//main_menu_background.jpg");
#endif // DEBUG
	sf::RenderWindow window(sf::VideoMode(texture_background.getSize().x, texture_background.getSize().y), "Snake");
	sf::Sprite sprite_background;
	sprite_background.setTexture(texture_background);
	window.setSize(sf::Vector2u(GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN)));
	sf::Event event;
	Menu menu = Menu::mainMenu;
	
	//icon
	sf::Image icon;
#ifdef DEBUG
	icon.loadFromFile("Debug\\content\\icon.png");
#else
	icon.loadFromFile("content\\icon.png");
#endif
	window.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());

	sf::Font font;
#ifdef DEBUG
	font.loadFromFile("Debug\\content\\JFJunRoc.ttf");
#else
	font.loadFromFile("content\\JFJunRoc.ttf");
#endif
	sf::Text text;
	text.setOutlineColor(sf::Color::Black);
	text.setOutlineThickness(3);
	text.setFont(font);
	sf::Clock eventTimer;
	eventTimer.restart();

	int difficult = 1;
	std::string nickname = "default";

	while (window.isOpen()) {
		if (menu == Menu::mainMenu) {
			eventTimer.restart();
			int main_menu_position = 1;
			/*
			1 - Start
			2 - Records
			3 - Exit
			*/
			while (menu == Menu::mainMenu) {
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
									while (flag) {
										window.clear();
										while (window.pollEvent(event)) {
											if (event.type == sf::Event::Closed) {
												window.close();
											}
											if (eventTimer.getElapsedTime().asMilliseconds() > 150) {
												if (event.key.code == sf::Keyboard::Escape) {
													flag = false;
													break;
												}
												if (event.key.code == sf::Keyboard::BackSpace && nickname.length()!= 0) {
													nickname.pop_back();
												}
												if (event.key.code >= 0 && event.key.code <= 25) {
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
											text.setString("< Your nick " + nickname + " >");
										}
										else {
											text.setString("Your nick " + nickname);
										}
										text.setOrigin(text.getLocalBounds().left + text.getLocalBounds().width / 2, 0);
										text.setPosition(texture_background.getSize().x * 0.3, texture_background.getSize().y * 0.9);
										window.draw(text);

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
					text.setString("< Your nick " + nickname + " >");
				}
				else {
					text.setString("Your nick " + nickname);
				}
				text.setOrigin(text.getLocalBounds().left + text.getLocalBounds().width / 2, 0);
				text.setPosition(texture_background.getSize().x * 0.3, texture_background.getSize().y * 0.9);
				window.draw(text);

				window.display();
			}
		}
		else if (menu == Menu::records) {
#ifdef DEBUG
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
			while (menu == Menu::records) {
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
					text.setPosition(texture_background.getSize().x * 0.1, texture_background.getSize().y * (i + 1) / 10);
					window.draw(text);
				}
				window.display();
			}
		}
		else if (menu == Menu::choosingDifficult) {
			eventTimer.restart();
			while (menu == Menu::choosingDifficult) {
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

				window.display();
			}
		}
		else if (menu == Menu::game) {
		
		}
	}
	return 0;
}