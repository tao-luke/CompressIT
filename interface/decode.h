#ifndef ___DECODE
#define ___DECODE

#include "comp.h"

class Decode : public Comp {
public:
  Decode();
  void run(Input*) override;
};

#endif
