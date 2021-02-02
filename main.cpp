#include <SFML/Graphics.hpp>
#include <string>
#include <iostream>

#define DEBUG

using std::cout;
using std::endl;

int main()
{
	//window and background
	sf::RenderWindow window(sf::VideoMode(1000, 1000), "Snake");
	sf::Sprite sprite_background;
	sf::Texture texture_background;
#ifdef DEBUG
	texture_background.loadFromFile("Debug\\content\\main_menu_background.jpg");
#else
	texture.loadFromFile("content\\main_menu_background.jpg");
#endif
	window.setSize(texture_background.getSize());
	sprite_background.setTexture(texture_background);
	//window.setSize(sf::Vector2u(texture_background.getSize().x, texture_background.getSize().y));
	

	//icon
	sf::Image icon;
#ifdef DEBUG
	icon.loadFromFile("Debug\\content\\icon.png");
#else
	icon.loadFromFile("content\\icon.png");
#endif
	window.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());

	//creating menu
	sf::Font font;
	sf::Text text;
#ifdef DEBUG
	font.loadFromFile("Debug\\content\\JFJunRoc.ttf");
#else
	font.loadFromFile("content\\JFJunRoc.ttf");
#endif
	text.setFont(font);
	text.setFillColor(sf::Color::White);
	text.setOutlineColor(sf::Color::Black);
	text.setOutlineThickness(3);


	while (window.isOpen())
	{
		window.clear();
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}
		//background
		window.draw(sprite_background);
		
		//main menu text
		text.setCharacterSize(50);
		text.setString("WELCOME TO THE JUNGLE");
		sf::FloatRect textRect = text.getLocalBounds();
		text.setPosition(0, 0);
		cout << text.getGlobalBounds().width << endl;
		window.draw(text);

		window.display();
	}
	return 0;
}