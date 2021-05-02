#include "program.h"
#include "encode.h"
#include "decode.h"
#include "../input/stdin.h"
#include "../input/ifile.h"
    
Program::Program(int c, char **argv) {
    if (c < 2 || c > 4) throw Error("improper usage," + info);

    if (string(argv[1]) == "-decode") {
        comp = unique_ptr<Comp>(new Decode());
    } else if (string(argv[1]) == "-encode") {
        comp = unique_ptr<Comp>(new Encode(argv[2],fileNames));
    } else {
        throw Error("invalid mode selected," + info + " , ex: ./CompressIt -encode -bmr < test.txt ");
    }

    int counter = 2;
    while(counter < c){ //for future, if files are read in
        if (argv[counter][0] != '-')
        fileNames.push_back(string(argv[counter]));
        counter++;
    }
}

void Program::run() {
    unique_ptr<Input> input{nullptr};

    if (fileNames.empty()){ //decide between from input stream or from file stream
        input = unique_ptr<Input>(new Stdin(comp->encode()));
    }else{
        input = unique_ptr<Input>(new Ifile(comp->encode(), fileNames));
    }

    comp->run(input.get());
}
