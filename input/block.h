#ifndef ___BLOCK
#define ___BLOCK

#include <vector>
#include <utility>
#include "../global.h"

/**
 * Block is a data used to represent ONE unit of something..either pixel,line,and whatever is desired.
 * - members: 
 *    LOCAL: 
 *      - m_data: the vector that repreents the data of the one unit.
 */
class Block{
    protected:
         std::vector<long> m_data;
    public:
        //gets m_data
        std::vector<long> &getData();

        //sets m_data
        void setData(std::vector<long> &&ref);

        //push to m_data
        void insertToData(long num);

        //clears data
        void clearData();

        //gets m_data.size()
        size_t getDataSize();

        //pops the '\0' char off the end. used in decoding.
        void popEOT();
        virtual ~Block(){};
        Block(std::vector<long> &&input) : m_data(std::move(input)) {}
        Block() {}
};

#endif