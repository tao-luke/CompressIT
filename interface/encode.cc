#include "encode.h"
#include "../global.h"
#include "../compression/transform.h"
#include "../compression/bwt.h"
#include "../compression/mtf.h"
#include "../compression/rle.h"
#include "../compression/huff.h"
#include "../output/ofile.h"
#include "../input/input.h"

Encode::Encode(char* flags): Comp{true} {
  int counter = 1;
  while(flags[counter] != 0){
    if (flags[counter] == 'b')
      Tseq.push_back(Transformation::BWT);
    else if (flags[counter] == 'm')
      Tseq.push_back(Transformation::MTF);
    else if (flags[counter] == 'r')
      Tseq.push_back(Transformation::RLE);
    else throw Error("invalid transformation selected," + info);
    counter++;
  }
}

void Encode::run(Input* input) {
  std::vector<std::unique_ptr<Block> > &data = input->getInputData(); //read the data
  
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
}