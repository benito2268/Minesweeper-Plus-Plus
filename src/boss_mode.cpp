/*==================================
* minesweeper++ - file boss_mode.cpp
* ==================================
* author: ben staehle
* date:	  11/28/22
* this program (and file) use the GNU GPL v3 License
* see COPYRIGHT.txt for license related information
* ==================================================*/
#include"engine.h"
#include"resource.h"
#include"crash.h"
#include"loader.h"
#include<fstream>
#include<iostream>
#include<cstdio>
#include<windows.h>
#include"boss_mode.h"

namespace boss {

	void bossFuncs::b_init(float width, float height) {
		//makes the window fullscreen
		//foreground is slightly transparent
		//disables settings and restart button

		util::Resource r(IDB_PNG3, PNG);
		uint8_t* mem = r.GetResourceAsCharPtr();
		std::size_t memSize = r.getResourceSize();

		backTex.loadFromMemory(mem, memSize);

		background.setPosition(0, 0);
		background.setSize(sf::Vector2f(width, height));

		//scale the texture based on screen resolution


		background.setTexture(&backTex);

		util::Resource rr(IDR_OGG1, OGG);
		void* audio = rr.GetResourceNoType();
		std::size_t audiolen = rr.getResourceSize();

		//start audio
		if (!cntdown.openFromMemory(audio, audiolen)) {
		}
		cntdown.play();
	}

	void bossFuncs::b_render(engine::GameEngine* ref) {
		ref->window->draw(background);
	}

	void bossFuncs::b_win() {
		//pause the video and audio
		cntdown.pause();

		//pause the game and open a window telling the play they have won
		int msgBoxID = MessageBox(
			NULL,
			(LPCWSTR)L"Congratulations, you have escaped your inevitable demise",
			(LPCWSTR)L"( ͡° ͜ʖ ͡°)",
			MB_ICONASTERISK | MB_OK
		);

		//pull the ultimate funny
		switch (msgBoxID) {
		case IDOK:
			std::abort();
			break;
		default:
			std::abort();
			break;
		}
	}

	void bossFuncs::b_lose() {
		//pause the video and audio
		cntdown.pause();

		//open a window to tell the user they have lost
		int msgBoxID = MessageBox(
			NULL,
			(LPCWSTR)L"oh no                                       ",
			(LPCWSTR)L"(ㆆ _ ㆆ)",
			MB_ICONERROR | MB_OK
		);

		//pull the ultimate funny
		switch (msgBoxID) {
		case IDOK:
			crash();
			break;
		default:
			crash();
			break;
		}
	}
}