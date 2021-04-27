#include "stdin.h"
#include "line.h"
#include <iostream>
#include <memory>
Stdin::Stdin(bool c):Input(){
    if (c)
        read();
    else
        decodeRead();
}

void Stdin::read(){
    std::string tmp {};
    while(getline(std::cin,tmp)){
        insertToData(std::unique_ptr<Block>(new Line(tmp)));
    }
}

bool Stdin::verifySig(){
    char* buffer2 = new char[3];
    buffer2 [2] = '\0';
    cin.read(buffer2, 2); //attempt to read signature
    string tmp(buffer2);
    delete [] buffer2;
    return (tmp == "LT");
}
unsigned char Stdin::getNextChar(){
    char * buffer1 = new char[1];
    cin.read(buffer1, 1);
    unsigned char tmp = static_cast<unsigned char> (buffer1 [0]);
    delete [] buffer1;
    return tmp;
}

string Stdin::getName(unsigned char length){
    char *buffern = new char[length+1];
    buffern[length] = '\0';
    cin.read(buffern, length);
    string tmp(buffern);
    delete[] buffern;
    return tmp;
}

template <typename T>void Stdin::readNArr(vector<T> &mem,unsigned char length,bool typecheck){
    char c;
    for (int i = 0; i < length;i++){
        cin.read(&c, 1);
        if (typecheck && (c >= Transformation::size_of_enum))
            throw Error("invalid transformation arr");
        mem.push_back(c);
    }
}
unsigned int Stdin::getInt(){
    char* buffer = new char[4];
    unsigned int test = 0;
    std::cin.read(reinterpret_cast<char *>(&test),4);
    return test;
}

void Stdin::decodeRead(){
    if (!verifySig())
        throw Error("invalid signature for decoding");
    unsigned char name_length = getNextChar();

    unsigned char trans_length = getNextChar();

    string name = getName(name_length);
    cout << " name:  " << name << endl;

    vector<long> transform_arr{};
    readNArr(transform_arr,trans_length,1);
    insertToData(std::unique_ptr<Block>(new Line(std::move(transform_arr))));
    // cout << "trans arr: ";
    // for(const auto& c: transform_arr){
    //     cout << c << " ";
    // }
    unsigned int comp_char_count = getInt();
    cout << comp_char_count << endl;
    unsigned int org_char_count= getInt();
    cout << org_char_count << endl;

    unsigned char trio_count = getNextChar();
    // cout << "trio_count:  " << static_cast<int>(trio_count) << endl;

    vector<long> hufftrios{};
    readNArr(hufftrios, trio_count,0);
    insertToData(std::unique_ptr<Block>(new Line(std::move(hufftrios))));
    // cout << "trio arr:  ";

    // int counter = 1;
    // for (const auto &c : hufftrios)
    // {
    //     cout << static_cast<int>(c) << " ";
    //     if (counter % 3 == 0)
    //     {
    //         cout << endl;
    //     }
    //     counter++;
    // }
    vector<long> dataptr{};
    readNArr(dataptr, comp_char_count, 0);
    insertToData(std::unique_ptr<Block>(new Line(std::move(dataptr))));
    // int counter = 1;
    // for (const auto &c : dataptr)
    // {
    //     cout << static_cast<int>(c) << " ";
    //     if (counter % 3 == 0)
    //     {
    //         cout << endl;
    //     }
    //     counter++;
    // }
    if (!verifySig())
        throw Error("invalid signature for decoding");
}