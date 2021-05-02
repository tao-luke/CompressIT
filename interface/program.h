#ifndef ___PROGRAM
#define ___PROGRAM

#include "comp.h"
#include <vector>

class Program {
    unique_ptr<Comp> comp{nullptr};
    vector<Transformation> Tseq{};
    vector<string> fileNames{};
    string info = "follow format ./CompressIt <mode> <transformation> < inputflow";

    //!usage:
    // follow format ./CompressIt <mode> <transformation> < inputflow
    // ex: ex: ./CompressIt -encode -bmr < test.txt
    // the above encodes test.txt from std::cin and encodes first applying burrows wheeler,
    //move-to-front, and then runlength encoding , and saving the file in huffman at the end.

public:
    Program(int, char**);
    void run();
};

#endif
