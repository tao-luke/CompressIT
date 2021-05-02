#include "decode.h"
#include "../global.h"
#include "../compression/transform.h"
#include "../compression/bwt.h"
#include "../compression/mtf.h"
#include "../compression/rle.h"
#include "../compression/huff.h"
#include "../input/input.h"
#include "../output/ofile.h"
Decode::Decode(): Comp{false} {}

void Decode::run(Input* input) {
  std::vector<std::unique_ptr<Block> > &data = input->getInputData(); //read the data
  
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
    Ofile(decoded[0]->getData(), string("test.raw").data()); // save to file
    //! get filename from input!
    cout << endl << "complete,saved to file test.raw" << endl;
}