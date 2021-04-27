#include "./input/stdin.h"
#include <iostream>
#include "./compression/bwt.h"
#include "./compression/mtf.h"
#include "./compression/rle.h"
#include "./compression/huff.h"
#include "./output/ofile.h"
using namespace std;
int main(){

    unique_ptr<Input> input{new Stdin(0)};
    // std::vector<std::unique_ptr<Block> > &data = input->getInputData();
    // for(auto && ptr: data){
    //     for(const auto& n: ptr->getData()){
    //         cout << n << " " <<static_cast<char>(n) << endl;
    //     }
    // }
    // cout << "...." << endl;
    // Transform *trans = new Bwt(new Mtf( new Rle( new Huff())));

    // vector<Transformation> Tseq{Transformation::BWT, Transformation::MTF, Transformation::RLE};
    // try{
    //     trans->run(data);
    //     Ofile(data, trans->getEncodeMap(), Tseq, input->getOriginalSize());
    // }catch(Error e){
    //     cout << e.what() << endl;
    // }


//decode
    std::vector<std::unique_ptr<Block> > &data = input->getInputData();
    const vector<long> &transform_arr = data[0]->getData();
    Transform *trans = nullptr;
    for (int i = 0; i < transform_arr.size(); i++)
    {
        if (transform_arr[i] == Transformation::BWT){
            trans = new Bwt(trans);
        }
        else if (transform_arr[i] == Transformation::MTF)
        {
            trans = new Mtf(trans);
        }
        else if (transform_arr[i] == Transformation::RLE)
        {
            trans = new Rle(trans);
        }
        else
            throw Error("bad transformation arr read");
    }
    trans = new Huff(trans);


    trans->setEncodeMap(data[1]->getData());
    vector<unique_ptr<Block> > decoded{};
    decoded.push_back(data[2]->getDataBlock());
    cout << decoded[0]->getDataSize() << endl;
    trans->run2(decoded);
    for (auto &&ptr : data)
    {
        for(const auto& n: ptr->getData()){
            cout << n << " " << endl;
        }
    }

    delete trans;
}