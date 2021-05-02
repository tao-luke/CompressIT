#ifndef ___OFILE
#define ___OFILE


#include <fstream>
#include <string.h>
#include <bitset>
#include <iostream>
#include "../global.h"
#include <queue>
#include <math.h>
#include "../input/block.h"
class Ofile{
    //! goal: produce a compressed file 
    //! input: a vector<vector<long>> of data(last one is frequency), 
    //! an encoding scheme(vector<long>), 
    //! enoding method

    //following needs use of strcpy to rid of \0

    //signature
    char FILE_SIG[3] = "LT"; //2byte  

    //name length
    char FILE_NAME_LENGTH; //1byte

    //transform history length
    char TRANSFORM_LENGTH; //1byte

    //name of file
    const char* FILE_NAME = nullptr; //inited by ctor

    //transofrm history
    char *TRANSFORM_ARR = nullptr; // inited by ctor //must be freed

    //how many chars we should MATCH with data
    unsigned int COMP_CHAR_COUNT = 0;        // inited by ctor  4bytes

    //actual data char count, compare after decompress for sanity check
    unsigned int FILE_BYTE_COUNT; // inited by ctor 4bytes

    long huffbyte = 0; //how many size of byte is hufftrio data
    //huff trio length
    unsigned char DATA_TRIO_COUNT = 0;  // inited by ctor

    //huff trio
    char *HUFFTRIOS = nullptr; // inited by ctor //must be freed

    //data file
    unsigned long databyte = 0; //actual byte usage of data write
    char *dataPtr = nullptr; //inited by ctor //must be freed

    void initTransformArr(vector<Transformation> &Tseq);
    void insertBigChar(vector<unsigned char> &result, unsigned long n);
    void initHuffTrio(vector<long> encodeMapArr);

    void writeAsEncodedFile();
    void initData(vector<unique_ptr<Block> > &data, const vector<long> &encodingLength);
    void writeAsRawFile(const vector<long> &data);

public:
    Ofile(vector<unique_ptr<Block> > &data, vector<long> encodeMapArr, vector<Transformation> Tseq, unsigned int originalSize, const char *FILE_NAME, size_t FILE_NAME_LENGTH);
    Ofile(vector<long> &data, const char *FILE_NAME);
};
#endif