#include "program.h"
#include "encode.h"
#include "decode.h"
#include "../input/stdin.h"
#include "../input/ifile.h"
    
Program::Program(int c, char **argv) {
    if (c < 2 || c > 4) throw Error("improper usage," + info);

    if (string(argv[1]) == "-decode") {
        m_comp = unique_ptr<Comp>(new Decode());
    } else if (string(argv[1]) == "-encode") {
        m_comp = unique_ptr<Comp>(new Encode(c >= 3 ? argv[2] : nullptr, m_file_names));
    } else {
        throw Error("invalid mode selected," + info + " , ex: ./CompressIt -encode -bmr < test.txt ");
    }

    int counter = 2;
    while(counter < c){ //for future, if files are read in
        if (argv[counter][0] != '-')
        m_file_names.push_back(string(argv[counter]));
        counter++;
    }
}

void Program::run() {
    unique_ptr<Input> input{nullptr};

    if (m_file_names.empty()){ //decide between from input stream or from file stream
        input = unique_ptr<Input>(new Stdin(m_comp->encode()));
    }else{
        input = unique_ptr<Input>(new Ifile(m_comp->encode(), m_file_names));
    }

    m_comp->run(input.get());
}
