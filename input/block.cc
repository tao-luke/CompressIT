
#include "block.h"
std::vector<long>& Block::getData(){
    return m_data;
}
void Block::setData(std::vector<long> &&ref){
    m_data = std::move(ref);
}
void Block::insertToData(long num)
{
    m_data.push_back(num);
}
void Block::popEOT(){
    if (m_data.back() == 0)
        m_data.pop_back();
    else
        throw Error("tried to pop non-end eot char and saw instead: " + to_string(m_data.back()));
}
void Block::clearData()
{
    m_data.clear();
}
size_t Block::getDataSize()
{
    return m_data.size();
}