/*================================
* minesweeper++ - file component.h
* ================================
* author: ben staehle
* date:	  10/30/22
* this program (and file) use the GNU GPL v3 License
* see COPYRIGHT.txt for license related information
* ==================================================*/
#pragma once
#include<sfml/Graphics.hpp>
#include<sfml/System.hpp>
#include<sfml/Window.hpp>
#include<functional>

//forward dec
namespace engine {
	class GameEngine;
}

namespace menu {
	enum comptype { BUTTON, DISPLAY, TEXT, TEXTBOX};
	enum render_location {MAIN, SETTINGS};

	//number texture coords
	static inline std::pair<float, float> ldc[10] = {
		{478.0f, 0.0f},
		{478.0f, 30.0f},
		{478.0f, 60.0f},
		{478.0f, 90.0f},
		{498.0f, 0.0f},
		{498.0f, 30.0f},
		{498.0f, 60.0f},
		{498.0f, 90.0f},
		{518.0f, 0.0f},
		{518.0f, 30.0f},
	};

	struct Component {
		float xPos;
		float yPos;
		float xSize;
		float ySize;
		comptype type;
		engine::GameEngine* ref;

		Component(float x, float y, float xSize, float ySize, comptype type, engine::GameEngine* ref) {
			this->xPos = x;
			this->yPos = y;
			this->xSize = xSize;
			this->ySize = ySize;
			this->type = type;
			this->ref = ref;
		}

		virtual void render(render_location r) = 0;
		virtual void flipColor() = 0;
	};

	struct Display : public Component {
		
		sf::Sprite hundreds;
		sf::Texture huns_tex;

		sf::Sprite tens;
		sf::Texture tens_tex;

		sf::Sprite ones;
		sf::Texture ones_tex;

		int value; //max of 999
		bool frozen = false; //in time

		Display(float x, float y, int hundreds, int tens, int ones, comptype type, engine::GameEngine* ref);
		void inc(); //increments the display by one
		void dec(); //decrements the display by one
		void freeze(); //freezes the counter
		void render(render_location r) override;
		void flipColor() override;

		bool isZero() {
			//does the display read zero
			return this->value == 0;
		}

		bool isFull() {
			//does the display read 999
			return this->value == 999;
		}
	};

	struct Button : public Component {
		sf::Sprite sprite;
		sf::Texture texture;
		std::function<void(void)> pressFunc;
		std::function<void(void)> hoverFunc;

		sf::Sprite hoverSprite;

		float tbase_x;
		float tbase_y;
		bool firstClick = true;
		bool hoverable = false;

		Button(float x, float y, float xSize, float ySize, comptype type, render_location r, engine::GameEngine* ref);
		void setTexture(float textureX, float textureY, float sizeX, float sizeY);
		void setPressFunc(std::function<void(void)> lambda);
		void setHoverSprite(float textureX, float textureY, float sizeX, float sizeY, bool scaleUp);
		void press();
		bool isMouseOver(float x, float y);
		void render(render_location r) override;
		void flipColor() override;
	};

	struct TextBox : public Component {
		sf::Sprite sprite;
		sf::Sprite deselected;
		sf::Sprite typing;
		sf::Text text;
		sf::Font font;

		int maxVal; 
		int fontSize = 20;
		bool isLocked = false;
		bool isSelected = false;
		std::string value;

		TextBox(float x, float y, comptype type, engine::GameEngine* ref, float xSize = 0, float ySize = 0);
		int valueToInt(); //returns the texbox's value as and int
		void render(render_location r) override;
		void insertChar(sf::Keyboard::Key& k);
		void setTextureSel(float textureX, float textureY, float sizeX, float sizeY);
		void setTextureDes(float textureX, float textureY, float sizeX, float sizeY);
		void setTextureTyp(float textureX, float textureY, float sizeX, float sizeY);
		bool isMouseOver(float x, float y);
		void flipColor() override;
	};
	
	struct Text : public Component {
		sf::Text text;
		sf::Font font;
		sf::Text::Style style;
		sf::Color color;
		int fontSize;
		std::string value;

		Text(float x, float y, std::string text, std::string fontName, int fontSize, comptype type, engine::GameEngine* ref, float xSize = 0, float ySize = 0);
		void setSytle(sf::Text::Style style);
		void setText(std::string text);
		void setColor(sf::Color color);
		void render(render_location r) override;
		void flipColor() override;
	};
}