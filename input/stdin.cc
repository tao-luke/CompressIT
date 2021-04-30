#include "stdin.h"
#include "line.h"
#include <iostream>
#include <memory>
Stdin::Stdin(bool c):Input(){ //read either normally or read as decode format
    if (c)
        read();
    else
        decodeRead();
}

void Stdin::read(){
    std::string tmp {};
    while(true){
        int c = getchar();
        if (c == -1)
            break;
        tmp.push_back(c);
    }
    insertToData(std::unique_ptr<Block>(new Line(std::move(tmp))));
}

bool Stdin::verifySig(){ //verify signature
    char* buffer2 = new char[3];
    buffer2 [2] = '\0';
    cin.read(buffer2, 2); //attempt to read signature
    string tmp(buffer2);
    delete [] buffer2;
    return (tmp == "LT");
}
unsigned char Stdin::getNextChar(){ //gets the next char in unsigned from std::in
    unsigned char buffer1;
    cin.read(reinterpret_cast<char*>( &buffer1), 1);
    return buffer1;
}

string Stdin::getName(unsigned char length){
    char *buffern = new char[length+1];
    buffern[length] = '\0';
    cin.read(buffern, length);
    string tmp(buffern);
    delete[] buffern;
    return tmp;
}

template <typename T>void Stdin::readNArr(vector<T> &mem,unsigned int length,bool typecheck){
    //reads N chars from cin to mem
    unsigned char c;
    for (int i = 0; i < length;i++){
        cin.read(reinterpret_cast<char*>( &c), 1);
        if (typecheck && (c >= Transformation::size_of_enum))
            throw Error("invalid transformation arr");
        mem.push_back(c);
    }
}
void Stdin::readHuff(vector<long>& mem,unsigned int size){
    //reads the huffman map encoding from cin
    unsigned long tmp;
    unsigned char c;
    unsigned int digits = 0;
    for (int i = 1; i <= size; i++)
    {
        if (digits != 0){ //reading big int 
            unsigned char *buffer = new unsigned char[digits];
            cin.read(reinterpret_cast<char *>(buffer), digits);
            memcpy(&tmp, buffer, digits);
            digits = 0;
            delete[] buffer;

        }
        else
        {
            cin.read(reinterpret_cast<char *>(&c), 1);
            tmp = c;
            if (i % 3 == 2)
            {
                digits = ceil(tmp/(double)8.0);
            }
        }
        mem.push_back(tmp);
    }
}
unsigned int Stdin::getInt(){
    unsigned int test = 0;
    std::cin.read(reinterpret_cast<char *>(&test),4);
    return test;
}

void Stdin::decodeRead(){
    if (!verifySig()) //verify signature
        throw Error("invalid signature for decoding");
    unsigned char name_length = getNextChar(); //read name length

    unsigned char trans_length = getNextChar();

    string name = getName(name_length);
    cout << " name:  " << name << endl;

    vector<long> transform_arr{};
    readNArr(transform_arr,trans_length,1);
    insertToData(std::unique_ptr<Block>(new Line(std::move(transform_arr))));


    unsigned int comp_char_count = getInt();
    cout << comp_char_count << endl;
    unsigned int org_char_count= getInt();
    cout << org_char_count << endl;

    unsigned int trio_count = getNextChar()*3;
    // cout << "trio_count:  " << static_cast<int>(trio_count) << endl;
    vector<long> hufftrios{};
    readHuff(hufftrios, trio_count);
    // cout << "reading # trio pairs: " << trio_count / 3 << endl;
    // cout << "trio arr:  ";

    // int counter = 1;
    // for (const auto &c : hufftrios)
    // {
    //     cout << static_cast<unsigned int>(c) << " ";
    //     if (counter % 3 == 0)
    //     {
    //         cout << endl;
    //     }
    //     counter++;
    // }
    insertToData(std::unique_ptr<Block>(new Line(std::move(hufftrios))));

    vector<long> dataptr{};
    readNArr(dataptr, comp_char_count, 0);

    endValidBit = dataptr.back();
    dataptr.pop_back();
    if (endValidBit != 0){
        dataptr.push_back(getNextChar());
    }
    // for(const auto& e:dataptr){
    //     cout << e << " ";
    // }
    insertToData(std::unique_ptr<Block>(new Line(std::move(dataptr))));

    if (!verifySig())
        throw Error("invalid signature for decoding");
    cout << "complete decode read, last byte only valid for first(bits) " << static_cast<unsigned int>( endValidBit) << endl;
}