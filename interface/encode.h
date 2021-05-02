#ifndef ___ENCODE
#define ___ENCODE

#include "comp.h"
#include <vector>

class Encode : public Comp {
  vector<Transformation> Tseq{};

public:
  Encode(char*);
  void run(Input*) override;
};

#endif
