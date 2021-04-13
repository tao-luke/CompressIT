#include "./input/stdin.h"
#include <iostream>
#include "./compression/bwt.h"
#include "./compression/mtf.h"
using namespace std;
int main(){

    unique_ptr<Input> input{new Stdin()};
    std::vector<std::unique_ptr<Block> > &data = input->getInputData();
    for(auto && ptr: data){
        for(const auto& n: ptr->getData()){
            cout << n << " " <<static_cast<char>(n) << endl;
        }
    }
    cout << "...." << endl;
    Transform *trans = new Bwt(new Mtf(nullptr));
    trans->run(data);

    Transform *dec = new Mtf(new Bwt(nullptr));
    dec->run2(data);
    for (auto &&ptr : data)
    {
        for(const auto& n: ptr->getData()){
            cout << n << " " <<static_cast<char>(n) << endl;
        }
    }
    delete trans;
    delete dec;
}