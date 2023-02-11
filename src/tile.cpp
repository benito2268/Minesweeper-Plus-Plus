/*===============================
* minesweeper++ - file tile.cpp
* ===============================
* author: ben staehle
* date:	  10/30/22
* this program (and file) use the GNU GPL v3 License
* see COPYRIGHT.txt for license related information
* ==================================================*/
#include"tile.h"
#include"engine.h"
#include"mine.h"
#include<iostream>

namespace mine {

	Tile::Tile(float x, float y, int value, bool isMine, engine::GameEngine* ref) {
		this->x = x;
		this->y = y;
		this->value = value;
		this->isMine = isMine;
		this->isFlagged = false;

		//tex_x and tex_y always begin at the coordinate of the uncovered tile texture corresponding to the light theme
		
		this->tex_x = 8 * TILE_SIZE; //8th tile from the left
		this->tex_y = 0;			 //no need to multiply here, since 0 * TILE_SIZE = 0 (duh)
		this->tex_offset = 0;        //tex_offset is a y offest that is only non-zero when dark mode is active
		
		//set up the texture and sprite
		this->sprite = std::make_shared<sf::Sprite>();

		sprite->setTexture(*ref->texture);
		sprite->setTextureRect(sf::IntRect(tex_x, tex_y, TILE_SIZE, TILE_SIZE));
		sprite->setPosition(x, y);
	}

	void Tile::uncover() {
		//we need to calculate the new tile texture based on the value of the tile and current color offset
		//everything always gets multiplied by the offset incase it's not 0
		if (this->value == -1) {
			//no need to change x, mine in same col as uncovered
			this->tex_y = (3 * TILE_SIZE);
			sprite->setTextureRect(sf::IntRect(this->tex_x, this->tex_y + this->tex_offset, TILE_SIZE, TILE_SIZE));
		}
		else if (this->value == 0) {
			//set the appropriate texture for a zero tile
			this->tex_x = 0; 
			this->tex_y = TILE_SIZE;
			sprite->setTextureRect(sf::IntRect(this->tex_x, this->tex_y + this->tex_offset, TILE_SIZE, TILE_SIZE));

		}
		else if (this->value < 9) {
			//any other numbered 3x3 mode tile
			this->tex_x = (this->value - 1) * TILE_SIZE;
			sprite->setTextureRect(sf::IntRect(this->tex_x, this->tex_y + this->tex_offset, TILE_SIZE, TILE_SIZE));

		}
		else {
			//assume 5x5 mode tile
			//unused currently
			if (this->value >= 9 && this->value <= 16) {
				//in the first row
				this->tex_y = 2 * TILE_SIZE;
				this->tex_x = (this->value - 9) * TILE_SIZE;
				sprite->setTextureRect(sf::IntRect(this->tex_x, this->tex_y + this->tex_offset, TILE_SIZE, TILE_SIZE));
			}
			else if (this->value > 16) {
				//in the second row
				this->tex_y = 3 * TILE_SIZE;
				this->tex_x = (this->value - 17) * TILE_SIZE;
				sprite->setTextureRect(sf::IntRect(this->tex_x, this->tex_y + this->tex_offset, TILE_SIZE, TILE_SIZE));
			}
		}
	}

	void Tile::flipColor() {
		//just reset the texture
		if (!this->isLosingTile) {
			sprite->setTextureRect(sf::IntRect(this->tex_x, this->tex_y + this->tex_offset, TILE_SIZE, TILE_SIZE));
		}
	}

	bool Tile::flag() {
		if (!this->isRevealed) {
			this->isFlagged = true;
			//set tex_x and tex_y to the flag texture
			this->tex_x = 10 * TILE_SIZE;
			this->tex_y = (3 * TILE_SIZE);

			//reset the texture
			this->sprite->setTextureRect(sf::IntRect(this->tex_x, this->tex_y + this->tex_offset, TILE_SIZE, TILE_SIZE));
			return true;
		}
		return false;
	}

	void Tile::unflag() {
		if (!this->isRevealed) {
			this->isFlagged = false;
			//set tex_x and tex_y to the uncovered texture
			this->tex_x = 8 * TILE_SIZE;
			this->tex_y = 0;
			//reset the texture
			this->sprite->setTextureRect(sf::IntRect(this->tex_x, this->tex_y + this->tex_offset, TILE_SIZE, TILE_SIZE));
		}
	}

	void Tile::setFalseFlag() {
		//set tex_x and tex_y to the false flag texture
		this->tex_x = 11 * TILE_SIZE;
		this->tex_y = (3 * TILE_SIZE);

		//reset the texture
		this->sprite->setTextureRect(sf::IntRect(this->tex_x, this->tex_y + this->tex_offset, TILE_SIZE, TILE_SIZE));
	}

	void Tile::setRevealed() {
		this->isRevealed = true;
	}
}