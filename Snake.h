#pragma once
#include <SFML/Graphics.hpp>

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

	sf::IntRect setTile(Tiles tile);

	struct Point {
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
		sf::RenderWindow* window;
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
		Snake(sf::RenderWindow& window, sf::Vector2u sizeOfRender);
		void showGF();

		void clearGF();

		void spawnFood();

		int getSizeOfSnake();

		void moveSnake();

		int checkBorders(int x, int y);

		//movement
		void changeMovement(int i);

		bool getFinished();

		~Snake();
	};
}