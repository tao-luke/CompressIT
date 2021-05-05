#ifndef ___GLOBAL
#define ___GLOBAL

#include <string>
using namespace std;

class Error
{
    string bad{};
    public:
    Error(string what) : bad(what){};
    string what() { return "ERROR:" + bad; };
};

enum Transformation
{
    BWT = 0,
    MTF,
    RLE,
    LZW,
    size_of_enum
};
#endif