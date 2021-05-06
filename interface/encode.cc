#include "encode.h"
#include "../global.h"
#include "../compression/transform.h"
#include "../compression/bwt.h"
#include "../compression/mtf.h"
#include "../compression/rle.h"
#include "../compression/lzw.h"
#include "../compression/huff.h"
#include "../output/ofile.h"
#include "../input/input.h"

Encode::Encode(char* flags, vector<string>&fileNames) : Comp{true}, fileNames(fileNames) {
  int counter = 1;
  while(flags && flags[counter] != 0) {
    if (flags[counter] == 'b')
      Tseq.push_back(Transformation::BWT);
    else if (flags[counter] == 'm')
      Tseq.push_back(Transformation::MTF);
    else if (flags[counter] == 'r')
      Tseq.push_back(Transformation::RLE);
    else if (flags[counter] == 'l')
      Tseq.push_back(Transformation::LZW);
    else throw Error("invalid transformation selected," + info);
    counter++;
  }
}

void Encode::run(Input* input) {
  std::vector<std::unique_ptr<Block> > &data = input->getInputData(); //read the data
  
  Transform *encoding = new Huff(nullptr); //generate transformation sequence
  
  int counter = Tseq.size() - 1;
  while (counter >= 0) {
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
    else if (Tseq[counter] == Transformation::LZW)
    {
      encoding = new Lzw(encoding);
    }
    counter--;
  }

  encoding->execute(data); //execute encoding
  if (fileNames.empty())
        fileNames.push_back(string("noName")); //standize a name if not given
  Ofile(data, encoding->getEncodeMap(), Tseq, input->getOriginalSize(),fileNames[0].data(),fileNames[0].size());// save to file
  delete encoding;
}