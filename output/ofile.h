#ifndef ___OFILE
#define ___OFILE


#include <fstream>
#include <string.h>
#include <bitset>
#include "../global.h"
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

    //huff trio length
    unsigned char DATA_TRIO_COUNT = 0;  // inited by ctor

    //huff trio
    unsigned char *HUFFTRIOS = nullptr; // inited by ctor //must be freed

    //data file
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
    void initHuffTrio(vector<long> encodeMapArr){
        unsigned int count = encodeMapArr.size();
        unsigned char *X_NULL_RESULT = new unsigned char[count];
        for (size_t i = 0; i < count;i++){
            if (encodeMapArr[i] > 255){
                cout << encodeMapArr[i] << endl;
                throw Error("weird bug at initHuffTrio");
            }
            X_NULL_RESULT[i] = static_cast<unsigned char>(encodeMapArr[i]);
        }
        HUFFTRIOS = X_NULL_RESULT;
        DATA_TRIO_COUNT = count;
        cout << "complete HUff Trio" << endl;
    }
    void writeToFile(){
        // ofstream outfile;
        // outfile.open("junk.dat", ios::binary | ios::out);
        // char c[12] = "lt";
        // outfile.write(c, 2*sizeof(char));
        // outfile.close();


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
        outfile.write(reinterpret_cast<char *>(HUFFTRIOS), DATA_TRIO_COUNT*sizeof(char));
        outfile.write(reinterpret_cast<char *>(dataPtr), COMP_CHAR_COUNT*sizeof(char));
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
        bitset<8> bytemimick;
        vector<long> flatData{};
        flatten(flatData, data);
        vector<unsigned char> result{};
        int capacity = 8;
        size_t size = encodingLength.size();

        size_t counter = 0;
        bitset<8> tmp;
        while (counter < size)
        {
            if (capacity >= encodingLength[counter])
            {                                        //fit case
                tmp = bitset<8> (flatData[counter]); //to be copied
                capacity -= encodingLength[counter]; //update capacity 
                int start = capacity;     //starting bit of the mimick
                
                for (int i = 0; i < encodingLength[counter]; i++)
                {
                    bytemimick[start] = tmp[i];
                    start++;
                }
                counter++; //check next data;
                if (capacity == 0){
                    result.push_back(static_cast<char>( bytemimick.to_ulong()));
                    capacity = 8;
                    bytemimick.reset();
                }
            }
            else
            { //length > capacity, so we copy as many as possible.
                tmp = bitset<8> (flatData[counter]);
                int tp = encodingLength[counter] - capacity; //make tp the begining par tof the copy //4
                for (int i = 0; i < capacity;i++){ //copy some over to mimick untill mimick full //3
                    bytemimick[i] = tmp[tp];
                    tp++;
                }
                result.push_back(static_cast<char>( bytemimick.to_ulong()));
                bytemimick.reset();
                capacity = 8 - (encodingLength[counter] - capacity); //4
                tp = capacity;
                for (int i = 0; tp < 8; i++)
                {
                    bytemimick[tp] = tmp[i];
                    tp++;
                }
                counter++;
            }
        }
        char *X_NULL_RESULT = new char[result.size()];
        COMP_CHAR_COUNT = result.size();
        memcpy(X_NULL_RESULT, reinterpret_cast<char *>(result.data()), result.size());
        dataPtr = X_NULL_RESULT; 
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