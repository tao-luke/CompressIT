#include "./comp.h"

    
Comp::Comp(int c, char **argv) {
  string info = "follow format ./CompressIt <mode> <transformation> < inputflow";
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
  while(counter < c){ //for future, if files are read in
	if (argv[counter][0] != '-')
	fileNames.push_back(string(argv[counter]));
	counter++;
  }
}

void Comp::run() {
  Input *rawptr = nullptr;

  if (fileNames.empty()){ //decide between from input stream or from file stream
      rawptr = new Stdin(encode);
  }else{
      rawptr = new Ifile(encode, fileNames);
  }
  
  unique_ptr<Input> input{rawptr}; 
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
      if (fileNames.empty())
        fileNames.push_back(string("noName")); //standize a name if not given
      Ofile(data, encoding->getEncodeMap(), Tseq, input->getOriginalSize(),fileNames[0].data(),fileNames[0].size());// save to file
  }
  else
  {
    const vector<long> &transform_arr = data[0]->getData(); //encoding used
    //data[0] = encoding sequqnce used
    //data[1] = huffman map used
    //data[2] = actual data
    Transform *decoding = nullptr; //generate the decoding sequence
    for (int i = 0; i < transform_arr.size(); i++)
    {
      if (transform_arr[i] == Transformation::BWT)
      {
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

    decoding->setEncode(false);                 //set mode to decoding
    decoding->setEncodeMap(data[1]->getData()); //set huffman encoding map
    vector<unique_ptr<Block> > decoded{};

    decoded.push_back(move(data[2]));
    data.pop_back();
    decoding->setEndValidBits(input->getEndVal()); //some bits are invalid at the end, set the val to read in
    decoding->execute(decoded);
    Ofile(decoded[0]->getData(), string("test.raw").data()); // save to file
    cout << endl << "complete,saved to file test.raw" << endl;
  }
}