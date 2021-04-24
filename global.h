#ifndef ___MTF
#define ___MTF
#include <string>
using namespace std;
class Error
{
    string what{};
    Error(string what):what(what){}
    string what(){return "ERROR:"+ what};
}

enum Transformation{
    BWT = 0,
    MTF,
    RLE
}
#endif