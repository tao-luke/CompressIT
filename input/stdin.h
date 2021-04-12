#ifndef ___STDIN
#define ___STDIN

#include <utility>
#include "input.h"

//!  GOAL: output the binary info of user input(either file or stdin)
class Stdin: public Input{
    void read() override;

public:
    Stdin(); //make a vector of line
};

#endif