#ifndef ___ENCODE_INTERFACE
#define ___ENCODE_INTERFACE

#include "comp.h"
#include <vector>

class Encode : public Comp {
  vector<Transformation> Tseq{};
  vector<string> &fileNames;

public:
  Encode(char*,vector<string>& fileNames);
  void run(Input*) override;
};

#endif
