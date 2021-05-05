#ifndef ___LINE
#define ___LINE

#include <vector>
#include "block.h"
#include <string>
/**
 * Line is a subclass of a Block. And is used to represent a line in a txt doc
 * 
 * as it appears, this concept is VERY flawed, as most encoding methods shove all the data in one "line"
 * we should later replace this as a just a raw "file".
 * - members: 
 *  INHERITED: 
 *      - m_data: the vector that repreents the data of the one unit.
 *  LOCAL:
 */
class Line: public Block{
    public:
    Line(std::string read){
        for(const char& c:read){
            insertToData(static_cast<long>(c));
        }
        insertToData(0); //EOL char (does this in encoding)
    }
    Line(vector<long>&& ref){
        m_data = std::move(ref);
    }
};

#endif