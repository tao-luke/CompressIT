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
    template <typename T>void readNArr(vector<T> &mem,unsigned char length,bool typecheck);
    unsigned int getInt();

public:
    Stdin(bool c); //make a vector of line
};

#endif