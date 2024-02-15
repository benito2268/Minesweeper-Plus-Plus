/*===============================
* minesweeper++ - file engine.cpp
* ===============================
* author: ben staehle
* date:	  10/30/22
* this program (and file) use the GNU GPL v3 License
* see COPYRIGHT.txt for license related information
* ==================================================*/
#include<iostream>
#include<fstream>
#include<string>
#include"boss_mode.h"
#include"proc.h"
#include"settings.h"
#include"engine.h"
#include"loader.h"
#include"resource.h"
#include"toolbar.h"
#include"component.h"
#include"timer.h"
#include"mine.h"
#include<windows.h>

namespace engine {

	GameEngine::GameEngine(uint32 rows, uint32 cols, std::string title, bool vsync) {
		this->timer = std::make_shared<util::Timer>();
		this->vsync = vsync;
		this->width = rows;
		this->height = cols;
		this->title = title;
	}

	void GameEngine::run() {
		try {
			init();
			gameLoop();
		}
		catch (std::exception e) {
			std::cout << e.what();
		}
		cleanup();
	}

	void GameEngine::init() {
		this->texture = std::make_shared<sf::Texture>();
		this->sTexture = std::make_shared<sf::Texture>();

		//generate the board
		this->window = std::make_shared<sf::RenderWindow>(sf::VideoMode((this->width * 30) - 2, (this->height * 30) - 2 + MENU_HEIGHT), this->title.c_str(), sf::Style::Titlebar | sf::Style::Close);
		this->view = this->window->getDefaultView();
		this->window->setVerticalSyncEnabled(false);
		this->window->setFramerateLimit(60);
		
		//set all the game properties
		mine::init(mine::CUSTOM, mine::THREE, this, 99, 24, 24);
		this->timer->init();

		//load the atlas as raw png data of type uint8_t*
		util::Resource r(IDB_PNG1, PNG);
		uint8_t* mem = r.GetResourceAsCharPtr();
		std::size_t memSize = r.getResourceSize();
		this->texture->loadFromMemory(mem, memSize, sf::IntRect(0, 0, 538, 240));

		//load the settings menu texture
		util::Resource r1(IDB_PNG2, PNG);
		uint8_t* mem1 = r1.GetResourceAsCharPtr();
		std::size_t memSize1 = r1.getResourceSize();
		this->sTexture->loadFromMemory(mem1, memSize1, sf::IntRect(0, 0, 500, 600));

		//set up the settings menu bar thing
		this->rect = std::make_shared<sf::RectangleShape>(sf::Vector2f(187.0f, 500.0f));
		this->rect->setFillColor(sf::Color(138, 138, 138));
		this->rect->setPosition(0, 0);

		//set the window icon
		util::Resource r2(IDB_PNG4, PNG);
		uint8_t* mem2 = r2.GetResourceAsCharPtr();
		std::size_t icoSize = r2.getResourceSize();
		auto icon = sf::Image{};
		icon.loadFromMemory(mem2, icoSize);
		this->window->setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());

		//init the cpu counter
		proc::init();

		started = false;

		//start the toolbar
		resetToolbar();

		//set up the settings window
		this->gset = std::make_shared<menu::Settings>(this);
	}

	void GameEngine::openSettings()  {
		//set up the settings window
		this->settings = std::make_shared<sf::RenderWindow>(sf::VideoMode(500.0f, 500.0f), "Minesweeper++ Settings");
		this->settings->setVerticalSyncEnabled(false);
		this->settings->setFramerateLimit(60);

		this->settingsOpen = true;
	}

	void GameEngine::closeSettings() {
		this->settings->close();
	}

	void GameEngine::resetToolbar() {
		//resets the clock and the mine count
		this->toolbar = std::make_shared<menu::Toolbar>(mine::gameState::numMines, this);
		this->ticks = 0;
	}

	void GameEngine::haltGame() {
		//called when the game is won. lost, or paused
		//for when the game need to pause but not exit
		//stop the timer
		this->toolbar->clock->freeze();
	}

	void GameEngine::gameLoop() {
		//timers for keeping track of where we are in each frame
		float elapsedTime;
		float accumulator = 0.0f;
		float interval = 1.0f / TARGET_UPS;

		bool running = true;
		while (running && this->window->isOpen()) {

			elapsedTime = this->timer->getElapsedTime();
			accumulator += elapsedTime;

			input(); //check for key and mouse input

			//basically make sure that update does not slow down as rendering does
			//if gpu drawing slows, allows the cpu to continue while waiting wait
			while (accumulator >= interval) {  
				update(interval);
				accumulator -= interval;
			}

			render(); //main render call - once per frame
		}
	}

	void GameEngine::update(float interval) {
		//check delta
		using namespace std::chrono;
		//tick the clock every second
		if (started) {
			if(this->ticks == 29 && !this->toolbar->clock->frozen) { //calibrated (tm)
				this->delta = high_resolution_clock::now();
				this->toolbar->incClock();
				this->ticks = 0;

				//also update cpu counter
				if(settingsOpen) {
					this->gset->cpuUse->setText(std::to_string(proc::getCurrentValue()).substr(0, 5));
					this->gset->memUse->setText(std::to_string(proc::getCurrentMemValue()).substr(0, 5));
				}
				else {
					this->ticks++;
				}
			}
		}

		ticks++;
	}

	//make into its own class imo
	void GameEngine::input() {
		//check if the window needs to close
		sf::Event winEvent;
		while (this->window->pollEvent(winEvent)) {
			//close window if prompted
			if (winEvent.type == sf::Event::Closed) {
				this->window->close();
			}
			else if (winEvent.type == sf::Event::MouseButtonPressed) {
				if (winEvent.mouseButton.button == sf::Mouse::Left) {
					//start the clock if this is the first click
					if (!started && winEvent.mouseButton.y > MENU_HEIGHT) {
						started = true;
					}

					//check for normal click
					if(winEvent.mouseButton.y > MENU_HEIGHT && mine::gameState::gstate == mine::PLAYING) {
						mine::uncover(winEvent.mouseButton.x, winEvent.mouseButton.y - MENU_HEIGHT, false);

					}
					else {
						//check which button it's over
						for (auto& element : this->toolbar->components) {
							if (element->type == menu::BUTTON) {
								menu::Button* ref = dynamic_cast<menu::Button*>(element);
								if (ref->isMouseOver(winEvent.mouseButton.x, winEvent.mouseButton.y)) {
									ref->press();
									break;
								}
							}
						}	
					}
				}
				else if (winEvent.mouseButton.button == sf::Mouse::Right && mine::gameState::gstate == mine::PLAYING) {
					if (winEvent.mouseButton.y > MENU_HEIGHT) {
						mine::flag(winEvent.mouseButton.x - mine::gameState::boss_offset_x, (winEvent.mouseButton.y - MENU_HEIGHT) - mine::gameState::boss_offset_y);
					}
				}
				else if (winEvent.mouseButton.button == sf::Mouse::Middle && mine::gameState::gstate == mine::PLAYING) {
					if (mine::gameState::gmode == mine::THREE) {
						mine::revealShort3x3(winEvent.mouseButton.x, winEvent.mouseButton.y);
					}
					else if (mine::gameState::gmode == mine::FIVE) {
						mine::revealShort5x5(winEvent.mouseButton.x, winEvent.mouseButton.y);
					}
				}
			}
			else if (winEvent.type == sf::Event::KeyPressed) {
				if (winEvent.key.code == sf::Keyboard::C) {
					mine::toggleColor();
				}
				else if (winEvent.key.code == sf::Keyboard::F2) {
					resetGame();
				}
				else if (winEvent.key.code == sf::Keyboard::F3) {
					mine::clearAll();
				}
				else if (winEvent.key.code == sf::Keyboard::M) {
					mine::toggle5x5();
					//debug
					std::cout << mine::gameState::numCovered << std::endl;
				}
				else if (winEvent.key.code == sf::Keyboard::S) {
					openSettings();
				}
				else if (winEvent.key.code == sf::Keyboard::Space) {
					sf::Vector2i mVec = sf::Mouse::getPosition(*this->window);
					if (mine::gameState::gmode == mine::THREE) {
						mine::revealShort3x3(mVec.x, mVec.y);
					}
					else if (mine::gameState::gmode == mine::FIVE) {
						mine::revealShort5x5(mVec.x, mVec.y);
					}
				}
			}
		}

		//loop for settings window
		if (this->settingsOpen) {
			sf::Event setEvent;
			while (this->settings->pollEvent(setEvent)) {
				if (setEvent.type == sf::Event::Closed) {
					closeSettings();
				}
				if (setEvent.type == sf::Event::MouseButtonPressed) {
					//check if it's over the tab buttons
					if (setEvent.mouseButton.button == sf::Mouse::Left) {
						if (this->gset->customGame->isMouseOver(setEvent.mouseButton.x, setEvent.mouseButton.y)) {
							this->gset->customGame->press();
						}
						if (this->gset->about->isMouseOver(setEvent.mouseButton.x, setEvent.mouseButton.y)) {
							this->gset->about->press();
						}
						if (this->gset->controls->isMouseOver(setEvent.mouseButton.x, setEvent.mouseButton.y)) {
							this->gset->controls->press();
						}
						if (this->gset->exit->isMouseOver(setEvent.mouseButton.x, setEvent.mouseButton.y)) {
							this->gset->exit->press();
						}
						if (this->gset->close->isMouseOver(setEvent.mouseButton.x, setEvent.mouseButton.y)) {
							this->gset->close->press();
						}

						//check the other menu buttons
						if (this->gset->curr == menu::NEW_GAME) {
							if (this->gset->easy->isMouseOver(setEvent.mouseButton.x, setEvent.mouseButton.y)) {
								this->gset->easy->press();
							}
							if (this->gset->normal->isMouseOver(setEvent.mouseButton.x, setEvent.mouseButton.y)) {
								this->gset->normal->press();
							}
							if (this->gset->hard->isMouseOver(setEvent.mouseButton.x, setEvent.mouseButton.y)) {
								this->gset->hard->press();
							}
							if (this->gset->custom->isMouseOver(setEvent.mouseButton.x, setEvent.mouseButton.y)) {
								this->gset->custom->press();
							}
							if (this->gset->startGame->isMouseOver(setEvent.mouseButton.x, setEvent.mouseButton.y)) {
								this->gset->startGame->press();
							}
						}
						if (this->gset->sizeX->isMouseOver(setEvent.mouseButton.x, setEvent.mouseButton.y)) {
							this->gset->sizeX->isSelected = true;
							this->gset->sizeY->isSelected = false;
							this->gset->mineCount->isSelected = false;
						}
						else if (this->gset->sizeY->isMouseOver(setEvent.mouseButton.x, setEvent.mouseButton.y)) {
							this->gset->sizeY->isSelected = true;
							this->gset->mineCount->isSelected = false;
							this->gset->sizeX->isSelected = false;
						}
						else if (this->gset->mineCount->isMouseOver(setEvent.mouseButton.x, setEvent.mouseButton.y)) {
							this->gset->mineCount->isSelected = true;
							this->gset->sizeX->isSelected = false;
							this->gset->sizeY->isSelected = false;
						}
						else {
							this->gset->sizeX->isSelected = false;
							this->gset->sizeY->isSelected = false;
							this->gset->mineCount->isSelected = false;
						}
					}
				}
				else if (setEvent.type == sf::Event::KeyPressed) {
					if (this->gset->sizeX->isSelected) {
						this->gset->sizeX->insertChar(setEvent.key.code);
					}
					else if (this->gset->sizeY->isSelected) {
						this->gset->sizeY->insertChar(setEvent.key.code);
					}
					else if (this->gset->mineCount->isSelected) {
						this->gset->mineCount->insertChar(setEvent.key.code);
					}
				}
			}
		}
	}

	void GameEngine::resetGame() {
		//re - init the game by passing params back in
		using g = mine::gameState;
		mine::style temp = g::color;
		mine::init(g::currDiff, g::gmode, this, g::numMines, g::rows, g::cols);
		mine::setColorImm(temp);
		started = false;

		//reset the toolbar
		resetToolbar();
	}


	void GameEngine::render() {
		//clear the last frame from the display buffer
		window->clear();

		//draw the toolbar
		this->toolbar->render();
		
		//draw from the array of mines
		//put together the new frame
		for (int i = 0; i < mine::gameState::rows; i++) {
			for (int k = 0; k < mine::gameState::cols; k++) {
				sf::Sprite s = *mine::gameState::gboard->get(i, k).sprite;
				window->draw(s);
			}
		}

		//push the new frame onto the buffer
		window->display();

		//now do the same for the settings window, if it's open
		if (this->settingsOpen) {
			settings->clear(sf::Color(195, 195, 195));
			//render the side bar thing
			settings->draw(*this->rect);
			this->gset->render();
			settings->display();
		}
	}
	
	void GameEngine::cleanup() {
		//delete board
		mine::gameState::gboard->destroy();
	}
}
