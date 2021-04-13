#include "rle.h"

Rle::Rle(Transform *next) : Transform(next){}
void Rle::transform(vector<unique_ptr<Block> > &input){
    for(const unique_ptr<Block>& line:input){
        applyTo(line->getData()); //transform the input
    }
}
void Rle::decode(vector<unique_ptr<Block>>& input){
    for(const unique_ptr<Block>& line: input){
        deplyTo(line->getData());
    }
}
void Rle::applyTo(vector<long>& data){

}
void Rle::deplyTo(vector<long> &data){
    
}