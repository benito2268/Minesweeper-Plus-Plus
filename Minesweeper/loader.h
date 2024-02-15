/*===============================
* minesweeper++ - file loader.h
* ===============================
* author: ben staehle
* date:	  10/30/22
* this program (and file) use the GNU GPL v3 License
* see COPYRIGHT.txt for license related information
* ==================================================*/
#pragma once
#pragma warning(disable:4996)  //shut

#include"resource.h"
#include<windows.h>
#include<string>
#include<cstdlib>

namespace util {
	class Resource {
    public:
        struct Parameters {
            std::size_t size_bytes = 0;
            void* ptr = nullptr;  //lost in the sauce
        };

    private:
        HRSRC hResource = nullptr;
        HGLOBAL hMemory = nullptr;

        Parameters p;
        
    public:
        Resource(int resource_id, int resource_class) {
            hResource = FindResource(nullptr, MAKEINTRESOURCE(resource_id), MAKEINTRESOURCE(resource_class));
            hMemory = LoadResource(nullptr, hResource);

            p.size_bytes = SizeofResource(nullptr, hResource);
            p.ptr = LockResource(hMemory);
        }

        auto GetResourceAsCharPtr() const {
            uint8_t* dst; 
            dst = reinterpret_cast<uint8_t*>(p.ptr);  //back from the sauce
            return dst;
        }

        auto GetResourceNoType() const {
            return p.ptr;
        }

        std::size_t getResourceSize() {
            return this->p.size_bytes;
        }
	};
}