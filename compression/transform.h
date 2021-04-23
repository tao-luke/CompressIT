#ifndef ___TRANSFORM
#define ___TRANSFORM

#include <memory>
#include "../input/block.h"
#include <map>
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
    map<pair<int, int>, long> encodeMap{};
public:
    Transform(Transform* next):next(next){}

    void run(vector<unique_ptr<Block> > & input){ //transform the input
        Transform* ptr = next;
        transform(input);
        while (ptr)
        {
            ptr->transform(input);
            ptr = ptr->next;
        }
    }
    void run2(vector<unique_ptr<Block> > & input){ //transform the input
        Transform* ptr = next;
        decode(input);
        while (ptr)
        {
            ptr->decode(input);
            ptr = ptr->next;
        }
    }
    void setEncodeMap(vector<long> &enMapArr){
        int size = enMapArr.size();
        if (size%3 != 0){} //! report error
        for (size_t i = 0; i < size; i+=3){
            encodeMap.insert({{i, i + 1}, i + 2});
        }
    }
    vector<long> &&getEncodeMap(){
        vector<long> tmp{};
        for(const auto& e: encodeMap){
            tmp.push_back(e.first.first);
            tmp.push_back(e.first.second);
            tmp.push_back(e.second);
        }
        return std::move(tmp);
    }
    virtual ~Transform(){
        delete next;
    };
};

#endif