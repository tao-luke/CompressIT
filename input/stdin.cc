#include "stdin.h"
#include "line.h"
#include <iostream>
#include <memory>
Stdin::Stdin():Input(){
    read();
}

void Stdin::read(){
    std::string tmp {};
    while(getline(std::cin,tmp)){
        insertToData(std::unique_ptr<Block>(new Line(tmp)));
    }
}