/*===============================
* minesweeper++ - file toolbar.h
* ===============================
* author: ben staehle
* date:	  11/16/22
* this program (and file) use the GNU GPL v3 License
* see COPYRIGHT.txt for license related information
* ==================================================*/
#pragma once
#include<sfml/Graphics.hpp>
#include<sfml/System.hpp>
#include<sfml/Window.hpp>
#include<vector>
#include<memory>

namespace engine { class GameEngine; }

namespace menu {
	struct Component;

	class Toolbar {
	private:
		//the texture and sprite of the toolbar itself
		sf::Sprite sprite;
		sf::Texture texture;

	public:
		std::vector<Component*> components;

		//individual gui components
		std::shared_ptr<Display> mineCount;
		std::shared_ptr<Display> clock; 
		std::shared_ptr<Button> smileyButton;
		std::shared_ptr<Button> settingsButton;
		std::shared_ptr<Button> colorButton;
	
		Toolbar(int numMines, engine::GameEngine* ref);
		void addComponent(Component* c);
		void render();
		void incClock();
		void decClock();
		void incMine();
		void decMine();
		void flipColor();
	};
}

