#include "./input/stdin.h"
#include <iostream>
#include "./compression/bwt.h"
#include "./compression/mtf.h"
#include "./compression/rle.h"
#include "./compression/huff.h"
#include "./output/file.h"
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
    Transform *trans = new Bwt(new Mtf( new Rle( new Huff())));

    vector<Transformation> Tseq{Transformation::BWT, Transformation::MTF, Transformation::RLE};
    try{
        trans->run(data);
        File(data, trans->getEncodeMap(), Tseq, input->getOriginalSize());
    }catch(Error e){
        cout << e.what() << endl;
    }
    // trans->run2(data);
    // for (auto &&ptr : data)
    // {
    //     for(const auto& n: ptr->getData()){
    //         cout << n << " " << endl;
    //     }
    // }

    // dec->run2(data);
    // for (auto &&ptr : data)
    // {
    //     for(const auto& n: ptr->getData()){
    //         cout << n << " "<< static_cast<char>(n)  << endl;
    //     }
    // }
    delete trans;
}