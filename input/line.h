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
    Line(vector<long>&& ref,bool appendEnd = false){
        m_data = std::move(ref);
        if (appendEnd){
            insertToData(0); //EOL char (does this in encoding)
        }
    }
};

#endif