#include "./input/stdin.h"
#include <iostream>
#include "./compression/bwt.h"
#include "./compression/mtf.h"
#include "./compression/rle.h"
#include "./compression/huff.h"
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
    Huff *trans = new Huff(nullptr);
    trans->run(data);
    for(auto && ptr: data){
        for(const auto& n: ptr->getData()){
            cout << n << " " << trans->decodeChar(n)<< endl;
            //! need a new data type in block to represent binary!
        }
    }

    // dec->run2(data);
    // for (auto &&ptr : data)
    // {
    //     for(const auto& n: ptr->getData()){
    //         cout << n << " "<< static_cast<char>(n)  << endl;
    //     }
    // }
    delete trans;
}