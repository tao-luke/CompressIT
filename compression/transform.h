#ifndef ___TRANSFORM
#define ___TRANSFORM
#include "../global.h"
#include <memory>
#include "../input/block.h"
#include <map>

#include <iostream>

using namespace std;
class Transform
{
    //!input: vector<unique_ptr<Block>> inputdata and each block is vector<long>
    //! goal: vector<unique_ptr<Block>> where each block is a vector<long>
    //! idea: each transform is like a node, and we pass in a input and it kinds goes through the node link
    Transform* next;
    bool encode = true;
    virtual void transform(vector<unique_ptr<Block> > &input) = 0;
    virtual void decode(vector<unique_ptr<Block> > &input) = 0;
    virtual void applyTo(vector<long> &data) = 0;

    void run(vector<unique_ptr<Block> > & input){ //transform the input
        initEncodeMap();
        if (input.empty())
            throw Error("empty encoding input string");
        Transform *ptr = next;
        transform(input);
        while (ptr)
        {
            ptr->transform(input);
            ptr = ptr->next;
        }
    }
    void run2(vector<unique_ptr<Block> > & input){ //transform the input
        if (input.empty())
            throw Error("empty decoding input string");
        Transform* p = next;
        decode(input);
        while (p)
        {

            // for (auto &&ptr : input)
            // {
            //     for(const auto& n: ptr->getData()){
            //         cout << n << " ";
            //     }
            // }
            p->decode(input);
            p = p->next;
        }
        input[0]->popEOT();
    }
protected:
    map<pair<long,unsigned char>, unsigned char>* encodeMap = nullptr;
    unsigned int originalSize = 0; //! not good
public:
    void setEncode(bool n){
        encode = n;
    }
    Transform(Transform* next):next(next){}
    unsigned int getOriginalSize(){
        return originalSize;
    }
    void initEncodeMap(){
        encodeMap = new map<pair<long, unsigned char>, unsigned char>{};
        Transform *ptr = next;
        while(ptr){
            ptr->encodeMap = encodeMap;
            ptr = ptr->next;
        }
    }
    void execute(vector<unique_ptr<Block> > & input){
        if (encode)
            run(input);
        else
            run2(input);
    }
    void setEncodeMap(const vector<long> &enMapArr){ 
        int size = enMapArr.size();
        initEncodeMap();
        if (size % 3 != 0)
            throw Error("invalid encoding scheme, not div3");
        for (size_t i = 0; i < size; i += 3)
        {
            encodeMap->insert({{enMapArr[i+2], enMapArr[i+ 1] }, enMapArr[i]});
        }
    }
    vector<long> getEncodeMap(){
        if (!encodeMap || encodeMap->empty())
            throw Error("empty encode map, not get able");
        vector<long> tmp{};
        for(const auto& e: *encodeMap){
            tmp.push_back(e.second);
            tmp.push_back(e.first.second);
            tmp.push_back(e.first.first);
            
            
        }
        return tmp;
    }
    virtual ~Transform(){
        if (next == nullptr){
            delete encodeMap;
        }
        delete next;
        
    };
};

#endif