#ifndef ___IFILE
#define ___IFILE

#include <utility>
#include "input.h"
#include <fstream>
class Ifile:public Input{

public:
    Ifile(bool c,const vector<string>& fileNames):Input(new ifstream (fileNames[0],std::ifstream::in)){
    //! extend this to handle multiple files!
    //current default to setting name as first file
    run(c);
}
};

#endif