/*================================
* minesweeper++ - file boss_mode.h
* ================================
* author: ben staehle
* date:	  11/28/22
* this program (and file) use the GNU GPL v3 License
* see COPYRIGHT.txt for license related information
* ==================================================*/
#pragma once
#include<sfml/Config.hpp>
#include<sfml/Graphics.hpp>
#include<sfml/Audio.hpp>
#include<string>

namespace engine { class GameEngine; }
namespace boss {
	struct bossFuncs {
		static inline sf::Music cntdown;

		static inline sf::RectangleShape background;
		static inline sf::Texture backTex;

		static void b_init(float width, float height);
		static void b_render(engine::GameEngine* ref);
		static void b_win();
		static void b_lose();
	};

} 