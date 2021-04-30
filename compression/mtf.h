#ifndef ___MTF
#define ___MTF
#include "transform.h"
class Mtf: public Transform
{
    vector<long> ascii;
    void transform(vector<unique_ptr<Block> > &input) override;
    void applyTo(vector<long>& data) override;
    void decode(vector<unique_ptr<Block> > &input) override;
    void deplyTo(vector<long> &data);
    int moveToFront(long target);

public:
    Mtf(Transform *next);
};

#endif