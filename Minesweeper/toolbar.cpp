/*================================
* minesweeper++ - file toolbar.cpp
* ================================
* author: ben staehle
* date:	  11/16/22
* this program (and file) use the GNU GPL v3 License
* see COPYRIGHT.txt for license related information
* ==================================================*/
#include<iostream>
#include"component.h"
#include"toolbar.h"
#include"mine.h"
#include"engine.h"

#define TILE_SIZE 30
#define SCALED_TILE_SIZE 60 //a tile with a scale of 2.0

namespace menu {

	Toolbar::Toolbar(int numMines, engine::GameEngine* ref) {
		//add the mines counter
		this->mineCount = std::make_shared<Display>(20  + mine::gameState::boss_offset_x, (MENU_HEIGHT / 2) - 15, (numMines / 100) % 10, (numMines / 10) % 10, numMines % 10, DISPLAY, ref);
		addComponent(this->mineCount.get());

		this->clock = std::make_shared<Display>(((engine::GameEngine::width * TILE_SIZE) - 80) + mine::gameState::boss_offset_x, (MENU_HEIGHT / 2) - 15, 0, 0, 0, DISPLAY, ref);
		addComponent(this->clock.get());

		//add the smiley button to the topcenter of the screen
		this->smileyButton = std::make_shared<Button>((((engine::GameEngine::width * TILE_SIZE) / 2) - 30) + mine::gameState::boss_offset_x, (MENU_HEIGHT / 2) - 30, SCALED_TILE_SIZE, SCALED_TILE_SIZE, BUTTON, MAIN ,ref);
		//set it's texture
		this->smileyButton->setTexture(240, 30, TILE_SIZE, TILE_SIZE);
		//set it's press function
		this->smileyButton->setPressFunc([&, ref](void) -> void {
			ref->resetGame();
		});
		this->smileyButton->sprite.setScale(2.0, 2.0);
		addComponent(this->smileyButton.get());

		//add the settings button
		//if size is too small you have to use the key shortcuts
		if (mine::gameState::cols >= 13) {
			this->settingsButton = std::make_shared<Button>((this->smileyButton->xPos - 70), this->smileyButton->yPos, SCALED_TILE_SIZE, SCALED_TILE_SIZE, BUTTON, MAIN, ref);
			this->settingsButton->setTexture(330, 0, TILE_SIZE - 2, TILE_SIZE - 2);
			this->settingsButton->setPressFunc([&, ref](void) -> void {
				ref->openSettings();
				});
			this->settingsButton->sprite.setScale(2.0, 2.0);
			addComponent(this->settingsButton.get());

			//add color button
			this->colorButton = std::make_shared<Button>((this->smileyButton->xPos + 70), this->smileyButton->yPos, SCALED_TILE_SIZE, SCALED_TILE_SIZE, BUTTON, MAIN, ref);
			this->colorButton->setTexture(270, 0, TILE_SIZE - 2, TILE_SIZE - 2);
			this->colorButton->setPressFunc([&, ref](void) -> void {
				mine::toggleColor();
				});
			this->colorButton->sprite.setScale(2.0, 2.0);
			addComponent(this->colorButton.get());
		}
		
		if (mine::gameState::color == mine::DARK) {
			flipColor();
		}
	}

	void Toolbar::addComponent(Component* c) {
		this->components.push_back(c);
	}

	void Toolbar::incClock() {
		this->clock->inc();
	}

	void Toolbar::decClock() {
		this->clock->dec();
	}

	void Toolbar::flipColor() {
		for (auto& element : this->components) {
			element->flipColor();
		}
	}

	void Toolbar::incMine() {
		this->mineCount->inc();
	}

	void Toolbar::decMine() {
		this->mineCount->dec();
	}

	void Toolbar::render() {
		for (auto& element : this->components) {
			element->render(MAIN);
		}
	}
}

