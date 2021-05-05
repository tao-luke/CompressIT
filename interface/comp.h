#ifndef ___COMP
#define ___COMP

#include "../input/input.h"
/**
 * Comp is a abstract super class that encapsulates the core program(encoding or decoding)
 * 
 * - members: 
 *  LOCAL:
 *     - m_isEncode: 1 for encode mode
 *     - info: useful string
 */
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
