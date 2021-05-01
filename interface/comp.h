#ifndef ___COMP
#define ___COMP
#include "../input/stdin.h"
#include <iostream>
#include "../compression/bwt.h"
#include "../compression/mtf.h"
#include "../compression/rle.h"
#include "../compression/huff.h"
#include "../output/ofile.h"
#include <vector>
class Comp{
    bool encode = true;
    vector<Transformation> Tseq{};
    string fileName = "";

//!usage:
// follow format ./CompressIt <mode> <transformation> < inputflow
// ex: ex: ./CompressIt -encode -bmr < test.txt 
// the above encodes test.txt from std::cin and encodes first applying burrows wheeler, 
//move-to-front, and then runlength encoding , and saving the file in huffman at the end.

public:
    string info = "follow format ./CompressIt <mode> <transformation> < inputflow";
    Comp(int c, char **argv)
    {
        if (c < 2 || c > 4)
            throw Error("improper usage," + info);
        if (string(argv[1]) == "-decode"){
            encode = false;
        }else if (string(argv[1]) != "-encode"){
            throw Error("invalid mode selected," + info + " , ex: ./CompressIt -encode -bmr < test.txt ");
        }
        if (encode){
            int counter = 1;
            while(argv[2][counter] != 0){
                if (argv[2][counter] == 'b')
                    Tseq.push_back(Transformation::BWT);
                else if (argv[2][counter] == 'm')
                    Tseq.push_back(Transformation::MTF);
                else if (argv[2][counter] == 'r')
                    Tseq.push_back(Transformation::RLE);
                else throw Error("invalid transformation selected," + info);
                counter++;
            }
        }

        int counter = 2;
        while(counter < c && argv[counter][0] != '-'){ //for future, if files are read in
            fileName.append(string(argv[counter]));
        }
    }
    void run(){
        unique_ptr<Input> input{new Stdin(encode)}; //create input (for now from std::cin)
        std::vector<std::unique_ptr<Block> > &data = input->getInputData(); //read the data
        if (encode){ //encoding data
            Transform *encoding = new Huff(nullptr); //generate transformation sequence
            int counter = Tseq.size() - 1;
            while (counter >= 0)
            {
                if (Tseq[counter] == Transformation::BWT){
                    encoding = new Bwt(encoding);
                }
                else if (Tseq[counter] == Transformation::MTF)
                {
                    encoding = new Mtf(encoding);
                }
                else if (Tseq[counter] == Transformation::RLE)
                {
                    encoding = new Rle(encoding);
                }
                counter--;
            }   
            encoding->execute(data); //execute encoding
            Ofile(data, encoding->getEncodeMap(), Tseq, input->getOriginalSize());// save to file
        }else{
            const vector<long> &transform_arr = data[0]->getData(); //encoding used 
            //data[0] = encoding sequqnce used
            //data[1] = huffman map used
            //data[2] = actual data
            Transform *decoding = nullptr; //generate the decoding sequence
            for (int i = 0; i < transform_arr.size(); i++)
            {
                if (transform_arr[i] == Transformation::BWT){
                    decoding = new Bwt(decoding);
                }
                else if (transform_arr[i] == Transformation::MTF)
                {
                    decoding = new Mtf(decoding);
                }
                else if (transform_arr[i] == Transformation::RLE)
                {
                    decoding = new Rle(decoding);
                }
                else
                    throw Error("bad transformation arr read");
            }
            decoding = new Huff(decoding);

            decoding->setEncode(false); //set mode to decoding
            decoding->setEncodeMap(data[1]->getData()); //set huffman encoding map
            vector<unique_ptr<Block> > decoded{};
            
            decoded.push_back(move(data[2]));
            data.pop_back();
            decoding->setEndValidBits(input->getEndVal()); //some bits are invalid at the end, set the val to read in
            decoding->execute(decoded);
            for(const auto& n: decoded[0]->getData()){
                cout << static_cast<char>(n);
            }
            cout  << endl << "complete" << endl;
        }
    }
};

#endif