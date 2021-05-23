#include "mtf.h"

//Move to front, suppose an input S where there is a imbalance of frquency, create run ons of 0
//Implementation: O(n) keesp track of a adaptive vector, and moves accordingly.
Mtf::Mtf(Transform* next):Transform(next){
}
void Mtf::initAsciiDict(){
    ascii = vector<long>(Transform::m_alphabetSize, 0);
    for (unsigned long i = 0; i < Transform::m_alphabetSize; i++)
    {
        ascii[i] = i;
    } //init encode arr
}
void Mtf::transform(vector<unique_ptr<Block> > &input){
    initAsciiDict();
    int zeroC = 0;
    for(auto& n: input[0]->getData()){
        if (n==257){
            zeroC++;
        }
    }
    cout << " #of 257 reaD bef mtf: " << zeroC << endl;
    for (const unique_ptr<Block> &line : input)
    {
        applyTo(line->getData()); //transform the input
    }
    zeroC = 0;
    for(auto& n: input[0]->getData()){
        if (n==257){
            zeroC++;
        }
    }
    cout << " #of 257 reaD after mtf: " << zeroC << endl;
}
void Mtf::decode(vector<unique_ptr<Block>>& input){
        int zeroC = 0;
    for(auto& n: input[0]->getData()){
        if (n==257){
            zeroC++;
        }
    }
    cout << " #of 257 reaD bef mtf: " << zeroC << endl;
    initAsciiDict();
    for(const unique_ptr<Block>& line: input){
        deplyTo(line->getData());
    }
    zeroC = 0;
    for(auto& n: input[0]->getData()){
        if (n==257){
            zeroC++;
        }
    }
    cout << " #of 257 reaD after mtf: " << zeroC << endl;
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
    unsigned int counter = 0;
    for(const auto& c: copy){
        data[counter++] = ascii[c];
        for (unsigned int i = c; i >= 1; i--){
            swap(ascii[i], ascii[i - 1]);
        } //! something wrong here, produced 22 257????
    }
    
}
int Mtf::moveToFront(long target){
    int result = 0;
    // cout << "Tried mtf with: " << target << endl;
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
