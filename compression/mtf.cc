#include "mtf.h"

Mtf::Mtf(Transform* next):Transform(next){
}

void Mtf::transform(vector<unique_ptr<Block> > &input){
        cout << "before target debuf" << endl;
        int counter = 0;
        for (const auto &p : input)
        {
            for (const auto &c : p->getData())
            {
                cout << c << " ";
                counter++;
            }
    }
    cout << "size is: " << counter << endl;
    for (const unique_ptr<Block> &line : input)
    {
        applyTo(line->getData()); //transform the input
    }
}
void Mtf::decode(vector<unique_ptr<Block>>& input){
    // cout << " bef target debug" << endl;
    // for (const auto &p : input)
    // {
    //     for(const auto& c: p->getData()){
    //         cout << c << " ";
    //     }
    // }
    for(const unique_ptr<Block>& line: input){
        deplyTo(line->getData());
    }
    cout << "target debug" << endl;
    int counter = 0;
    for (const auto &p : input)
    {
        for(const auto& c: p->getData()){
            cout << c << " ";
            counter++;
        }
    }
    cout << " size is: " << counter << endl;
}
void Mtf::applyTo(vector<long>& data){
    vector<long> ascii(256,0);
    vector<long> copy = data;
    for (int i = 0; i < 256; i++)
    {
        ascii[i] = i;
    } //init encode arr
    int counter = 0;
    for (const auto &c : copy)
    {
        data[counter++] = moveToFront(ascii, c);
    }
}
void Mtf::deplyTo(vector<long> &data){
    vector<long> ascii(256,0);
    vector<long> copy = data;
    for (int i = 0; i < 256; i++)
    {
        ascii[i] = i;
    }//init encoder arr
    int counter = 0;
    for(const auto& c: copy){
        data[counter++] = ascii[c];
        for (int i = c; i >= 1; i--){
            swap(ascii[i], ascii[i - 1]);
        }
    }
    
}
int Mtf::moveToFront(vector<long> &data, long target){
    int result = 0;
    for (const auto &n : data)
    {
        if (n == target)
            break;
        result++;
    }
    for (int i = result; i >= 1;i--){
        swap(data[i], data[i - 1]);
    }
    return result;
}
