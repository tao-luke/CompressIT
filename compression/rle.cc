#include "rle.h"
#include "math.h"
//Run length Encoding: suppose an input S that has long runs of 0, we create text S2 where the run ons are shortened.
// Implementation: we use 2s bijective numeration to represent run length of 0. suppose 000000......n, we represent it as bnnRep(n). the convetion
// for this Bbn is using 0 and 256 as the alphabet.
// 
// we traverse through the array once, generating constant time operation for 0s, O(n)
Rle::Rle(Transform *next) : Transform(next){}

void Rle::reserveAlphabet(const vector<long> & data){
    for(const auto& n: data){
        if (n == Transform::getBijective1()){
            m_alphabetSize++;
            Transform::setBijective1(m_alphabetSize - 1);
            break;
        }
    }
}

void Rle::transform(vector<unique_ptr<Block> > &input){ //functionally tested
    reserveAlphabet(input[0]->getData());

    vector<long> before{input[0]->getData()};
    for (const unique_ptr<Block> &line : input)
    {
        applyTo(line->getData()); //transform the input
    }
}

void Rle::decode(vector<unique_ptr<Block>>& input){
    vector<long> copy{};

    for (const unique_ptr<Block> &line : input)
    {
        copy.clear();
        deplyTo(line->getData(), copy);
        line->setData(std::move(copy));
    }
    // int zeroC = 0;
    // for (auto &n : input[0]->getData())
    // {
    //     if (n== 0)
    //         zeroC++;
    //     cout << n << " ";
    // }
    // cout << input[0]->getData().size() << "total 0 count is: " << zeroC << endl;
}
void Rle::applyTo(vector<long>& data){
    unsigned int pops = 0;
    unsigned int counter = 0;
    unsigned int n = 0;
    unsigned int size = data.size();
    unsigned int slow = 0;
    while (counter < size)
    {
        if (data[counter] == 0)
        {
            n++;
            counter++;
            continue;
        }
        if (n != 0){ //if we read some 0s, //convert to Bijective numeration
            pops += n - bbnRep(data,n,slow); 
            n = 0;
        }
        data[slow] = data[counter];
        slow++;
        counter++;
    }
    if (n != 0){ //normal case
        pops += n - bbnRep(data,n,slow);
        n = 0;
    }
    while(pops != 0){
        data.pop_back();
        pops--;
    }
}
size_t Rle::bbnRep(vector<long>& data,unsigned int n ,unsigned int& slow){
    //convert to binary bijective numeration
    unsigned int counter = 0;
    unsigned int tmp = 0;
    while (true)
    {
        tmp = (n - 2 * (ceil(n /(double) 2.0) - 1));
        if (tmp == 1) data[slow] = Transform::getBijective1();
        else data[slow] = 0;
        slow++;
        counter++;
        n = (ceil(n /(double) 2) - 1);
        if (n == 0)
            break;
    }
    return counter;
}
size_t Rle::longRep(vector<long>& data, unsigned int& index){
    //num the thing represents, returned
    //increment index to the one thats not good.
    size_t result = 0;
    int power = 0;
    while(data[index] == 0 || data[index] == Transform::getBijective1()){
        if (data[index] == 0){
            result += 2 * pow(2, power++);
        }
        else
        {
            result += pow(2, power++);
        }
        index++;
        if (index >= data.size())
            break;
    }
    return result;
}
void Rle::deplyTo(vector<long> &data,vector<long>& copy){
    //deocde a line
    unsigned int tmp = 0;
    unsigned int size = data.size();
    for (unsigned int i = 0; i < size; i++){
        if (data[i] != 0 && data[i] != Transform::getBijective1()){
            copy.push_back(data[i]);
            continue;
        }
        tmp = longRep(data, i);
        while (tmp != 0)
        {
            copy.push_back(0);
            tmp--;
        }
        i--;
    }

}