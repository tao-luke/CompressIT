#include "rle.h"
#include "math.h"
Rle::Rle(Transform *next) : Transform(next){}
void Rle::transform(vector<unique_ptr<Block> > &input){
    for(const unique_ptr<Block>& line:input){
        applyTo(line->getData()); //transform the input
    }
}
void Rle::decode(vector<unique_ptr<Block>>& input){
    for(const unique_ptr<Block>& line: input){
        deplyTo(line->getData());
    }
}
void Rle::applyTo(vector<long>& data){
    int pops = 0;
    int counter = 0;
    int n = 0;
    int size = data.size();
    int slow = 0;
    while (counter < size)
    {
        if (data[counter] == 0)
        {
            n++;
            counter++;
            continue;
        }
        if (n != 0){
            pops += n - bbnRep(data,n,slow);
            n = 0;
        }
        data[slow] = data[counter];
        slow++;
        counter++;
    }
    if (n != 0){
        pops += n - bbnRep(data,n,slow);
        n = 0;
    }
    while(pops != 0){
        data.pop_back();
        pops--;
    }
}
size_t Rle::bbnRep(vector<long>& data,int n ,int& slow){
    //convert to binary bijective numeration
    int counter = 0;
    int tmp = 0;
    while (true)
    {
        tmp = (n - 2 * (ceil(n /(double) 2.0) - 1));
        if (tmp == 1) data[slow] = 255;
        else data[slow] = 0;
        slow++;
        counter++;
        n = (ceil(n /(double) 2) - 1);
        if (n == 0)
            break;
    }
    return counter;
}
size_t Rle::longRep(vector<long>& data, int& index){
    //num the thing represents, returned
    //increment index to the one thats not good.
    size_t result = 0;
    int power = 0;
    while(data[index] == 0 || data[index] == -1){
        if (data[index] == 0){
            result += 2 * pow(2, power++);
        }
        else
        {
            result += pow(2, power++);
        }
        index++;
    }
    return result;
}
void Rle::deplyTo(vector<long> &data){
    //deocde a line
    vector<long> copy{};
    int tmp = 0;
    int size = data.size();
    for (int i = 0; i < size; i++){
        if (data[i] != 0 && data[i] != -1){
            copy.push_back(data[i]);
            continue;
        }
        tmp = longRep(data, i);
        while(tmp != 0){
            copy.push_back(0);
            tmp--;
        }
        i--;
    }
}