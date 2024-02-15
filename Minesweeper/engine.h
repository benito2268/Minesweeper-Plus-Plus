/*===============================
* minesweeper++ - file engine.h
* ===============================
* author: ben staehle
* date:	  10/30/22
* this program (and file) use the GNU GPL v3 License
* see COPYRIGHT.txt for license related information
* ==================================================*/
#pragma once
#include<memory>
#include<string>
#include<chrono>
#include<sfml/Graphics.hpp>
#include<sfml/System.hpp>
#include<sfml/Window.hpp>

#define MENU_HEIGHT 70

namespace util { class Timer; }
namespace menu {
	class Display;
	class Button;
	class Toolbar;
	class Settings;
}
								
namespace engine {
	typedef unsigned int uint32;

	class GameEngine {
	private:
		std::shared_ptr<util::Timer> timer;

	protected:
		void init();
		void gameLoop();
		void update(float interval);
		void input();
		void render();
		void cleanup();

	public:
		static inline int width;
		static inline int height;
		bool started = false;
		static inline std::string title;
		static inline int ticks;
		static inline std::chrono::time_point<std::chrono::high_resolution_clock> delta;
		static inline bool settingsOpen = false;
		static inline bool newGame = true;
		std::shared_ptr<sf::Texture> texture;
		std::shared_ptr<sf::Texture> sTexture;
		std::shared_ptr<menu::Toolbar> toolbar;
		std::shared_ptr<menu::Settings> gset;
		std::shared_ptr<sf::RenderWindow> window;
		std::shared_ptr<sf::RenderWindow> settings;
		std::shared_ptr<sf::RectangleShape> rect;
		sf::View view;

		GameEngine(uint32 width, uint32 height, std::string title, bool vsync);
		void run();
		void haltGame();
		void resetGame();
		void openSettings();
		void closeSettings();
		void resetToolbar();

		inline static constexpr uint32 TARGET_FPS = 75;
		inline static constexpr uint32 TARGET_UPS = 30;

		bool vsync;

		friend class menu::Display; //this is an exception okay don't yell at me
		friend class menu::Button;
	};
}
