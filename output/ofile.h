#ifndef ___OFILE
#define ___OFILE


#include <fstream>
#include <string.h>
#include <bitset>
#include "../global.h"
#include <queue>
class Ofile{
    //! goal: produce a compressed file 
    //! input: a vector<vector<long>> of data(last one is frequency), 
    //! an encoding scheme(vector<long>), 
    //! enoding method

    //following needs use of strcpy to rid of \0

    //signature
    char FILE_SIG[3] = "LT"; //2byte  :)

    //name length
    char FILE_NAME_LENGTH; //1byte :)

    //transform history length
    char TRANSFORM_LENGTH; //1byte :)

    //name of file
    char FILE_NAME[11] = "sample.dat"; //10byte :)

    //transofrm history
    char *TRANSFORM_ARR = nullptr; // inited by ctor :) //must be freed

    //how many chars we should MATCH with data
    unsigned int COMP_CHAR_COUNT = 0;        // inited by ctor  4bytes

    //actual data char count, compare after decompress
    unsigned int FILE_BYTE_COUNT; // inited by ctor 4bytes

    long huffbyte = 0; //how many size of byte is hufftrio data
    //huff trio length
    unsigned char DATA_TRIO_COUNT = 0;  // inited by ctor

    //huff trio
    char *HUFFTRIOS = nullptr; // inited by ctor //must be freed

    //data file
    unsigned long databyte = 0;
    char *dataPtr = nullptr; //inited by ctor //must be freed

    void initTransformArr(vector<Transformation> &Tseq)
    {
        unsigned int count = Tseq.size();
        char *X_NULL_RESULT = new char[count]; //create a NOT null terminated char arr
        for (size_t i = 0; i < count;i++)
        {
            X_NULL_RESULT[i] = Tseq[i];
        }

        TRANSFORM_ARR = X_NULL_RESULT;
        cout << "complete Transform Init" << endl;
    }
    void insertBigChar(vector<unsigned char>& result,unsigned long n)
    {
        int digits = ceil(result.back()/(double)8.0);
        if (digits > 8)
            throw Error("unsupported file");
        unsigned char *buffer = new unsigned char[digits];
        memcpy(buffer, &n, digits);
        for (int i = 0; i < digits; i++)
        {
            result.push_back(buffer[i]);
        }
        delete[] buffer;
    }
    void initHuffTrio(vector<long> encodeMapArr){
        // have the entries in order, if an entry is bigger than 1 byte, it wll be 2 bytes :)
        vector<unsigned char> result{};
        unsigned int count = encodeMapArr.size();
        for (size_t i = 0; i < count;i++){
            if (i % 3 != 2)
            {
                result.push_back(encodeMapArr[i]);
            }
            else
            {
                insertBigChar(result, encodeMapArr[i]);
            }
        }
        HUFFTRIOS = new char[result.size()];
        memcpy(HUFFTRIOS, reinterpret_cast<char *>(result.data()), result.size());
        DATA_TRIO_COUNT = count / 3;
        huffbyte = result.size();
        cout << "expected pair count: " << static_cast<unsigned int>( DATA_TRIO_COUNT) << endl;
        if (count % 3 != 0)
            throw Error("encodeMparr not divisble by 3");
        cout << "complete Huff Trio" << endl;
    }
    void writeToFile(){



        ofstream outfile;
        outfile.open("test.dat", ios::binary | ios::out);
        outfile.write(FILE_SIG, 2 * sizeof(char));
        outfile.write(&FILE_NAME_LENGTH, sizeof(char));
        outfile.write(&TRANSFORM_LENGTH, sizeof(char));
        outfile.write(FILE_NAME, sizeof(char) * FILE_NAME_LENGTH);
        outfile.write(TRANSFORM_ARR, sizeof(char) * TRANSFORM_LENGTH);

        cerr << COMP_CHAR_COUNT << endl;
        cerr << FILE_BYTE_COUNT<< endl;

        outfile.write(reinterpret_cast<char *>(&COMP_CHAR_COUNT), sizeof(COMP_CHAR_COUNT));
        outfile.write(reinterpret_cast<char *>(&FILE_BYTE_COUNT), sizeof(FILE_BYTE_COUNT));
        outfile.write(reinterpret_cast<char *>(&DATA_TRIO_COUNT), sizeof(DATA_TRIO_COUNT));
        outfile.write(HUFFTRIOS, huffbyte*sizeof(char));
        outfile.write(dataPtr, databyte*sizeof(char));
        outfile.write(FILE_SIG, 2 * sizeof(char));

        outfile.close();

        // ifstream infile;
        // infile.open("test.dat", ios::binary | ios::in);
        // unsigned int test;
        // unsigned int test2;
        // infile.read(reinterpret_cast<char *>(&test), sizeof(test));
        // infile.read(reinterpret_cast<char *>(&test2), sizeof(test));
        // cout << test << endl;
        // cout << test2 << endl;
        cout << "write complete" << endl;
    }
    void flatten(vector<long>& flatData,vector<unique_ptr<Block>>& data){
        for(auto && ptr: data){
            for(const auto& n: ptr->getData()){
                flatData.push_back(n);
            }
        }
        data.clear(); //free up data
    }
    void initData(vector<unique_ptr<Block>>& data,const vector<long>&encodingLength){
        queue<bool> stream{};
        bitset<64> tmp{};
        vector<long> flatData{};
        flatten(flatData, data);
        unsigned int size = flatData.size();
        for (size_t i = 0; i < size;i++){
            tmp = bitset<64>(flatData[i]);
            for (int j = encodingLength[i] - 1; j >= 0;j--){
                stream.push(tmp[j]);
            }
        }
        vector<unsigned char> result{};
        bool out = false;
        while (true)
        {
            int counter = 0;
            tmp.reset();
            for (int i = 7; i >= 0;i--){
                if (stream.empty())
                {
                    result.push_back(counter);
                    COMP_CHAR_COUNT = result.size();
                    if (counter != 0){
                        result.push_back(tmp.to_ulong());
                    }
                    out = true;
                    break;
                }
                tmp[i] = stream.front();
                stream.pop();
                counter++;

            }
            if (out)
                break;
            result.push_back(tmp.to_ulong());
        }

        //debug

        bitset<8> boop{};
        bitset<200> net{};
        int cc = 199;
        for (int i = 0; i < 25; i++)
        {
            boop = bitset<8>(result[i]);
            for (int j = 7; j >= 0; j--)
            {
                net[cc--] = boop[j];
            }
        }

        cout << "debugggg" << endl;
        vector<long> reverseDebug{};
        for (int i = 0; i < 15; i++)
        {
            reverseDebug.push_back((net >> (200 - encodingLength[i])).to_ulong());
            cout << " actual" <<  reverseDebug.back() << endl;
            cout << " we expect" << flatData[i] << endl <<endl;
            net = (net << encodingLength[i]);
        }

        

        char *X_NULL_RESULT = new char[result.size()];


        memcpy(X_NULL_RESULT, reinterpret_cast<char *>(result.data()), result.size());
        dataPtr = X_NULL_RESULT;
        databyte = result.size();

        // for (const auto &e : result)
        // {
        //     cout << static_cast<unsigned int>(e) << " ";
        // }
        cout << "complete data init" << endl;
    }
//! yes we forgot the last char here!
public:
    Ofile(vector<unique_ptr<Block> > &data, vector<long> encodeMapArr, vector<Transformation> Tseq,unsigned int originalSize):FILE_BYTE_COUNT(originalSize)
    {
        const vector<long> encodingLengthArr = std::move(data.back()->getData());
        data.pop_back(); //pop out the encodinglength arr

        FILE_NAME_LENGTH = 10;

        TRANSFORM_LENGTH = Tseq.size(); //size of transformation sequence
        initTransformArr(Tseq);
        initHuffTrio(encodeMapArr);
        initData(data,encodingLengthArr);
        writeToFile();
        delete[] TRANSFORM_ARR;
        delete[] HUFFTRIOS;
        delete[] dataPtr;
    }
};
#endif