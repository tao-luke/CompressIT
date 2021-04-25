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

    virtual void transform(vector<unique_ptr<Block> > &input) = 0;
    virtual void decode(vector<unique_ptr<Block> > &input) = 0;
    virtual void applyTo(vector<long> &data) = 0;
    virtual void deplyTo(vector<long> &data) = 0;
protected:
    map<pair<int, int>, long>* encodeMap = nullptr;
    unsigned int originalSize = 0; //! not good
public:
    Transform(Transform* next):next(next){}
    unsigned int getOriginalSize(){
        return originalSize;
    }
    void run(vector<unique_ptr<Block> > & input){ //transform the input
        encodeMap = new map<pair<int, int>, long>{};
        if (input.empty())
            throw Error("empty encoding input string");
        Transform *ptr = next;
        transform(input);
        while (ptr)
        {
            ptr->encodeMap = encodeMap;
            ptr->transform(input);
            ptr = ptr->next;
        }
    }
    void run2(vector<unique_ptr<Block> > & input){ //transform the input
        if (input.empty())
            throw Error("empty decoding input string");
        Transform* ptr = next;
        decode(input);
        //! dont forget that the encodeMap should only be good at front
        while (ptr)
        {
            ptr->decode(input);
            ptr = ptr->next;
        }
    }
    void setEncodeMap(vector<long> &enMapArr){
        int size = enMapArr.size();
        if (size%3 != 0)
            throw Error("invalid encoding scheme, not div3");
        for (size_t i = 0; i < size; i += 3)
        {
            encodeMap->insert({{i, i + 1}, i + 2});
        }
    }
    vector<long> getEncodeMap(){
        if (!encodeMap || encodeMap->empty())
            throw Error("empty encode map, not get able");
        vector<long> tmp{};
        for(const auto& e: *encodeMap){
            tmp.push_back(e.first.first);
            tmp.push_back(e.first.second);
            tmp.push_back(e.second);
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