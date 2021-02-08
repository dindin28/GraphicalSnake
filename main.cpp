#include <SFML/Graphics.hpp>
#include <string>
#include <windows.h>

#define DEBUG
#ifdef DEBUG
	#include <iostream>
#endif

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
		
		}
		else if (menu == Menu::choosingDifficult) {
			
		}
		else if (menu == Menu::game) {
		
		}
	}
	return 0;
}
/*
window.clear();
			while (window.pollEvent(event)) {
				if (event.type == sf::Event::Closed) {
					window.close();
				}
				if (eventTimer.getElapsedTime().asMilliseconds() > 200) { //0.2 seconds
					if (event.key.code == sf::Keyboard::Down) {
						difficult++;
						if (difficult > 3) difficult = 1;
						eventTimer.restart();
					}
					else if (event.key.code == sf::Keyboard::Up) {
						difficult--;
						if (difficult < 1) difficult = 3;
						eventTimer.restart();
					}
					else if (event.key.code == sf::Keyboard::Escape) {
						menu = Menu::mainMenu;
						break;
					}
					else if (event.key.code == sf::Keyboard::Enter) {
						switch (difficult) {
						case(1): {
							break;
						}
						case(2): {
							break;
						}
						case(3): {
							break;
						}
							   eventTimer.restart();
						}
					}
				}
				window.draw(sprite_background);
				text.setString("Choose difficult");
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

				text.setString("Your nick " + nickname);
				text.setOrigin(text.getLocalBounds().left + text.getLocalBounds().width / 2, 0);
				text.setPosition(texture_background.getSize().x * 0.3, texture_background.getSize().y * 0.9);
				window.draw(text);

				window.display();
			}
				}*/