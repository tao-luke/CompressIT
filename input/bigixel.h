#ifndef ___BIGXEL
#define ___BIGXEL

#include <vector>
#include "block.h"
#include <initializer_list>
//!  GOAL: contian the information for a pixel
class Bigxel: public Block{
    public:
    Bigxel(std::initializer_list<long> l):Block(),data(l){}
};

#endif