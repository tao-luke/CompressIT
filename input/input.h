#ifndef ___INPUT
#define ___INPUT

#include <vector>
#include <utility>
#include <memory>
#include "block.h"
//!  GOAL: output the binary info of user input(either file or stdin)
class Input{
    std::vector<std::unique_ptr<Block>> inputData;
    unsigned long original = 0;
protected:
    
public:
    unsigned long getOriginalSize(){
        return original;
    }
    std::vector<std::unique_ptr<Block>>& getInputData(){
        return inputData;
    }
    void insertToData(std::unique_ptr<Block>&& ptr){
        original += ptr->getDataSize();
        inputData.push_back(std::move(ptr));
    }
    virtual void read() = 0;
    virtual ~Input(){};
};

#endif