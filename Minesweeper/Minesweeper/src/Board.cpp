#include "Board.h"

Board::Board() : _window(VideoMode(800, 600), "Minesweeper") {
    populateTextureMap();
    generateRandomBoard();
    gameEnabled = true;
    gameWon = false;
    allMinesRevealed = false;
    while (_window.isOpen())
    {
        Event event;
        while (_window.pollEvent(event))
        {
            if (event.type == Event::Closed)
                _window.close();
            if (event.type == Event::MouseButtonPressed) {
                if (event.mouseButton.button == Mouse::Left) {
                    leftClick();
                }
                if (event.mouseButton.button == Mouse::Right) {
                    rightClick();
                }
            }
        }
        _window.clear(Color::White);
        drawTiles();
        determineIfWinner();
        drawButtons();
        drawScore();
        _window.display();
    }
}

void Board::loadBoard(int testBoardNum) {
    
    string filename;
    switch (testBoardNum) {
    case 1: filename = "./boards/testboard1.brd"; break;
    case 2: filename = "./boards/testboard2.brd"; break;
    case 3: filename = "./boards/testboard3.brd"; break;
    default: return;
    }
    ifstream board(filename);

    for (int y = 0; y < 16; ++y) {
        std::string line;
        getline(board, line);
        for (int x = 0; x < 25; ++x) {
            bool isMine = (line[x] == '1');
            tiles[x][y] = new Tile(isMine, x * 32, y * 32);
        }
     }
    gameEnabled = true;
    gameWon = false;
    allMinesRevealed = false;
    assignNeighbors();
    calcMineCount();
}

void Board::drawButtons() {
    Sprite sprite1(availableTextures["test_1"]);
    sprite1.setPosition(688, 512);
    _window.draw(sprite1);


    Sprite sprite2(availableTextures["test_2"]);
    sprite2.setPosition(624, 512);
    _window.draw(sprite2);


    Sprite sprite3(availableTextures["test_3"]);
    sprite3.setPosition(560, 512);
    _window.draw(sprite3);


    Sprite spriteDebug(availableTextures["debug"]);
    spriteDebug.setPosition(496, 512);
    _window.draw(spriteDebug);

    if (gameEnabled) {
        Sprite spriteHappy(availableTextures["face_happy"]);
        spriteHappy.setPosition(368, 512);
        _window.draw(spriteHappy);
    }
    if (gameWon) {
        Sprite sprite(availableTextures["face_win"]);
        sprite.setPosition(368, 512);
        _window.draw(sprite);
    }
    if (!gameEnabled && !gameWon) {
        Sprite sprite(availableTextures["face_lose"]);
        sprite.setPosition(368, 512);
        _window.draw(sprite);
    }

}

void Board::drawTiles() {
    for (int x = 0; x < 25; x++)
    {
        for (int y = 0; y < 16; y++)
        {
            Sprite sprite;

            if (tiles[x][y]->isRevealed())
            {
                if (tiles[x][y]->isMine() && !gameWon)
                    sprite.setTexture(availableTextures["mine"]);
                else if (tiles[x][y]->getAdjacentMineCount() == 0)
                    sprite.setTexture(availableTextures["tile_revealed"]);
                else if (tiles[x][y]->getAdjacentMineCount() == 1)
                    sprite.setTexture(availableTextures["number_1"]);
                else if (tiles[x][y]->getAdjacentMineCount() == 2)
                    sprite.setTexture(availableTextures["number_2"]);
                else if (tiles[x][y]->getAdjacentMineCount() == 3)
                    sprite.setTexture(availableTextures["number_3"]);
                else if (tiles[x][y]->getAdjacentMineCount() == 4)
                    sprite.setTexture(availableTextures["number_4"]);
                else if (tiles[x][y]->getAdjacentMineCount() == 5)
                    sprite.setTexture(availableTextures["number_5"]);
                else if (tiles[x][y]->getAdjacentMineCount() == 6)
                    sprite.setTexture(availableTextures["number_6"]);
                else if (tiles[x][y]->getAdjacentMineCount() == 7)
                    sprite.setTexture(availableTextures["number_7"]);
                else if (tiles[x][y]->getAdjacentMineCount() == 8)
                    sprite.setTexture(availableTextures["number_8"]);
            }
            else if (tiles[x][y]->isFlag()) {
                sprite.setTexture(availableTextures["flag"]);
            }
            else
                sprite.setTexture(availableTextures["tile_hidden"]);

            sprite.setPosition(x * 32, y * 32);
            _window.draw(sprite);
        }
    }
}

void Board::drawScore() {
    Sprite firstDigit(availableTextures["digits"]);
    Sprite secondDigit(availableTextures["digits"]);
    Sprite thirdDigit(availableTextures["digits"]);
    Sprite negative(availableTextures["digits"]);
    int absMineCount = abs(mineCount); 
    int firstDigitValue = absMineCount / 100;
    int secondDigitValue = (absMineCount % 100) / 10;
    int thirdDigitValue = absMineCount % 10;
    IntRect neg(210, 0, 21, 32);
    IntRect first(21 * firstDigitValue, 0, 21, 32);
    IntRect second(21 * secondDigitValue, 0, 21, 32);
    IntRect third(21 * thirdDigitValue, 0, 21, 32);
    negative.setTextureRect(neg);
    firstDigit.setTextureRect(first);
    secondDigit.setTextureRect(second);
    thirdDigit.setTextureRect(third);
    if (mineCount < 0) {
        negative.setPosition(0, 512);
        firstDigit.setPosition(21, 512);
        secondDigit.setPosition(42, 512);
        thirdDigit.setPosition(63, 512);
        _window.draw(negative);
        _window.draw(firstDigit);
        _window.draw(secondDigit);
        _window.draw(thirdDigit);
    }
    else {
        firstDigit.setPosition(0, 512);
        secondDigit.setPosition(21, 512);
        thirdDigit.setPosition(42, 512);
        _window.draw(firstDigit);
        _window.draw(secondDigit);
        _window.draw(thirdDigit);
    } 
}

void Board::populateTextureMap() {
    Texture debug;
    debug.loadFromFile("./images/debug.png");
    availableTextures.emplace("debug", debug);

    Texture digits;
    digits.loadFromFile("./images/digits.png");
    availableTextures.emplace("digits", digits);

    Texture face_happy;
    face_happy.loadFromFile("./images/face_happy.png");
    availableTextures.emplace("face_happy", face_happy);

    Texture face_lose;
    face_lose.loadFromFile("./images/face_lose.png");
    availableTextures.emplace("face_lose", face_lose);

    Texture face_win;
    face_win.loadFromFile("./images/face_win.png");
    availableTextures.emplace("face_win", face_win);

    Texture flag;
    flag.loadFromFile("./images/flag.png");
    availableTextures.emplace("flag", flag);

    Texture mine;
    mine.loadFromFile("./images/mine.png");
    availableTextures.emplace("mine", mine);

    Texture number_1;
    number_1.loadFromFile("./images/number_1.png");
    availableTextures.emplace("number_1", number_1);

    Texture number_2;
    number_2.loadFromFile("./images/number_2.png");
    availableTextures.emplace("number_2", number_2);

    Texture number_3;
    number_3.loadFromFile("./images/number_3.png");
    availableTextures.emplace("number_3", number_3);

    Texture number_4;
    number_4.loadFromFile("./images/number_4.png");
    availableTextures.emplace("number_4", number_4);

    Texture number_5;
    number_5.loadFromFile("./images/number_5.png");
    availableTextures.emplace("number_5", number_5);

    Texture number_6;
    number_6.loadFromFile("./images/number_6.png");
    availableTextures.emplace("number_6", number_6);

    Texture number_7;
    number_7.loadFromFile("./images/number_7.png");
    availableTextures.emplace("number_7", number_7);

    Texture number_8;
    number_8.loadFromFile("./images/number_8.png");
    availableTextures.emplace("number_8", number_8);

    Texture test_1;
    test_1.loadFromFile("./images/test_1.png");
    availableTextures.emplace("test_1", test_1);

    Texture test_2;
    test_2.loadFromFile("./images/test_2.png");
    availableTextures.emplace("test_2", test_2);

    Texture test_3;
    test_3.loadFromFile("./images/test_3.png");
    availableTextures.emplace("test_3", test_3);

    Texture tile_hidden;
    tile_hidden.loadFromFile("./images/tile_hidden.png");
    availableTextures.emplace("tile_hidden", tile_hidden);

    Texture tile_revealed;
    tile_revealed.loadFromFile("./images/tile_revealed.png");
    availableTextures.emplace("tile_revealed", tile_revealed);
}

void Board::gameOver() {
    if (!allMinesRevealed) {
        revealAllMines();
    }
    gameEnabled = false;
}

void Board::determineIfWinner() {
    gameWon = true;
    for (int x = 0; x < 25; x++)
    {
        for (int y = 0; y < 16; y++) 
        {
            if (tiles[x][y]->isMine()) {
                continue;
            }
            else if (!tiles[x][y]->isMine()) {
                if (!tiles[x][y]->isRevealed()) {
                    gameWon = false;
                    return;
                }
            }
        }
    }
    for (int x = 0; x < 25; x++)
    {
        for (int y = 0; y < 16; y++)
        {
            if (tiles[x][y]->isMine()) {
                tiles[x][y]->flagTile();
            }
        }
    }
}

void Board::leftClick() {
    
    Vector2i clickPixelXY = Mouse::getPosition(_window);
    Vector2f clickPos = _window.mapPixelToCoords(clickPixelXY);
    int tileX = static_cast<int>(clickPos.x) / 32;
    int tileY = static_cast<int>(clickPos.y) / 32;
    if (tileX >= 0 && tileX < 25 && tileY >= 0 && tileY < 16 && gameEnabled)
    {
        if (!tiles[tileX][tileY]->isFlag() && !tiles[tileX][tileY]->isMine())
            tiles[tileX][tileY]->revealTile();
        if (!tiles[tileX][tileY]->isFlag() && tiles[tileX][tileY]->isMine()) {
            gameOver();
        }
    }
    if (clickPixelXY.x >= 496 && clickPixelXY.x <= 560 && clickPixelXY.y >= 512 && clickPixelXY.y <= 576) { // Debug button
        revealAllMines();
    }
    if (clickPixelXY.x >= 560 && clickPixelXY.x <= 624 && clickPixelXY.y >= 512 && clickPixelXY.y <= 576) { // Test 1
        loadBoard(1);
    }
    if (clickPixelXY.x >= 624 && clickPixelXY.x <= 688 && clickPixelXY.y >= 512 && clickPixelXY.y <= 576) { // Test 2
        loadBoard(2);
    }
    if (clickPixelXY.x >= 688 && clickPixelXY.x <= 752 && clickPixelXY.y >= 512 && clickPixelXY.y <= 576) { // Test 3
        loadBoard(3);
    }
    if (clickPixelXY.x >= 368 && clickPixelXY.x <= 432 && clickPixelXY.y >= 512 && clickPixelXY.y <= 576) { // Reset Game
        generateRandomBoard();
    }
}

void Board::rightClick() {
    Vector2i clickPixelXY = Mouse::getPosition(_window);
    Vector2f clickPos = _window.mapPixelToCoords(clickPixelXY);
    int tileX = static_cast<int>(clickPos.x) / 32;
    int tileY = static_cast<int>(clickPos.y) / 32;
    if (tileX >= 0 && tileX < 25 && tileY >= 0 && tileY < 16 && gameEnabled)
    {
        if (!tiles[tileX][tileY]->isFlag()) {
            tiles[tileX][tileY]->flagTile();
            mineCount--;
        }
        else {
            tiles[tileX][tileY]->removeFlag();
            mineCount++;
        }

    }
}

void Board::generateRandomBoard() {

    default_random_engine engine(random_device{}());
    uniform_int_distribution<int> distX(0, 24);
    uniform_int_distribution<int> distY(0, 15);

    for (int i = 0; i < 25; ++i) {
        for (int j = 0; j < 16; ++j) {
            delete tiles[i][j];
            tiles[i][j] = new Tile(false, i * 32, j * 32);
        }
    }
    for (int i = 0; i < 50; ++i) {
        while (true) {
            int x = distX(engine);
            int y = distY(engine);

            if (!tiles[x][y]->isMine()) {
                tiles[x][y]->activateMine();
                break;
            }
        }
    }
    gameWon = false;
    gameEnabled = true;
    allMinesRevealed = false;
    assignNeighbors();
    calcMineCount();

}

void Board::assignNeighbors() {
    for (int x = 0; x < 25; ++x) {
        for (int y = 0; y < 16; ++y) {
            Tile* currentTile = tiles[x][y];
            int adjacentMineCount = 0;
            for (int dx = -1; dx <= 1; ++dx) {
                for (int dy = -1; dy <= 1; ++dy) {
                    if (dx == 0 && dy == 0)
                        continue;
                    int neighborX = x + dx;
                    int neighborY = y + dy;
                    int index = (dy + 1) * 3 + (dx + 1);
                    if (neighborX >= 0 && neighborX < 25 && neighborY >= 0 && neighborY < 16) {
                        Tile* neighborTile = tiles[neighborX][neighborY];
                        currentTile->setNeighbor(index, neighborTile);
                        if (neighborTile->isMine())
                            ++adjacentMineCount;
                    }
                }
            }
            currentTile->assignMineCount(adjacentMineCount);
        }
    }
}

void Board::revealAllMines() {
    for (int x = 0; x < 25; x++)
    {
        for (int y = 0; y < 16; y++)
        {   
            if (tiles[x][y]->isMine() && !tiles[x][y]->isRevealed()) {
                tiles[x][y]->revealMine();
            }
            else if (tiles[x][y]->isMine() && !tiles[x][y]->isRevealed() && tiles[x][y]->isFlag()) {
                tiles[x][y]->revealMine();
            }
            else if (tiles[x][y]->isMine() && tiles[x][y]->isRevealed())
                tiles[x][y]->hideTile();
            }
        }
    if (!allMinesRevealed) { allMinesRevealed = true; }
    else { allMinesRevealed = false; }
    }

void Board::calcMineCount() {
    mineCount = 0;
    for (int x = 0; x < 25; x++)
    {
        for (int y = 0; y < 16; y++)
        {
            if (tiles[x][y]->isMine() && !tiles[x][y]->isFlag()) {
                mineCount++;
            }
        }
    }
}
