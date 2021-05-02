#ifndef ___INPUT
#define ___INPUT

#include <vector>
#include <utility>
#include <memory>
#include "line.h"
#include "block.h"
#include "../global.h"
//!  GOAL: output the binary info of user input(either file or stdin)
class Input{
    std::vector<std::unique_ptr<Block>> inputData;
    unsigned long original = 0;

protected:
    unsigned char endValidBit = 0;
    void run(bool);

public:
    unsigned char getEndVal();
    unsigned long getOriginalSize();
    std::vector<std::unique_ptr<Block> >& getInputData();
    void insertToData(std::unique_ptr<Block>&&);
    virtual string getFileName();
    virtual void read() = 0;
    virtual void decodeRead() = 0;
    virtual ~Input(){};
};

#endif