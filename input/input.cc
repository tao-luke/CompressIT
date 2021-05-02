#include "input.h"

void Input::run(bool c){
  if (c)
    read();
  else
    decodeRead();
}

unsigned char Input::getEndVal(){
  return endValidBit;
}

unsigned long Input::getOriginalSize(){
  return original;
}

std::vector<std::unique_ptr<Block>>& Input::getInputData(){
  return inputData;
}

void Input::insertToData(std::unique_ptr<Block>&& ptr){
  original += ptr->getDataSize();
  inputData.push_back(std::move(ptr));
}