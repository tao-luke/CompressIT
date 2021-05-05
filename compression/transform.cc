#include "transform.h"

unsigned int Transform::m_alphabetSize = 256;

void Transform::run(vector<unique_ptr<Block> > & input){ //transform the input
    initEncodeMap();
    if (input.empty())
        throw Error("empty encoding input string");
    Transform *ptr = m_next;
    transform(input);
    while (ptr)
    {
        ptr->transform(input);
        ptr = ptr->m_next;
    }
}
void Transform::run2(vector<unique_ptr<Block> > & input){ //transform the input
    if (input.empty())
        throw Error("empty decoding input string");
    Transform* p = m_next;
    decode(input);
    while (p)
    {
        p->decode(input);
        p = p->m_next;
    }
    input[0]->popEOT();
}
void Transform::initEncodeMap(){
    m_encodeMap = new map<pair<long, unsigned char>, unsigned char>{};
    Transform *ptr = m_next;
    while(ptr){ //propegate the map to the m_nexts
        ptr->m_encodeMap = m_encodeMap;
        ptr = ptr->m_next;
    }
}

void Transform::setEncodeMap(const vector<long> &enMapArr){  //create a mapping from some input arr
    int size = enMapArr.size();
    initEncodeMap();
    if (size % 3 != 0)
        throw Error("invalid encoding scheme, not div3");
    for (size_t i = 0; i < size; i += 3)
    {
        m_encodeMap->insert({{enMapArr[i+2], enMapArr[i+ 1] }, enMapArr[i]});
    }
}
vector<long> Transform::getEncodeMap()
{ //generate a arr representing  a huff mapping.
    if (!m_encodeMap || m_encodeMap->empty())
        throw Error("empty encode map, not get able");
    vector<long> tmp{};
    for(const auto& e: *m_encodeMap){
        tmp.push_back(e.second);
        tmp.push_back(e.first.second);
        tmp.push_back(e.first.first);
    }
    return tmp;
}