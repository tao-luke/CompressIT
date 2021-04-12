#ifndef ___LINE
#define ___LINE

#include <vector>
#include "block.h"
#include <string>
//!  GOAL: contian the information for a pixel
class Line: public Block{
    public:
    Line(std::string read){
        for(const char & c: read){
            insertToData(static_cast<long>(c));
        }
    }
};

#endif