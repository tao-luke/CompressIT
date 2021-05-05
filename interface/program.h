#ifndef ___PROGRAM
#define ___PROGRAM

#include "comp.h"
#include <vector>
/**
 * Program is the core of the application. encapsulating the running instance of Comp and information neded
 * by the output. initiaes the entire program basically
 * 
 * - members: 
 *  LOCAL:
 *     - m_comp: the internal Comp  that runs either decode or encode
 *     - m_file_names: the vector of filenames read
 *     - info: useful string
 */
class Program {
    unique_ptr<Comp> m_comp{nullptr};
    vector<string> m_file_names{};
    string info = "follow format ./CompressIt <mode> <transformation> < inputflow";

    // follow format ./CompressIt <mode> <transformation> < inputflow
    // ex: ex: ./CompressIt -encode -bmr < test.txt
    // the above encodes test.txt from std::cin and encodes first applying burrows wheeler,
    //move-to-front, and then runlength encoding , and saving the file in huffman at the end.

public:
    //decides mode of program, reads fileName
    Program(int, char**);

    //runs program
    void run();
};

#endif
