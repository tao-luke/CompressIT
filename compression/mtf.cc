#include "mtf.h"

Mtf::Mtf(Transform* next):Transform(next){
    ascii = vector<long>(256, 0);
    for (int i = 0; i < 256; i++)
    {
        ascii[i] = i;
    } //init encode arr
}

void Mtf::transform(vector<unique_ptr<Block> > &input){
    for (const unique_ptr<Block> &line : input)
    {
        applyTo(line->getData()); //transform the input
    }
}
void Mtf::decode(vector<unique_ptr<Block>>& input){
    for(const unique_ptr<Block>& line: input){
        deplyTo(line->getData());
    }
}
void Mtf::applyTo(vector<long>& data){
    vector<long> copy = data;
    int counter = 0;
    for (const auto &c : copy)
    {
        data[counter++] = moveToFront(c);
    }
}
void Mtf::deplyTo(vector<long> &data){
    vector<long> copy = data;
    int counter = 0;
    for(const auto& c: copy){
        data[counter++] = ascii[c];
        for (int i = c; i >= 1; i--){
            swap(ascii[i], ascii[i - 1]);
        }
    }
    
}
int Mtf::moveToFront(long target){
    int result = 0;
    for (const auto &n : ascii)
    {
        if (n == target)
            break;
        result++;
    }
    for (int i = result; i >= 1;i--){
        swap(ascii[i], ascii[i - 1]);
    }
    return result;
}
