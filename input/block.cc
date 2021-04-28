
#include "block.h"
std::vector<long>& Block::getData(){
    return data;
}
void Block::setData(std::vector<long> &&ref){
    data = std::move(ref);
}
void Block::insertToData(long num)
{
    data.push_back(num);
}
void Block::popEOT(){
    if (data.back() == 0)
        data.pop_back();
        //! add error case
}
void Block::clearData()
{
    data.clear();
}
size_t Block::getDataSize()
{
    return data.size();
}