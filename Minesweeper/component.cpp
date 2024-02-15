/*==================================
* minesweeper++ - file component.cpp
* ==================================
* author: ben staehle
* date:	  11/16/22
* this program (and file) use the GNU GPL v3 License
* see COPYRIGHT.txt for license related information
* ==================================================*/
#include"component.h"
#include"engine.h"
#include"loader.h"
#include"resource.h"
#include"tile.h"
#include<cmath>
#include<iostream>
#include<cstdlib>
#include<sfml/Main.hpp>
#include<sfml/Graphics.hpp>

#define TILE_SIZE 30

namespace menu {
	//define all diffrerent compoments all in one file, sorry

	//number display struct
	Display::Display(float x, float y, int hundreds, int tens, int ones, comptype type, engine::GameEngine* ref)
		: Component(x, y, 60, 30, type, ref) {
		
		//set the starting sprites and textures
		this->hundreds.setTexture(*ref->texture);
		this->hundreds.setTextureRect(sf::IntRect(ldc[hundreds].first, ldc[hundreds].second + mine::Tile::tex_offset, 20, 30));
		this->hundreds.setPosition(x, y);

		this->tens.setTexture(*ref->texture);
		this->tens.setTextureRect(sf::IntRect(ldc[tens].first, ldc[tens].second + mine::Tile::tex_offset, 20, 30));
		this->tens.setPosition(x + 20, y);

		this->ones.setTexture(*ref->texture);
		this->ones.setTextureRect(sf::IntRect(ldc[ones].first, ldc[ones].second + mine::Tile::tex_offset, 20, 30));
		this->ones.setPosition(x + 40, y);

		//set value
		this->value = (hundreds * 100) + (tens * 10) + ones;
	}

	void Display::render(render_location r) {
		this->ref->window->draw(this->hundreds);
		this->ref->window->draw(this->tens);
		this->ref->window->draw(this->ones);
	}
	 
	void Display::inc() {
		if (value < 999) {
			this->value++;
			int valueHuns = (value / 100) % 10;
			int valueTens = (abs(value) / 10) % 10;
			int valueOnes = abs(value) % 10;

			if (this->value >= 0) {
				//set back to positive hundreds
				this->hundreds.setTextureRect(sf::IntRect(ldc[valueHuns].first, ldc[valueHuns].second + mine::Tile::tex_offset, 20, 30));
			}

			this->tens.setTextureRect(sf::IntRect(ldc[valueTens].first, ldc[valueTens].second + mine::Tile::tex_offset, 20, 30));
			this->ones.setTextureRect(sf::IntRect(ldc[valueOnes].first, ldc[valueOnes].second + mine::Tile::tex_offset, 20, 30));
		}
	}

	void Display::dec() {
		if (value > -99) {
			this->value--;
			int valueHuns = (value / 100) % 10;
			int valueTens = (abs(value) / 10) % 10;
			int valueOnes = abs(value) % 10;

			if (this->value > 0) {
				//hundreds set normally
				this->hundreds.setTextureRect(sf::IntRect(ldc[valueHuns].first, ldc[valueHuns].second + mine::Tile::tex_offset, 20, 30));
			}
			else if (this->value == -1) {
				//into the unknown (negetive numbers)
				//hundreds is set to the minus sign
				this->hundreds.setTextureRect(sf::IntRect(520, 60 + mine::Tile::tex_offset, 20, 30));
			}

			this->tens.setTextureRect(sf::IntRect(ldc[valueTens].first, ldc[valueTens].second + mine::Tile::tex_offset, 20, 30));
			this->ones.setTextureRect(sf::IntRect(ldc[valueOnes].first, ldc[valueOnes].second + mine::Tile::tex_offset, 20, 30));
		}
	}

	void Display::freeze() {
		this->frozen = true;
	}

	void Display::flipColor() {
		//funny stratagy
		inc();
		dec();
	}

    //button struct 
	Button::Button(float x, float y, float xSize, float ySize, comptype type, render_location r, engine::GameEngine* ref)
		: Component(x, y, xSize, ySize, type, ref) {

		if (r == MAIN) {
			//by default the button gets a nothing texture
			this->sprite.setTexture(*ref->texture);
			this->sprite.setTextureRect(sf::IntRect(240, 0, TILE_SIZE, TILE_SIZE));
			this->sprite.setPosition(x, y);
		}
		else {
			//by default the button gets a nothing texture
			this->sprite.setTexture(*ref->sTexture);
			this->sprite.setTextureRect(sf::IntRect(240, 0, TILE_SIZE, TILE_SIZE));
			this->sprite.setPosition(x, y);
		}
	}

	bool Button::isMouseOver(float x, float y) {
		float right_edge = xPos + xSize;
		float bottom_edge = yPos + ySize;
		if (x > xPos && x < right_edge && y > yPos && y < bottom_edge) {
			return true;
		}
		return false;
	}

	void Button::render(render_location r) {
		if (r == MAIN) {
			this->ref->window->draw(this->sprite);
		}
		else {
			//show the hover effect if it has one
			if (this->hoverable && this->isMouseOver(sf::Mouse::getPosition(*ref->settings.get()).x, sf::Mouse::getPosition(*ref->settings.get()).y)) {
				this->ref->settings->draw(this->hoverSprite);
			}
			else {
				this->ref->settings->draw(this->sprite);
			}
		}
	}

	void Button::setTexture(float textureX, float textureY, float sizeX, float sizeY) {
		this->sprite.setTextureRect(sf::IntRect(textureX, textureY, sizeX, sizeY));
		if (this->firstClick) {
			this->tbase_x = textureX;
			this->tbase_y = textureY;
			this->firstClick = false;
		}
	}

	void Button::setHoverSprite(float textureX, float textureY, float sizeX, float sizeY, bool scaleUp) {
		this->hoverSprite.setTexture(*ref->sTexture);
		this->hoverSprite.setTextureRect(sf::IntRect(textureX, textureY, sizeX, sizeY));
		this->hoverSprite.setPosition(this->xPos, this->yPos);
		if (scaleUp) {
			this->hoverSprite.setScale(1.5, 1.5);
		}
	}


	void Button::setPressFunc(std::function<void(void)> lambda) {
		this->pressFunc = lambda;
	}

	void Button::press() {
		this->pressFunc();
	}

	void Button::flipColor() {
		this->setTexture(tbase_x, tbase_y + mine::Tile::tex_offset, TILE_SIZE - 2, TILE_SIZE - 2);
	}


	//text box struct
	TextBox::TextBox(float x, float y, comptype type, engine::GameEngine* ref, float xSize, float ySize)
		: Component(x, y, xSize, ySize, type, ref) {
		//set up both sprites
		this->sprite.setPosition(x, y);
		this->sprite.setTexture(*ref->sTexture);

		this->deselected.setPosition(x, y);
		this->deselected.setTexture(*ref->sTexture);

		this->typing.setPosition(x, y);
		this->typing.setTexture(*ref->sTexture);

		//set up the text
		//load the font
		util::Resource r(IDR_TTF1, TTF);
		void* mem = r.GetResourceNoType();
		std::size_t memSize = r.getResourceSize();
		this->font.loadFromMemory(mem, memSize);

		this->text.setFont(this->font);
		this->text.setCharacterSize(this->fontSize);
		this->text.setFillColor(sf::Color::Black);
		this->text.setPosition(x + 34, y + 2);
	}

	void TextBox::render(render_location r) {
		//for out purproses - only render to settings
		if (r == SETTINGS) {
			if (this->isLocked) {
				if (this->isSelected) {
					ref->settings->draw(this->typing);
				}
				else {
					ref->settings->draw(this->sprite);
				}
				ref->settings->draw(this->text);
			}
			else {
				ref->settings->draw(this->deselected);
			}

		}
	}

	bool TextBox::isMouseOver(float x, float y) {
		float right_edge = xPos + xSize;
		float bottom_edge = yPos + ySize;
		if (x > xPos && x < right_edge && y > yPos && y < bottom_edge) {
			return true;
		}
		return false;
	}

	void TextBox::setTextureSel(float textureX, float textureY, float sizeX, float sizeY) {
		this->sprite.setTextureRect(sf::IntRect(textureX, textureY, sizeX, sizeY));
	}

	void TextBox::setTextureDes(float textureX, float textureY, float sizeX, float sizeY) {
		this->deselected.setTextureRect(sf::IntRect(textureX, textureY, sizeX, sizeY));
	}

	void TextBox::setTextureTyp(float textureX, float textureY, float sizeX, float sizeY) {
		this->typing.setTextureRect(sf::IntRect(textureX, textureY, sizeX, sizeY));
	}

	void TextBox::insertChar(sf::Keyboard::Key& k) {
		//check if the key is a number
		switch (k) {
		case sf::Keyboard::Num0:
			this->value += "0";
			break;
		case sf::Keyboard::Num1:
			this->value += "1";
			break;
		case sf::Keyboard::Num2:
			this->value += "2";
			break;
		case sf::Keyboard::Num3:
			this->value += "3";
			break;
		case sf::Keyboard::Num4:
			this->value += "4";
			break;
		case sf::Keyboard::Num5:
			this->value += "5";
			break;
		case sf::Keyboard::Num6:
			this->value += "6";
			break;
		case sf::Keyboard::Num7:
			this->value += "7";
			break;
		case sf::Keyboard::Num8:
			this->value += "8";
			break;
		case sf::Keyboard::Num9:
			this->value += "9";
			break;
		case sf::Keyboard::BackSpace:
			if (!this->value.empty()) {
				this->value.pop_back();
			}
			break;
		default:
			return;
		}

		//set the text and check for max
		if (!this->value.empty() && std::stoi(this->value.c_str()) > this->maxVal) {
			this->value = std::to_string(this->maxVal);
		}

		this->text.setString(this->value);
	}

	int TextBox::valueToInt() {
		if (this->value.empty()) {
			return 0;
		}
		else {
			return std::stoi(this->value);
		}
	}

	void TextBox::flipColor() {

	}

	//text struct
	Text::Text(float x, float y, std::string text, std::string fontName, int fontSize, comptype type, engine::GameEngine* ref, float xSize, float ySize)
		: Component(x, y, xSize, ySize, type, ref) {
		this->value = text;
		this->fontSize = fontSize;

		//load the font
		util::Resource r(IDR_TTF1, TTF);
		void* mem = r.GetResourceNoType();
		std::size_t memSize = r.getResourceSize();
		this->font.loadFromMemory(mem, memSize);

		this->text.setFont(this->font);
		this->text.setCharacterSize(this->fontSize);
		this->setColor(sf::Color::Black);

		this->text.setString(this->value.c_str());
		this->text.setPosition(x, y);
	}

	void Text::render(render_location r) {
		this->ref->settings->draw(this->text);
	}

	void Text::setText(std::string text) {
		this->text.setString(text.c_str());
	}

	void Text::setSytle(sf::Text::Style style) {
		this->style = style;
		this->text.setStyle(style);
	}

	void Text::setColor(sf::Color color) {
		this->color = color;
		this->text.setFillColor(color);
	}

	void Text::flipColor() {

	}
}