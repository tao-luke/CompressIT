#ifndef ___IFILE
#define ___IFILE

#include <utility>
#include "input.h"

class Ifile:public Input{
    string fileName{};
    void read() override;
    void decodeRead() override;
    string getFileName() override;
    bool verifySig();
    unsigned char getNextChar();
    string getName(unsigned char length);
    template <typename T>void readNArr(vector<T> &mem,unsigned int length,bool typecheck);
    unsigned int getInt();
    void readHuff(vector<long> &mem, unsigned int size);
public:
    Ifile(bool c,const vector<string>& s);
};

#endif