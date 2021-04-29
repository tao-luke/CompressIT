#include "./input/stdin.h"
#include <iostream>
#include "./compression/bwt.h"
#include "./compression/mtf.h"
#include "./compression/rle.h"
#include "./compression/huff.h"
#include "./output/ofile.h"
using namespace std;
int main(){

    unique_ptr<Input> input{new Stdin(1)};
// encode
    std::vector<std::unique_ptr<Block> > &data = input->getInputData();
    // for(auto && ptr: data){
    //     for(const auto& n: ptr->getData()){
    //         cout << n << " " <<static_cast<char>(n) << endl;
    //     }
    // }
    cout << "...." << endl;
    Transform *trans = new Bwt(new Mtf( new Rle( new Huff())));

    vector<Transformation> Tseq{Transformation::BWT, Transformation::MTF, Transformation::RLE};
    try{
        trans->execute(data);
        Ofile(data, trans->getEncodeMap(), Tseq, input->getOriginalSize());
    }catch(Error e){
        cout << e.what() << endl;
    }


//decode
// try{
//     unique_ptr<Input> input{new Stdin(0)};
//     std::vector<std::unique_ptr<Block> > &data = input->getInputData();
//     const vector<long> &transform_arr = data[0]->getData();
//     Transform *trans = nullptr;
//     for (int i = 0; i < transform_arr.size(); i++)
//     {
//         if (transform_arr[i] == Transformation::BWT){
//             trans = new Bwt(trans);
//         }
//         else if (transform_arr[i] == Transformation::MTF)
//         {
//             trans = new Mtf(trans);
//         }
//         else if (transform_arr[i] == Transformation::RLE)
//         {
//             trans = new Rle(trans);
//         }
//         else
//             throw Error("bad transformation arr read");
//     }
//     trans = new Huff(trans);
//     trans->setEncode(false); //set mode to decoding

//     trans->setEncodeMap(data[1]->getData());
//     vector<unique_ptr<Block> > decoded{};
    
//     decoded.push_back(move(data[2]));
//     data.pop_back();

//     trans->execute(decoded);

//     for(const auto& n: decoded[0]->getData()){
//         cout << static_cast<char>(n);
//     }
//     cout  << endl << "complete" << endl;
//     //delete trans;
// }catch(Error oops){
//     cout << oops.what() << endl;
// }
}

//! dont forget we hardcoded some stuff!
//! wack bug at huffman