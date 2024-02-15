/*===============================
* minesweeper++ - file mine.cpp
* ===============================
* author: ben staehle
* date:	  10/30/22
* this program (and file) use the GNU GPL v3 License
* see COPYRIGHT.txt for license related information
* ==================================================*/
#include"mine.h"
#include"tile.h"
#include"engine.h"
#include"toolbar.h"
#include"component.h"
#include<windows.h>
#include<random>
#include<array>
#include<algorithm>
#include<cmath>
#include<iostream>

//define win32 console text colors
#define GREEN 2
#define WHITE 7
#define MENU_HEIGHT 70

namespace mine {
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

	//called once when the game begins
	//numMines, rows, and cols are optional params
	//only intended to be filled when the difficulty is custom
	void init(difficulty dif, gameMode gm, engine::GameEngine* ref, int numMines, int rows, int cols, float sx, float sy) {
		//customizable values
		gameState::currDiff = dif;
		gameState::gmode = gm;

		ref->newGame = true;

		//determine number of mines
		switch (gameState::currDiff) {
		case CUSTOM:
			if (numMines >= rows * cols || numMines == 0) {
				gameState::numMines = (rows * cols) / 2;
			}
			else if (numMines < 2) {
				gameState::numMines = 2;
			}
			else {
				gameState::numMines = numMines;
			}

			if (rows < 8) {
				gameState::rows = 8;
			}
			else {
				gameState::rows = rows;
			}
			if (cols < 8) {
				gameState::cols = 8;
			}
			else {
				gameState::cols = cols;
			}

			break;
		case BEGINNER:
			gameState::numMines = 10;
			gameState::rows = 9;
			gameState::cols = 9;
			break;
		case INTERMEDIATE:
			gameState::numMines = 40;
			gameState::rows = 16;
			gameState::cols = 16;
			break;
		case ADVANCED:
			gameState::numMines = 99;
			gameState::rows = 24;
			gameState::cols = 24;
			break;
		default: //same as intermediate
			gameState::numMines = 40;
			gameState::rows = 16;
			gameState::cols = 16;
			break; 
		}

		gameState::gptr = ref;

		//sync size so render window is scaled correctly
		ref->width = gameState::rows;
		ref->height = gameState::cols;

		gameState::numCovered = gameState::rows * gameState::cols;

		//default value
		gameState::gstate = PLAYING;

		//set up the board
		using g = gameState;
		gameState::gboard = std::make_shared<util::Table_2d<Tile>>(g::rows, g::cols);

		//generate the board
		gen(ref);

		//lastly set the window size correctly
		ref->window->setSize(sf::Vector2u(g::cols * TILE_SIZE, (g::rows * TILE_SIZE) + MENU_HEIGHT));
		sf::FloatRect visiableArea(0, 0, g::cols * TILE_SIZE, (g::rows * TILE_SIZE) + MENU_HEIGHT);
		ref->window->setView(sf::View(visiableArea));	

		gameState::gptr->newGame = false;
	}

	//called once when the game begins - or when the game is restarted
	//generates the game board by placing mines at random locations
	//then placing the appropriate tile on every tile that is not a mine
	void gen(engine::GameEngine* ref) {
		//create generator
		std::random_device rd;
		std::mt19937 gen32(rd()); //mersenne twister algorithm

		using g = gameState;
		int count = 0; 
		std::vector<std::pair<int, int>> mines;

		#ifdef _DEBUG
		std::cout << "placing mines..." << count << std::endl;
		#endif

		//first pass to set mines
		for (int i = g::numMines; i > 0;) {
			//for each mine we want to place, generate a random x and y
			int r_x = (gen32() % (g::rows));
			int r_y = (gen32() % (g::cols));

			if (!(std::find(mines.begin(), mines.end(), std::pair{ r_x, r_y }) != mines.end())) {
				gameState::gboard->set(Tile((float)(r_x * TILE_SIZE) + gameState::boss_offset_x, ((float)(r_y * TILE_SIZE) + MENU_HEIGHT) + gameState::boss_offset_y, -1, true, ref), r_x, r_y);
				mines.push_back({ r_x, r_y });  //once that location has been ocupied, add it to the list so it can't be chosen again
				count++;
				i--;
			}

			//count the mines in 3x3 mode
			gen3x3(mines, ref);
		}

		//set the global mines list
		gameState::currMines = mines;

		//set the first pass flag
		gameState::isFirstPass = false;

		#ifdef _DEBUG
		SetConsoleTextAttribute(hConsole, GREEN);
		std::cout << "done! " << count << std::endl;
		#endif	
	}

	//changes the numbers to 5x5 mode
	//does not place new mines
	void gen5x5(std::vector<std::pair<int, int>>& mines, engine::GameEngine* ref) {
		#ifdef _DEBUG
		SetConsoleTextAttribute(hConsole, WHITE);
		std::cout << "counting mines 5x5... " << std::endl;
		#endif

		int zeros = 0;
		//second pass to set numbers
		//place a number everywhere there is not a mine
		//using the 'mines' list of pairs of coordinates that denote each mine's location
		for (int i = 0; i < gameState::rows; i++) {
			for (int k = 0; k < gameState::cols; k++) {
				if (!(std::find(mines.begin(), mines.end(), std::pair{ i, k }) != mines.end())) {
					bool revealed = false;
					if (!gameState::isFirstPass && !gameState::gboard->get(i, k).isMine) {
						revealed = gameState::gboard->get(i, k).isRevealed;
					}
					gameState::gboard->set(Tile((float)(i * TILE_SIZE) + gameState::boss_offset_x, ((float)(k * TILE_SIZE) + MENU_HEIGHT) + gameState::boss_offset_y, countAdjacentMines5x5(i, k), false, ref), i, k);
					gameState::gboard->getptr(i, k)->value5x5 = countAdjacentMines5x5(i, k);
					zeros++;

					//reveal the tile if it has already been uncovered
					if (revealed) {
						uncover(i * TILE_SIZE, k * TILE_SIZE, true);
					}

					//janky
					if (!gameState::gptr->newGame) {
						toggleColor();
						toggleColor();
					}
				}
			}
		}

		#ifdef _DEBUG
		SetConsoleTextAttribute(hConsole, GREEN);
		std::cout << "done! " << zeros << std::endl;
		SetConsoleTextAttribute(hConsole, WHITE);
		#endif 
	}

	//changed the numbers to 3x3 mode
	//does not place mines
	void gen3x3(std::vector<std::pair<int, int>>& mines, engine::GameEngine* ref) {
		SetConsoleTextAttribute(hConsole, WHITE);

		#ifdef _DEBUG
		std::cout << "counting mines 3x3... " << std::endl;
		#endif

		int zeros = 0;
		//second pass to set numbers
		//place a number everywhere there is not a mine
		//using the 'mines' list of pairs of coordinates that denote each mine's location
		for (int i = 0; i < gameState::rows; i++) {
			for (int k = 0; k < gameState::cols; k++) {
				if (!(std::find(mines.begin(), mines.end(), std::pair{ i, k }) != mines.end())) {
					bool revealed = false;
					if (!gameState::isFirstPass && !gameState::gboard->get(i, k).isMine) {
						revealed = gameState::gboard->get(i, k).isRevealed;
					}
					gameState::gboard->set(Tile((float)(i * TILE_SIZE) + gameState::boss_offset_x, ((float)(k * TILE_SIZE) + MENU_HEIGHT) + +gameState::boss_offset_y, countAdjacentMines(i, k), false, ref), i, k);
					zeros++;

					//reveal the tile if it has already been uncovered
					if (revealed) {
						uncover(i * TILE_SIZE, k * TILE_SIZE, true);
					}

					//janky
					if (!gameState::gptr->newGame) {
						toggleColor();
						toggleColor();
					}
				}
			}
		}

		#ifdef _DEBUG
				SetConsoleTextAttribute(hConsole, GREEN);
				std::cout << "done! " << zeros << std::endl;
				SetConsoleTextAttribute(hConsole, WHITE);
		#endif // DEBUG
	}

	//set the texture atlas offset and game style enum value
	void setColor(style color) {
		gameState::color = color;
		if (color == DARK) {
			Tile::tex_offset = (4 * TILE_SIZE);
		}
		else {
			Tile::tex_offset = 0;
		}
	}

	//set the color immediatly to any value
	//without having to toggle
	void setColorImm(style color) {
		gameState::color = color;
		if (color == DARK) {
			Tile::tex_offset = (4 * TILE_SIZE);
		}
		else {
			Tile::tex_offset = 0;
		}

		//swap current colors for each tile
		for (int i = 0; i < gameState::rows; i++) {
			for (int k = 0; k < gameState::cols; k++) {
				gameState::gboard->getptr(i, k)->flipColor();
			}
		}
	}

	//toggle to the 'other' color, whichever is not current
	void toggleColor() {
		if (gameState::color == DARK) {
			setColor(LIGHT);	
		}
		else {
			setColor(DARK);
		}
		//swap current colors for each tile
		for (int i = 0; i < gameState::rows; i++) {
			for (int k = 0; k < gameState::cols; k++) {
				gameState::gboard->getptr(i, k)->flipColor();
			}
		}
		gameState::gptr->toolbar->flipColor();
	}

	void toggle5x5() {
		if (gameState::gmode == THREE) {
			//change it to 5
			gameState::gmode = FIVE;
			gen5x5(gameState::currMines, gameState::gptr);
		}
		else {
			//change it to 3
			gameState::gmode = THREE;
			gen3x3(gameState::currMines, gameState::gptr);
		}
	}

	//note: x and y are SCREEN COORDS
	void revealShort3x3(int x, int y) {
		//check if there are 3 mines in it's radius
		int at_x = ceil(x / TILE_SIZE);
		int at_y = ceil((y - MENU_HEIGHT) / TILE_SIZE);
		int adjacent = 0;

		if(!gameState::gboard->get(at_x, at_y).isRevealed) {
			return;
		}
		else {
			for (int i = at_x - 1; i < at_x + 2; i++) {
				for (int k = at_y - 1; k < at_y + 2; k++) {
					if ((i >= 0 && i < gameState::cols) && (k >= 0 && k < gameState::rows)) {
						if (gameState::gboard->get(i, k).isFlagged) {
							adjacent++;
						}
					} 
				}
			}

			if (adjacent == gameState::gboard->get(at_x, at_y).value) {
				//uncover each tile
				for (int i = at_x - 1; i < at_x + 2; i++) {
					for (int k = at_y - 1; k < at_y + 2; k++) {
						if ((i >= 0 && i < gameState::cols) && (k >= 0 && k < gameState::rows)) {
							uncoverImm(i, k, false);
						}
					}
				}
			} 
		}
	}

	//note: x and y are SCREEN COORDS
	void revealShort5x5(int x, int y) {
		//check if there are 3 mines in it's radius
		int at_x = ceil(x / TILE_SIZE);
		int at_y = ceil((y - MENU_HEIGHT) / TILE_SIZE);
		int adjacent = 0;

		if (gameState::gboard->get(at_x, at_y).isMine) {
			loseGame(at_x, at_y);
		}
		else {
			for (int i = at_x - 2; i < at_x + 3; i++) {
				for (int k = at_y - 2; k < at_y + 3; k++) {
					if ((i >= 0 && i < gameState::cols) && (k >= 0 && k < gameState::rows)) {
						if (gameState::gboard->get(i, k).isFlagged) {
							adjacent++;
						}
					}
				}
			}

			if (adjacent == gameState::gboard->get(at_x, at_y).value5x5) {
				//uncover each tile
				for (int i = at_x - 2; i < at_x + 3; i++) {
					for (int k = at_y - 2; k < at_y + 3; k++) {
						if ((i >= 0 && i < gameState::cols) && (k >= 0 && k < gameState::rows)) {
							uncoverImm(i, k, false);
						}
					}
				}
			}
		}
	}

	//uncovers the tile at screen loc (x, y)
	void uncover(int x, int y, bool isToggle) {
		//round all mouse click coords up or right one tile if needed
		int at_x = ceil(x / TILE_SIZE);
		int at_y = ceil(y / TILE_SIZE);

		//check if out of bounds
		if(at_x >= gameState::rows || at_y > gameState::cols || at_x < 0 || at_y < 0) {
			return;
		}

		if (!gameState::gboard->get(at_x, at_y).isRevealed) {
			if (gameState::gboard->get(at_x, at_y).isFlagged) { return; }

			//check if the tile was a mine - game lost 
			if (gameState::gboard->get(at_x, at_y).isMine) {
				loseGame(at_x, at_y);
				return;
			}

			gameState::gboard->getptr(at_x, at_y)->uncover();
			
			//do the funny
			//call the recursive depth first reveal method if the tile was a zero
			if (gameState::gboard->get(at_x, at_y).value == 0) {
				depthFirstReveal(at_x, at_y, isToggle);
			}
			else if (!isToggle){
				//otherwise just uncover that one and tally it up
				gameState::gboard->getptr(at_x, at_y)->isRevealed = true;
				gameState::numCovered--;
			}
			else {
				gameState::gboard->getptr(at_x, at_y)->isRevealed = true;
			}

			//check if all non-mines are uncovered - game is won
			if (checkWinCond()) {
				std::cout << "you win" << std::endl;
				winGame();
			}
		}	
	}

	void uncoverImm(int at_x, int at_y, bool isToggle) {
		//check if out of bounds
		if (at_x >= gameState::rows || at_y > gameState::cols || at_x < 0 || at_y < 0) {
			return;
		}

		if (!gameState::gboard->get(at_x, at_y).isRevealed) {
			if (gameState::gboard->get(at_x, at_y).isFlagged) { return; }

			//check if the tile was a mine - game lost 
			if (gameState::gboard->get(at_x, at_y).isMine) {
				loseGame(at_x, at_y);
				return;
			}

			gameState::gboard->getptr(at_x, at_y)->uncover();

			//do the funny
			//call the recursive depth first reveal method if the tile was a zero
			if (gameState::gboard->get(at_x, at_y).value == 0) {
				depthFirstReveal(at_x, at_y, isToggle);
			}
			else if (!isToggle) {
				//otherwise just uncover that one and tally it up
				gameState::gboard->getptr(at_x, at_y)->isRevealed = true;
				gameState::numCovered--;
			}
			else {
				gameState::gboard->getptr(at_x, at_y)->isRevealed = true;
			}

			//check if all non-mines are uncovered - game is won
			if (checkWinCond()) {
				std::cout << "you win" << std::endl;
				winGame();
			}
		}
	}

	//a helper function that helps the game count the number of mines
	//adjacent to a tile when the board is being generated
	int countAdjacentMines(int x, int y) {
		/*
		  counts the number of mines in the following radius...

		   N.W   N   N.E
			 \   |   /
			  \  |  /
		   W----Tile----E
				/ | \
			  /   |  \
		   S.W    S   S.E
		*/
		int xPos = x; //I don't know why i did this... 
		int yPos = y;
		int value = 0;

		using g = gameState;
		//check each neighbor of the source tile
		//this part could be condensed bigtime
		if (yPos != 0 && g::gboard->get(xPos, yPos - 1).value == -1) {
			value++;
		}
		//south
		if (yPos != (g::cols - 1) && g::gboard->get(xPos, yPos + 1).value == -1) {
			value++;
		}
		//east
		if (xPos != 0 && g::gboard->get(xPos - 1, yPos).value == -1) {
			value++;
		}
		//west
		if (xPos != (g::rows - 1) && g::gboard->get(xPos + 1, yPos).value == -1) {
			value++;
		}
		//northwest
		if ((xPos != 0 && yPos != 0) && g::gboard->get(xPos - 1, yPos - 1).value == -1) {
			value++;
		}
		//northeast
		if ((xPos != (g::rows - 1) && yPos != 0) && g::gboard->get(xPos + 1, yPos - 1).value == -1) {
			value++;
		}
		//southwest
		if ((xPos != 0 && yPos != (g::cols - 1)) && g::gboard->get(xPos - 1, yPos + 1).value == -1) {
			value++;
		}
		//southeast
		if ((xPos != (g::rows - 1) && yPos != (g::cols - 1)) && g::gboard->get(xPos + 1, yPos + 1).value == -1) {
			value++;
		}
		return value;
	}

	//counts all mines within a 5x5 radius of a tile
	int countAdjacentMines5x5(int x, int y) {
		/*
		  counts the number of mines in the following radius...
		     NNW   NN1   NN2  NN3  NNE
		       \    |    |    |    /
				\   |    |    |   /
				   N.W   N   N.E    
		    WW1------\   |   /-------EE1
					  \  |  /
			 WW----W----Tile----E----EE
						/ | \
			WW2--------/  |  \-------EE2
				   S.W    S   S.E
				    /     |     \
				   /      |      \
			     SSW      SS     SSE
		*/	

		int count = 0;
		using g = gameState;

		for (int sx = x - 2; sx < x + 3; sx++) {
			for (int sy = y - 2; sy < y + 3; sy++) {
				if (sx >= 0 && sy >= 0 && sx < g::rows && sy < g::cols && g::gboard->get(sx, sy).value == -1) {
					count++;
				}
			}
		}

		return count;
	}

	//called when a 'zero' tile is revealed
	//reveal every neighbor of every zero tile directly touching the one that was clicked
	//many hours have been spent here :|
	//a recursive function
	void depthFirstReveal(int x, int y, bool isToggle) {
		if (x < 0 || x >= gameState::rows || y < 0 || y >= gameState::cols) {
			//we've reached a border so back up a bit
			return;
		}
		if (gameState::gboard->get(x, y).value == 0 && !gameState::gboard->get(x, y).isRevealed && !gameState::gboard->get(x, y).isFlagged) {
			//in this case we need to keep decending in
			gameState::gboard->getptr(x, y)->uncover();
			gameState::gboard->getptr(x, y)->isRevealed = true;
			if (!isToggle) {
				gameState::numCovered--;
			}
			
			//nw
			depthFirstReveal(x - 1, y - 1, isToggle);
			//n
			depthFirstReveal(x, y - 1, isToggle);
			//ne
			depthFirstReveal(x + 1, y - 1, isToggle);
			//e
			depthFirstReveal(x + 1, y, isToggle);
			//se
			depthFirstReveal(x + 1, y + 1, isToggle);
			//s
			depthFirstReveal(x, y + 1, isToggle);
			//sw
			depthFirstReveal(x - 1, y + 1, isToggle);
			//w
			depthFirstReveal(x - 1, y, isToggle);
		}
		else {
			//otherwise reveal and move one level up
			//but do not blast past tiles that are already revealed
			if (!gameState::gboard->get(x, y).isRevealed) {
				if (!gameState::gboard->get(x, y).isFlagged) {
					gameState::gboard->getptr(x, y)->uncover();
					gameState::gboard->getptr(x, y)->isRevealed = true;
					if (!isToggle) {
						gameState::numCovered--;
					}
				}
			}
		}
	}

	// called every time a tile is revealed
	// checks if the game's win condition has been met
	bool checkWinCond() {
		using g = gameState;
		return g::numCovered == g::numMines;
	}

	//called when the game is won
	void winGame() {
		gameState::gstate = WIN;

		gameState::gptr->toolbar->smileyButton->setTexture(300, 30, TILE_SIZE, TILE_SIZE);

		//freeze the game
		gameState::gptr->haltGame();
	}

	//called when the game is lost
	void loseGame(int x, int y) {
		gameState::gstate = LOSE;

		//turn the tile red
		gameState::gboard->getptr(x, y)->sprite->setTextureRect(sf::IntRect(270, 90, TILE_SIZE, TILE_SIZE));
		gameState::gboard->getptr(x, y)->isRevealed = true;
		gameState::gboard->getptr(x, y)->isLosingTile = true;

		//turn to the frowny face
		gameState::gptr->toolbar->smileyButton->setTexture(270, 30 + Tile::tex_offset, TILE_SIZE, TILE_SIZE);
		gameState::gptr->toolbar->smileyButton->tbase_x = 270;

		//stop the game
		gameState::gptr->haltGame();

		//uncover all other mines
		for (int i = 0; i < gameState::rows; i++) {
			for (int k = 0; k < gameState::cols; k++) {
				if (gameState::gboard->get(i, k).isMine && !gameState::gboard->get(i, k).isRevealed) {
					gameState::gboard->getptr(i, k)->uncover();
				}
				else if (!gameState::gboard->get(i, k).isMine && gameState::gboard->get(i, k).isFlagged){
					gameState::gboard->getptr(i, k)->setFalseFlag();
				}
			}
		}
	}

	//called when a user right clicks (to flag or unflag) a tile
	void flag(int x, int y) {
		//same as uncover function pretty much
		int at_x = ceil(x / TILE_SIZE);
		int at_y = ceil(y / TILE_SIZE);

		//check if out of bounds
		if (at_x >= gameState::rows || at_y > gameState::cols || at_x < 0 || at_y < 0) {
			return;
		}

		//set isFlagged bool and change texture
		if (gameState::gboard->get(at_x, at_y).isFlagged) {
			gameState::gboard->getptr(at_x, at_y)->unflag();
			//increment mine counter
			gameState::gptr->toolbar->incMine();
		}
		else {
			if (gameState::gboard->getptr(at_x, at_y)->flag()) {
				//decrement mine counter
				gameState::gptr->toolbar->decMine();
			}
		}
	}

	//a debug function that clears the entire board
	void clearAll() {
		//for debug mode only clears the whole board
		for (int i = 0; i < gameState::rows; i++) {
			for (int k = 0; k < gameState::cols; k++) {
				uncover(i, k, true);
			}
		}
	}
}

