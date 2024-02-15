/*===============================
* minesweeper++ - file timer.h
* ===============================
* author: ben staehle
* date:	  10/30/22
* this program (and file) use the GNU GPL v3 License
* see COPYRIGHT.txt for license related information
* ==================================================*/
#pragma once
#include<chrono>

namespace util {

	class Timer {
	private:
		double lastLoopTime;
		std::chrono::time_point<std::chrono::high_resolution_clock> time;
	public:
		void init() {
			using namespace std::chrono;
			this->time = high_resolution_clock::now();
			this->lastLoopTime = getLastLoopTime();
		}

		double getTime() {
			using namespace std::chrono;
			auto toRet = duration_cast<milliseconds>(high_resolution_clock::now() - this->time);
			return (double)toRet.count() / 1000.0;
		}

		float getElapsedTime() {
			double time = getTime();
			float elapsedTime = (float)(time - this->lastLoopTime);
			lastLoopTime = time;
			return elapsedTime;
		}

		double getLastLoopTime() {
			return this->lastLoopTime;
		}
	};
}
