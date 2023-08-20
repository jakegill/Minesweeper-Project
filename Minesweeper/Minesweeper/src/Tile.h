#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <string>
#include <iostream>
using namespace std;
using namespace sf;

class Tile
{
private:
	bool mine;
	bool flag;
	bool revealed;
	int posX;
	int posY;
	int width;
	int height;
	Tile* neighbors[8] = { nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr };
	int adjacentMineCount;

public:
	Tile(bool mine, int posX, int posY);
	void revealTile();
	void revealMine();
	void hideTile();
	void activateMine();
	void deactivateMine();
	void flagTile();
	void removeFlag();
	bool isMine();
	bool isFlag();
	bool isRevealed();
	void setNeighbor(int index, Tile* neighbor);
	void assignMineCount(int count);
	int getAdjacentMineCount();
};

