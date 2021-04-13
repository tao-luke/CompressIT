#ifndef ___RLE
#define ___RLE
#include "transform.h"
class Rle: public Transform
{
    void transform(vector<unique_ptr<Block> > &input) override;
    void applyTo(vector<long>& data) override;
    void decode(vector<unique_ptr<Block> > &input) override;
    void deplyTo(vector<long> &data) override;

public:
    Rle(Transform *next);
};

#endif