#ifndef ___PIXEL
#define ___PIXEL

#include <vector>
#include "block.h"
#include <initializer_list>
//!  GOAL: contian the information for a pixel
class Pixel: public Block{
    public:
    Pixel(std::initializer_list<long> l):Block(),data(l){}
};

#endif