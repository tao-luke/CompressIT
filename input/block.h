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
        std::unique_ptr<Block> &&getDataBlock();
        void insertToData(long num);
        void clearData();
        size_t getDataSize();
        Block(std::vector<long> &&input) : data(std::move(input)) {}
        Block() {}
        virtual ~Block(){};
};

#endif