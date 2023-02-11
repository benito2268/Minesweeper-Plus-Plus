/*===============================
* minesweeper++ - file tile.h
* ===============================
* author: ben staehle
* date:	  10/30/22
* this program (and file) use the GNU GPL v3 License
* see COPYRIGHT.txt for license related information
* ==================================================*/
#pragma once
#include<sfml/Graphics.hpp>
#include<string>
#include<memory>

#define TILE_SIZE 30

typedef unsigned int uint32;


namespace engine {	//forward def
	class GameEngine;
}

namespace mine {
	class Tile {
	public:
		float x;
		float y;
		float tex_x;
		float tex_y;
		static inline float tex_offset;
		bool isMine;
		bool isRevealed = false; //by default
		bool isFlagged; //by default
		bool isLosingTile = false; //by default
		int value;   // mines have a value of -1
		int value5x5; //stored value in 5x5 mode
		std::shared_ptr<sf::Sprite> sprite;

		Tile(float x, float y, int value, bool isMine, engine::GameEngine* ref);
		Tile() = default;
		void uncover();
		bool flag();
		void unflag();
		void flipColor();
		void setFalseFlag();
		void setRevealed();

		friend class GameEngine;
	};
}