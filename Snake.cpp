#include "Snake.h"

namespace snake {
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
	};

	Snake::Snake(sf::RenderWindow& window, sf::Vector2u sizeOfRender) {
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
		tileSprite.setScale(tileSize.x / tileTexture.getSize().y, tileSize.y / tileTexture.getSize().y);
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
	void Snake::showGF() {
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
						else if (prevMovement == 2) {
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

	void Snake::clearGF() {
		for (char* iter = headOfGF; iter != (headOfGF + sizeGF * sizeGF); iter++) {
			*(iter) = ' ';
		}
	}

	void Snake::spawnFood() {
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
		int randBuff = rand() % (sizeGF * sizeGF - getSizeOfSnake()) - 1;
		*(headOfGF + ptr[randBuff].y * sizeGF + ptr[randBuff].x) = 'X';
		delete[sizeGF * sizeGF - getSizeOfSnake()] ptr;
	}

	int Snake::getSizeOfSnake() {
		Node* temp = headOfSnake;
		int counter = 0;
		while (temp) {
			temp = temp->next;
			counter++;
		}
		return counter;
	}

	void Snake::moveSnake() {
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

	int Snake::checkBorders(int x, int y) {
		if (x >= sizeGF || x < 0 || y >= sizeGF || y < 0 || *(headOfGF + sizeGF * y + x) == 'o') {
			return 0;
		}
		else if (*(headOfGF + sizeGF * y + x) == 'X') {
			return 2;
		}
		else return 1;
	}

		//movement
	void Snake::changeMovement(int i) {
		if (i == 1 && prevMovement != 3 || i == 2 && prevMovement != 4 || i == 3 && prevMovement != 1 || i == 4 && prevMovement != 2) {
			movement = i;
		}
	}

	bool Snake::getFinished() {
			return finished;
	}

	Snake::~Snake() {
			int sizeOfSnake = getSizeOfSnake();
			Node* iter = headOfSnake;
			for (int i = 0; i < sizeOfSnake; i++) {
				iter = iter->next;
				delete headOfSnake;
				headOfSnake = iter;
			}
			delete[sizeGF * sizeGF] headOfGF;
		}
	};