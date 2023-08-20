#include "Tile.h"

Tile::Tile(bool mine, int posX, int posY) {
	this->mine = mine;
	this->revealed = false;
	this->flag = false;
	this->width = 32;
	this->height = 32;
	this->posX = posX;
	this->posY = posY;
}

bool Tile::isMine() {
	return mine;
}

bool Tile::isRevealed() {
	return revealed;
}

bool Tile::isFlag() {
	return flag;
}

void Tile::revealTile() {
	if (revealed) 
		return;

	this->revealed = true;
	if (adjacentMineCount == 0) {
		for (int i = 0; i < 8; ++i) { 
			Tile* neighbor = neighbors[i];
			if (neighbor != nullptr && !neighbor->isFlag()) {
				neighbor->revealTile();
			}
		}
	}
}

void Tile::revealMine() {
	this->revealed = true;
}

void Tile::hideTile() {
	this->revealed = false;
}

void Tile::activateMine() {
	this->mine = true;
}

void Tile::flagTile() {
	this->flag = true;
}

void Tile::removeFlag() {
	this->flag = false;
}

void Tile::setNeighbor(int index, Tile* neighbor) {
	neighbors[index] = neighbor;
}

void Tile::assignMineCount(int count) {
	this->adjacentMineCount = count;
}

int Tile::getAdjacentMineCount() {
	return adjacentMineCount;
}