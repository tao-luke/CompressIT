#ifndef ___STDIN
#define ___STDIN

#include <utility>
#include "input.h"
#include <iostream>
//!  GOAL: output the binary info of user input(either file or stdin)
class Stdin: public Input{
public:
    Stdin(bool c):Input(cin){
        run(c);
    }
};

#endif