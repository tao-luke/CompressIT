#include <fstream>
#include "../global.h"
#include <string>
#include <bitset>
class File{
    //! goal: produce a compressed file 
    //! input: a vector<vector<long>> of data(last one is frequency), 
    //! an encoding scheme(vector<long>), 
    //! enoding method

    //following needs use of strcpy to rid of \0
    unsigned char FILE_SIG[] = "LT"; //2byte
    unsigned char FILE_NAME_LENGTH = 10; //1byte
    unsigned char TRANSFORM_LENGTH; //1byte
    unsigned char FILE_NAME[] = "sample.dat"; //10byte
    unsigned char *TRASNFORM_ARR = nullptr; // inited by ctor
    unsigned int COMP_FILE_SIZE = 0;        //! inited by ctor
    unsigned int FILE_SIZE; // inited by ctor
    unsigned char DATA_TRIO_COUNT = 0;  // inited by ctor
    unsigned char *HUFFTRIOS = nullptr; // inited by ctor
    unsigned char *data = nullptr; //!inited by ctor
    void initTransformArr(vector<Transformation> &Tseq)
    {
        unsigned int count = Tseq.size();
        unsigned char *X_NULL_RESULT = new unsigned char[count]; //create a NOT null terminated char arr
        for (size_t i = 0; i < count;i++)
        {
            X_NULL_RESULT[i] = Tseq[i];
        }

        TRASNFORM_ARR = X_NULL_RESULT;
    }
    void initHuffTrio(const vector<long>& encodeMapArr){
        unsigned int count = encodeMapArr.size();
        unsigned char *X_NULL_RESULT = new unsigned char[count];
        for (size_t i = 0; i < count;i++){
            if (encodeMapArr[i] > 255)
                throw Error("weird bug at initHuffTrio");
            X_NULL_RESULT[i] = static_cast<unsigned char>(encodeMapArr[i]);
        }
        HUFFTRIOS = X_NULL_RESULT;
        DATA_TRIO_COUNT = count;
    }
    void writeToFile(){

    }
    void initData(const vector<unique_ptr<Block>>& data,const vector<long>&encodingLength){
        bitset<8> bytemimick;
        vector<char> result{};
        int capacity = 8;
        size_t counter = 0;
        while (capacity >=encodingLength[counter]){
            capacity -= encodingLength[counter]; //update capacity
            int start = capacity;     //starting bit of the mimick
            bitset<encodingLength[counter]> tmp(data[counter]); //to be copied
            for (int i = 0; i < encodingLength[counter]; i++)
            {
                bytemimick[start] = tmp[i];
                start++;
            }
            counter++; //check next data;

            if (capacity == 0){
                capacity = 8;
                result.push_back(static_cast<char>( bytemimick.to_ulong()));
            }
        }
        //! this does not account for overflow bit > capacity lol make this work!
        //!
    }

public:
    File(vector<unique_ptr<Block> > &data, const vector<long> &encodeMapArr, vector<Transformation> Tseq,unsigned int originalSize):TRANSFORM_LENGTH(Tseq.size()),FILE_SIZE(originalSize)
    {
        const vector<long> encodingLengthArr = std::move(data.back()->getData());
        int encodeCounter = 0;
        data.pop_back(); //pop out the encodinglength arr

        initTransformArr(Tseq);
        initHuffTrio(encodeMapArr);
        initData(data);
    }
}