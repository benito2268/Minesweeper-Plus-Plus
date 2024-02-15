/*===============================
* minesweeper++ - file settings.h
* ===============================
* author: ben staehle
* date:	  10/30/22
* this program (and file) use the GNU GPL v3 License
* see COPYRIGHT.txt for license related information
* ==================================================*/
#pragma once
#include<vector>
#include<memory>

namespace engine { class GameEngine; }
namespace mine { enum difficulty; }

namespace menu {
	struct Component;
	struct Button;
	struct Text;
	struct TextBox;

	enum Tab {
		NEW_GAME, ABOUT, CONTROLS
	};

	class Settings {
	public:
		//which tab is open now
		Tab curr = NEW_GAME;

		//what difficulty is selected currently (in the menu not the game)
		mine::difficulty diff;

		std::vector<Component*> components_tab;
		std::vector<Component*> new_game_tab;
		std::vector<Component*> about_tab;
		std::vector<Component*> controls_tab;

		//individual components
		std::shared_ptr<Button> customGame;
		std::shared_ptr<Button> controls;
		std::shared_ptr<Button> about;
		std::shared_ptr<Button> exit;
		std::shared_ptr<Text> aboutText1;
		std::shared_ptr<Text> aboutText2;
		std::shared_ptr<Text> aboutText3;
		std::shared_ptr<Text> aboutText4;

		std::shared_ptr<Text> difficulty;
		std::shared_ptr<Text> sep1;
		std::shared_ptr<Text> sep2;
		std::shared_ptr<Text> sep3;

		std::shared_ptr<Button> easy;
		std::shared_ptr<Button> normal;
		std::shared_ptr<Button> hard;
		std::shared_ptr<Button> custom;

		std::shared_ptr<TextBox> mineCount;
		std::shared_ptr<TextBox> sizeX;
		std::shared_ptr<TextBox> sizeY;

		std::shared_ptr<Button> startGame;
		std::shared_ptr<Button> close;

		std::shared_ptr<Text> rendererText;
		std::shared_ptr<Text> cpuUse;
		std::shared_ptr<Text> memUse;
		std::shared_ptr<Text> plusplus;

		//keybind text
		std::shared_ptr<Text> keybindText;

		//functions
		Settings(engine::GameEngine* ref);
		void render();
		void switchTabs(Tab newTab);
		void switchDifficulties(mine::difficulty diff);
		void applyChanges(engine::GameEngine& ref);
		void addComponent(Component* c);
	};
}