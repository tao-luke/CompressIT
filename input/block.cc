
#include "block.h"
std::vector<long>& Block::getData(){
    return data;
}

std::unique_ptr<Block> && Block::getDataBlock()
{
    return std::unique_ptr<Block>(new Block(std::move(data)));
}
void Block::insertToData(long num)
{
    data.push_back(num);
}

void Block::clearData()
{
    data.clear();
}
size_t Block::getDataSize()
{
    return data.size();
}