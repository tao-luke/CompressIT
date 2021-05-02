#ifndef ___COMP
#define ___COMP

#include "../input/input.h"

class Comp {
  const bool isEncode;

protected:
  string info = "follow format ./CompressIt <mode> <transformation> < inputflow";

public:
  Comp(bool e): isEncode{e} {}
  bool encode() { return isEncode; };

  virtual ~Comp() {};
  virtual void run(Input*) = 0;
};

#endif
