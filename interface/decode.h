#ifndef ___DECODE_INTERFACE
#define ___DECODE_INTERFACE

#include "comp.h"

class Decode : public Comp {
public:
  Decode();
  void run(Input*) override;
};

#endif
