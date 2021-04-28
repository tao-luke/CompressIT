#ifndef ___BLOCK
#define ___BLOCK

#include <vector>
#include <utility>

//!  GOAL: contian the information for each "block" of data,ex pixle, a line of input and etc. 
class Block{
    protected:
    std::vector<long> data;
    public:
        std::vector<long> &getData();
        void setData(std::vector<long> &&ref);
        void insertToData(long num);
        void clearData();
        size_t getDataSize();
        Block(std::vector<long> &&input) : data(std::move(input)) {}
        Block() {}
        void popEOT();
        virtual ~Block(){};
};

#endif