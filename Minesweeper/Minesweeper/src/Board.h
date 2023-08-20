#pragma once
#include "Tile.h"
#include <fstream>
#include <random>


class Board
{
private:
	RenderWindow _window;
	Tile* tiles[25][16];
	map<string, Texture> availableTextures;
	int mineCount;
	bool gameEnabled;
	bool gameWon;
	bool allMinesRevealed;
	void generateRandomBoard();
	void populateTextureMap();
	void drawButtons();
	void drawTiles();
	void drawScore();
	void loadBoard(int testBoardNum);
	void leftClick();
	void rightClick();
	void assignNeighbors();
	void gameOver();
	void revealAllMines();
	void determineIfWinner();
	void calcMineCount();
public:
	Board();
};

