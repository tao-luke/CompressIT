#ifndef ___INPUT
#define ___INPUT

#include <vector>
#include <utility>
#include <memory>
#include "line.h"
#include "block.h"
#include "../global.h"
#include <fstream>
//!  GOAL: output the binary info of user input(either file or stdin)
class Input{
    std::vector<std::unique_ptr<Block>> inputData;
    unsigned long original = 0;
    istream &inputStream;
    ifstream *fileptr = nullptr;
    char *decodedFileName = nullptr;

protected:
    unsigned char endValidBit = 0;
    void run(bool);
    //replace cin to stream
    bool verifySig();
    unsigned char getNextChar();
    char* getName(unsigned char length);
    template <typename T>void readNArr(vector<T> &mem,unsigned int length,bool typecheck);
    unsigned int getInt();
    void readHuff(vector<long> &mem, unsigned int size);
public:
    unsigned char getEndVal();
    unsigned long getOriginalSize();
    std::vector<std::unique_ptr<Block> >& getInputData();
    void insertToData(std::unique_ptr<Block>&&);
    char* getDecodeFileName();
    virtual void read();
    virtual void decodeRead();
    virtual ~Input(){
        if(fileptr)
            delete fileptr;
        if (decodedFileName)
            delete decodedFileName;
    };
    Input(istream &inputStream) : inputStream(inputStream){};
    Input(ifstream* i) : inputStream(*i),fileptr(i){};
};

#endif