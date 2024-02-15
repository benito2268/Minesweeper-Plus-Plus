/*===============================
* minesweeper++ - file mtable.h
* ===============================
* author: ben staehle
* date:	  10/30/22
* this program (and file) use the GNU GPL v3 License
* see COPYRIGHT.txt for license related information
* ==================================================*/
#pragma once

namespace util {
	template<typename T>
	struct Table_2d {
		T** arr;  //2d array, think of it as a container of containers of variable size
		int wi;   //the initial size of the x dimension
		int hi;   //the initial size of the y dimension

		Table_2d(int w, int h) {
			arr = new T*[w]; //create set of 'w' rows that will contain columns
			for (int i = 0; i < w; i++) {
				arr[i] = new T[h]; //create a column of height 'h' within each row
			}

			wi = w;
			hi = h;
		}

		T get(int x, int y) {  //gets a copy of the element 
			return arr[x][y];
		}

		T* getptr(int x, int y) {  //gets a pointer to the element itself
			return &arr[x][y];
		}

		void set(T data, int x, int y) {
			arr[x][y] = data;
		}

		//frees memory allocated to the board
		void destroy() {
			for (int i = 0; i < wi; i++) {
				delete[] arr[i];
			}
			delete[] arr;
		}
	};
}