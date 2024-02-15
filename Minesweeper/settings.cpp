/*===============================
* minesweeper++ - file settings.cpp
* ===============================
* author: ben staehle
* date:	  10/30/22
* this program (and file) use the GNU GPL v3 License
* see COPYRIGHT.txt for license related information
* ==================================================*/
#include<iostream>
#include<cstdlib>
#include<string>
#include<sfml\OpenGL.hpp>
#include"settings.h"
#include"engine.h"
#include"component.h"
#include"mine.h"

#define TILE_SIZE 30

namespace menu {

	Settings::Settings(engine::GameEngine* ref) {
		//selected difficulty starts at beginner
		this->diff = mine::BEGINNER;

		//"new game" button
		this->customGame = std::make_shared<Button>(0, 0, 187, 45, BUTTON, SETTINGS, ref);
		this->customGame->setTexture(0, 60, 125, 30);
		//set it's press function
		this->customGame->setPressFunc([&, ref](void) -> void {
			switchTabs(NEW_GAME);
		});
		this->customGame->sprite.setScale(1.5, 1.5);
		addComponent(this->customGame.get());
			
		//controls button
		this->controls = std::make_shared<Button>(0, 45, 187, 45, BUTTON, SETTINGS, ref);
		this->controls->setTexture(0, 120, 125, 30);
		//set it's press function
		this->controls->setPressFunc([&, ref](void) -> void {
			switchTabs(CONTROLS);
		});
		this->controls->sprite.setScale(1.5, 1.5);
		addComponent(this->controls.get());

		//about button
		this->about = std::make_shared<Button>(0, 90, 187, 45, BUTTON, SETTINGS, ref);
		this->about->setTexture(0, 90, 125, 30);
		//set it's press function
		this->about->setPressFunc([&, ref](void) -> void {
			switchTabs(ABOUT);
		});
		this->about->sprite.setScale(1.5, 1.5);
		addComponent(this->about.get());

		//exit button
		this->exit = std::make_shared<Button>(0, 455, 187, 45, BUTTON, SETTINGS, ref);
		this->exit->setTexture(0, 150, 125, 30);
		//set it's press function
		this->exit->setPressFunc([&, ref](void) -> void {
			//close the program
			std::exit(EXIT_SUCCESS);
		});
		this->exit->sprite.setScale(1.5, 1.5);
		addComponent(this->exit.get());

		//difficulty text
		this->difficulty = std::make_shared<Text>(225, 10, "Difficulty Level", "dos_font_win32.ttf", 18, TEXT, ref);
		this->difficulty->setColor(sf::Color(64, 64, 64));
		this->new_game_tab.push_back(this->difficulty.get());

		//seperator
		this->sep1 = std::make_shared<Text>(225, 25, "-------------------------", "dos_font_win32.ttf", 18, TEXT, ref);
		this->sep1->setColor(sf::Color(64, 64, 64));
		this->new_game_tab.push_back(this->sep1.get());

		//difficulty + custom level select buttons
		this->easy = std::make_shared<Button>(250, 50, 154, 30, BUTTON, SETTINGS, ref);
		this->easy->setTexture(0, 180, 154, 30);
		//set it's press function
		this->easy->setPressFunc([&, ref](void) -> void {
			switchDifficulties(mine::BEGINNER);
		});
		this->new_game_tab.push_back(this->easy.get());

		this->normal = std::make_shared<Button>(250, 85, 154, 30, BUTTON, SETTINGS, ref);
		this->normal->setTexture(0, 210, 154, 30);
		//set it's press function
		this->normal->setPressFunc([&, ref](void) -> void {
			switchDifficulties(mine::INTERMEDIATE);
		});
		this->new_game_tab.push_back(this->normal.get());

		this->hard = std::make_shared<Button>(250, 120, 154, 30, BUTTON, SETTINGS, ref);
		this->hard->setTexture(0, 240, 154, 30);
		//set it's press function
		this->hard->setPressFunc([&, ref](void) -> void {
			switchDifficulties(mine::ADVANCED);
		});
		this->new_game_tab.push_back(this->hard.get());

		//seperator 2
		this->sep2 = std::make_shared<Text>(225, 150, "-------------------------", "dos_font_win32.ttf", 18, TEXT, ref);
		this->sep2->setColor(sf::Color(64, 64, 64));
		this->new_game_tab.push_back(this->sep2.get());

		this->custom = std::make_shared<Button>(250, 180, 154, 30, BUTTON, SETTINGS, ref);
		this->custom->setTexture(0, 270, 154, 30);
		//set it's press function
		this->custom->setPressFunc([&, ref](void) -> void {
			switchDifficulties(mine::CUSTOM);
		});
		this->new_game_tab.push_back(this->custom.get());

	    //cancel and apply buttons
		this->close = std::make_shared<Button>(375, 430, 112, 45, BUTTON, SETTINGS, ref);
		this->close->setTexture(0, 0, 75, 30);
		//set it's press function
		this->close->setPressFunc([&, ref](void) -> void {
			//close the window
			ref->settings->close();
		});
		this->close->hoverable = true;
		this->close->setHoverSprite(0, 30, 75, 30, true);
		this->close->sprite.setScale(1.5, 1.5);
		this->new_game_tab.push_back(this->close.get());

		this->startGame = std::make_shared<Button>(250, 430, 112, 45, BUTTON, SETTINGS, ref);
		this->startGame->setTexture(75, 0, 75, 30);
		//set it's press function
		this->startGame->setPressFunc([&, ref](void) -> void {
			//call apply 
			applyChanges(*ref);
		});
		this->startGame->hoverable = true;
		this->startGame->setHoverSprite(75, 30, 75, 30, true);
	    this->startGame->sprite.setScale(1.5, 1.5);
		this->new_game_tab.push_back(this->startGame.get());

		//custom game textboxes
		this->sizeX = std::make_shared<TextBox>(250, 225, TEXTBOX, ref, 86, 30);
		this->sizeX->setTextureSel(154, 300, 86, 30);
		this->sizeX->setTextureDes(240, 300, 86, 30);
		this->sizeX->setTextureTyp(326, 300, 86, 30);
		this->sizeX->maxVal = 200;
		this->new_game_tab.push_back(this->sizeX.get());

		this->sizeY = std::make_shared<TextBox>(350, 225, TEXTBOX, ref, 86, 30);
		this->sizeY->setTextureSel(154, 330, 86, 30);
		this->sizeY->setTextureDes(240, 330, 86, 30);
		this->sizeY->setTextureTyp(326, 330, 86, 30);
		this->sizeY->maxVal = 200;
		this->new_game_tab.push_back(this->sizeY.get());

		this->mineCount = std::make_shared<TextBox>(300, 260, TEXTBOX, ref, 86, 30);
		this->mineCount->setTextureSel(154, 360, 86, 30);
		this->mineCount->setTextureDes(240, 360, 86, 30);
		this->mineCount->setTextureTyp(326, 360, 86, 30);
		this->mineCount->maxVal = 150;
		this->new_game_tab.push_back(this->mineCount.get());

		//seperator 3
		this->sep3 = std::make_shared<Text>(225, 325, "-------------------------", "dos_font_win32.ttf", 18, TEXT, ref);
		this->sep3->setColor(sf::Color(64, 64, 64));
		this->new_game_tab.push_back(this->sep3.get());

		//create keybind text screen
		this->keybindText = std::make_shared<Text>(200, 20, "Keybinds\n===================\nColor Theme.......C\nReset Game........F2\nToggle 5x5 Mode...M\nOpen Settings.....S", "dos_font_win32.ttf", 24, TEXT, ref);
		this->keybindText->setColor(sf::Color(64, 64, 64));
		this->controls_tab.push_back(this->keybindText.get());

		//about screen text
		this->aboutText1 = std::make_shared<Text>(200, 20, "Minesweeper\n===================\n\n\n\n===================\n\n\n\n\n\n===================", "dos_font_win32.ttf", 24, TEXT, ref);
		this->aboutText1->setColor(sf::Color(64, 64, 64));
		this->about_tab.push_back(this->aboutText1.get());

		this->plusplus = std::make_shared<Text>(353, 20, "++", "dos_font_win32.ttf", 24, TEXT, ref);
		this->plusplus->setColor(sf::Color(255, 32, 3));
		this->about_tab.push_back(this->plusplus.get());

		this->aboutText2 = std::make_shared<Text>(200, 50, "\nProgrammed by Ben Staehle\nGraphics and Art by Xererain\nWebpage and Source Code:\nhttps://github.com/benito2268\n", "dos_font_win32.ttf", 16, TEXT, ref);
		this->aboutText2->setColor(sf::Color(64, 64, 64));
		this->about_tab.push_back(this->aboutText2.get());

		this->aboutText3 = std::make_shared<Text>(200, 165, "Basically a silly modern C++ \nspinoff of the classic\n\"WinMine\" with a few extra\nfeatures :)", "dos_font_win32.ttf", 16, TEXT, ref);
		this->aboutText3->setColor(sf::Color(64, 64, 64));
		this->about_tab.push_back(this->aboutText3.get());

		const GLubyte* e = glGetString(GL_VERSION);

		this->aboutText4 = std::make_shared<Text>(200, 315, "Compiler: MSVC 19.29\nMode: release64\nRenderer:\n    OpenGL \n\n\ncpu_usage(%)\nmem_usage(mb)", "dos_font_win32.ttf", 16, TEXT, ref);
		this->aboutText4->setColor(sf::Color(64, 64, 64));
		this->about_tab.push_back(this->aboutText4.get());

		this->rendererText = std::make_shared<Text>(295, 363, reinterpret_cast<const char*>(e), "dos_font_win32.ttf", 16, TEXT, ref);
		this->rendererText->setColor(sf::Color(64, 64, 64));
		this->about_tab.push_back(this->rendererText.get());

		this->cpuUse = std::make_shared<Text>(335, 410, "0.00", "dos_font_win32.ttf", 16, TEXT, ref);
		this->cpuUse->setColor(sf::Color(66, 78, 245));
		this->about_tab.push_back(this->cpuUse.get());

		this->memUse = std::make_shared<Text>(335, 430, "0.00", "dos_font_win32.ttf", 16, TEXT, ref);
		this->memUse->setColor(sf::Color(66, 78, 245));
		this->about_tab.push_back(this->memUse.get());

		//start on the new game tab
		switchTabs(NEW_GAME);
	}

	void Settings::render() {
		for (auto& element : this->components_tab) {
			element->render(SETTINGS);
		}

		if (this->curr == NEW_GAME) {
			for (auto& element : this->new_game_tab) {
				element->render(SETTINGS);
			}
		}
		else if (this->curr == ABOUT) {
			for (auto& element : this->about_tab) {
				element->render(SETTINGS);
			}
		}
		else if (this->curr == CONTROLS) {
			for (auto& element : this->controls_tab) {
				element->render(SETTINGS);
			}
		}
	}

	void Settings::switchTabs(Tab newTab) {
		switch(newTab) {
		case NEW_GAME:
			//set new game selected
			this->curr = NEW_GAME;

			//change the textures
			this->controls->setTexture(0, 120, 125, 30);
			this->about->setTexture(0, 90, 125, 30);

			this->customGame->setTexture(125, 60, 125, 30);

			break;
		case ABOUT:
			this->curr = ABOUT;

			//change the textures
			this->controls->setTexture(0, 120, 125, 30);
			this->customGame->setTexture(0, 60, 125, 30);

			this->about->setTexture(125, 90, 125, 30);

			break;
		case CONTROLS:
			this->curr = CONTROLS;

			//change the textures
			this->about->setTexture(0, 90, 125, 30);
			this->customGame->setTexture(0, 60, 125, 30);

			this->controls->setTexture(125, 120, 125, 30);

			break;
		default:
			break;
		}
	}

	void Settings::switchDifficulties(mine::difficulty diff) {
		switch (diff) {
		case mine::BEGINNER:
			this->diff = mine::BEGINNER;

			//change the textures
			this->normal->setTexture(0, 210, 154, 30);
			this->hard->setTexture(0, 240, 154, 30);
			this->custom->setTexture(0, 270, 154, 30);

			this->easy->setTexture(154, 180, 154, 30);

			//set the text boxes
			this->sizeX->isLocked = false;
			this->sizeY->isLocked = false;
			this->mineCount->isLocked = false;
			break;
		case mine::INTERMEDIATE:
			this->diff = mine::INTERMEDIATE;

			//change the textures
			this->easy->setTexture(0, 180, 154, 30);
			this->hard->setTexture(0, 240, 154, 30);
			this->custom->setTexture(0, 270, 154, 30);

			this->normal->setTexture(154, 210, 154, 30);
			//set the text boxes
			this->sizeX->isLocked = false;
			this->sizeY->isLocked = false;
			this->mineCount->isLocked = false;

			break;
		case mine::ADVANCED:
			this->diff = mine::ADVANCED;

			//change the textures
			this->normal->setTexture(0, 210, 154, 30);
			this->easy->setTexture(0, 180, 154, 30);
			this->custom->setTexture(0, 270, 154, 30);

			this->hard->setTexture(154, 240, 154, 30);

			//set the text boxes
			this->sizeX->isLocked = false;
			this->sizeY->isLocked = false;
			this->mineCount->isLocked = false;

			break;
		case mine::CUSTOM:
			this->diff = mine::CUSTOM;

			//change the textures
			this->normal->setTexture(0, 210, 154, 30);
			this->hard->setTexture(0, 240, 154, 30);
			this->easy->setTexture(0, 180, 154, 30);

			this->custom->setTexture(154, 270, 154, 30);

			//set the text boxes
			this->sizeX->isLocked = true;
			this->sizeY->isLocked = true;
			this->mineCount->isLocked = true;

			break;
		default:
			break;
		}
	}

	void Settings::applyChanges(engine::GameEngine& ref) {
		using g = mine::gameState;
		mine::style temp = g::color;
		if (this->diff == mine::CUSTOM) {
			mine::init(this->diff, g::gmode, &ref, this->mineCount->valueToInt(), this->sizeX->valueToInt(), this->sizeY->valueToInt());
		}
		else {
			mine::init(this->diff, g::gmode, &ref);
		}

		mine::setColorImm(temp);
		ref.started = false;
		ref.resetToolbar();
	}

	void Settings::addComponent(Component* c) {
		this->components_tab.push_back(c);
	}
}