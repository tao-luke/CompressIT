#ifndef ___ZIP
#define ___ZIP
#include "transform.h"
class Zip: public Transform
{
    void transform(vector<unique_ptr<Block> > &input) override;
    
    public:
    Zip(Transform *next);

}

#endif