/*===============================
* minesweeper++ - file mine.h
* ===============================
* author: ben staehle
* date:	  10/30/22
* this program (and file) use the GNU GPL v3 License
* see COPYRIGHT.txt for license related information
* ==================================================*/
#pragma once
#include<memory>
#include"mtable.h"
#include"tile.h"

#define TILE_SIZE 30

//forward decl
namespace engine {
	class GameEngine;
}

namespace mine {
	enum state {MENU, PLAYING, WIN, LOSE};
	enum gameMode {THREE, FIVE, DEATH};
	enum difficulty {BEGINNER, INTERMEDIATE, ADVANCED, CUSTOM};
	enum style {LIGHT, DARK};

	struct gameState {
		//state variables
		static inline int numMines;
		static inline state gstate;
		static inline gameMode gmode;
		static inline style color;
		static inline int rows;
		static inline int cols;
		static inline difficulty currDiff;
		static inline int numCovered = 0;
		static inline int boss_offset_x = 0;
		static inline int boss_offset_y = 0;
		static inline engine::GameEngine* gptr;
		static inline std::vector<std::pair<int, int>> currMines;
		static inline bool isFirstPass = true;
		
		//game variables
		static inline std::shared_ptr<util::Table_2d<Tile>> gboard;
	};

	void gen(engine::GameEngine* ref);
	void gen3x3(std::vector<std::pair<int, int>>& mines, engine::GameEngine* ref);
	void gen5x5(std::vector<std::pair<int, int>>& mines, engine::GameEngine* ref);
	void toggle5x5();
	void init(difficulty dif, gameMode gm, engine::GameEngine* ref, int numMines = 0, int rows = 0, int cols = 0, float sx = 0, float sy = 0);
	void setColor(style color);
	void setColorImm(style color);
	void uncover(int x, int y, bool isToggle);
	void uncoverImm(int at_x, int at_y, bool isToggle);
	void flag(int x, int y);
	void revealShort3x3(int x, int y);
	void revealShort5x5(int x, int y);
	bool checkWinCond();
	int countAdjacentMines(int x, int y);
	int countAdjacentMines5x5(int x, int y);
	void depthFirstReveal(int x, int y, bool isToggle);
	void loseGame(int x, int y);
	void winGame();
	void toggleColor();
	void clearAll();
}