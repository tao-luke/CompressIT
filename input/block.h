#ifndef ___BLOCK
#define ___BLOCK

#include <vector>
#include <utility>
//!  GOAL: contian the information for each "block" of data,ex pixle, a line of input and etc. 
class Block{
    std::vector<long> data;
    public:
    std::vector<long>& getData(){
        return data;
    }
    void insertToData(long num){
        data.push_back(num);
    }
    void clearData(){
        data.clear();
    }
    virtual ~Block(){};
};

#endif