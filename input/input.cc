#include "input.h"
#include "stdin.h"
#include <memory>
#include <math.h>
#include <cstring>
void Input::run(bool c){
  if (c)
    read();
  else
    decodeRead();
}

void Input::read(){
    vector<long> result{};
    char c;
    while (m_input_stream.get(c))
    {
        result.push_back(static_cast<unsigned char>(c));
    }
    insertToData(std::unique_ptr<Block>(new Line(std::move(result),1)));
}

void Input::decodeRead(){
    if (!verifySig()) //verify signature
        throw Error("invalid signature for decoding"); 
    unsigned char name_length = getNextChar(); //read name length

    unsigned char trans_length = getNextChar();

    m_decoded_file_name = getName(name_length);

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
unsigned char Input::getEndVal(){
  return endValidBit;
}

unsigned long Input::getOriginalSize(){
  return m_original;
}

char* Input::getDecodeFileName(){
  return m_decoded_file_name;
}

bool Input::verifySig(){ //verify signature
    char* buffer2 = new char[3];
    buffer2 [2] = '\0';
    m_input_stream.read(buffer2, 2); //attempt to read signature
    string tmp(buffer2);
    delete [] buffer2;
    return (tmp == "LT");
}
unsigned char Input::getNextChar(){ //gets the next char in unsigned from std::in
    unsigned char buffer1;
    m_input_stream.read(reinterpret_cast<char*>( &buffer1), 1);
    return buffer1;
}

char* Input::getName(unsigned char length){
    char *buffern = new char[length+6];
    buffern[length+5] = '\0';
    buffern[0] = '(';
    buffern[1] = 'd';
    buffern[2] = 'e';
    buffern[3] = 'c';
    buffern[4] = ')';
    //create output format for decoding as (dec)name.txt
    m_input_stream.read(buffern+5, length);
    return buffern;
}
template <typename T>void Input::readNArr(vector<T> &mem,unsigned int length,bool typecheck){
    //reads N chars from cin to mem
    unsigned char c;
    for (unsigned int i = 0; i < length;i++){
        m_input_stream.read(reinterpret_cast<char*>( &c), 1);
        if (typecheck && (c >= Transformation::size_of_enum))
            throw Error("invalid transformation arr");
        mem.push_back(c);
    }
}
void Input::readHuff(vector<long>& mem,unsigned int size){
    //reads the huffman map encoding from cin
    unsigned long tmp;
    unsigned char c;
    unsigned int digits = 0;
    for (unsigned int i = 1; i <= size; i++)
    {
        if (digits != 0){ //reading big int 
            unsigned char *buffer = new unsigned char[digits];
            m_input_stream.read(reinterpret_cast<char *>(buffer), digits);
            memcpy(&tmp, buffer, digits);
            digits = 0;
            delete[] buffer;

        }
        else
        {
            m_input_stream.read(reinterpret_cast<char *>(&c), 1);
            tmp = c;
            if (i % 3 == 2)
            {
                digits = ceil(tmp/(double)8.0);
            }
        }
        mem.push_back(tmp);
    }
}
unsigned int Input::getInt(){
    unsigned int test = 0;
    m_input_stream.read(reinterpret_cast<char *>(&test),4);
    return test;
}
std::vector<std::unique_ptr<Block>>& Input::getInputData(){
  return m_input_data;
}

void Input::insertToData(std::unique_ptr<Block>&& ptr){
  m_original += ptr->getDataSize();
  m_input_data.push_back(std::move(ptr));
}