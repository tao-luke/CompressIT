#ifndef ___STDIN
#define ___STDIN

#include <utility>
#include "input.h"

//!  GOAL: output the binary info of user input(either file or stdin)
class Stdin: public Input{
    void read() override;
    void decodeRead() override;
    bool verifySig();
    unsigned char getNextChar();
    string getName(unsigned char length);
    template <typename T>void readNArr(vector<T> &mem,unsigned int length,bool typecheck);
    unsigned int getInt();
    void readHuff(vector<long> &mem, unsigned int size);

public:
    Stdin(bool c); //make a vector of line
};

#endif