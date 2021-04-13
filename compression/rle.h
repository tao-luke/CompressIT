#ifndef ___RLE
#define ___RLE
#include "transform.h"
class Rle: public Transform
{
    void transform(vector<unique_ptr<Block> > &input) override;
    void applyTo(vector<long>& data) override;
    void decode(vector<unique_ptr<Block> > &input) override;
    void deplyTo(vector<long> &data) override;
    size_t bbnRep(vector<long> &data, int n , int& slow);
    void longRep(vector<long> &data, int& index);

public:
    Rle(Transform *next);
};

#endif