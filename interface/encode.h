#ifndef ___ENCODE
#define ___ENCODE

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
