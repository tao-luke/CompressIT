#include "lzw.h"
#include <iostream>

//Lempel-Ziv-Welch: suppose an input S, encode S using an adaptive dictionary for longer strings of characters
// given some text input S, suppose some patterns of text appears, BWT ensures a reversible text
// that has lots of high runlength texts. ex: aaabsbdbbbbbb has a long runs of a and b

//Implementation:
// generate suffix array , O(n)
// worst case: O(nlog^2n)
//expected O(n)

// Dictionary Trie Implementation:
Lzw::Node::Node(): k{0} {}
Lzw::Node::Node(unsigned int k): k{k} {}
Lzw::Node* Lzw::Node::add(char link, unsigned int k) {
  return (m[link] = unique_ptr<Lzw::Node>(new Node(k))).get();
}
Lzw::Node* Lzw::Node::getNext(char c) {
  return m.at(c).get();
}
unsigned int Lzw::Node::getKey() { return k; }
void Lzw::Node::print(int depth = 0) {
  for (pair<char const, unique_ptr<Lzw::Node>> &x:m) {
    for (int i = 0; i < depth; ++i) cout << " ";
    cout << x.first << " -> " << x.second->getKey() << endl;
    x.second->print(depth + 1);
  }
}

Lzw::Dict::Dict(unsigned int size): id{size}, r{new Node()} {}
Lzw::Node* Lzw::Dict::add(Node* n, char c) {
  if (n == r.get()) {
    return n->add(c, c);
  } else {
    return n->add(c, id++);
  }
}
Lzw::Node* Lzw::Dict::root() { return r.get(); }
unsigned int Lzw::Dict::nextId() { return id; }
void Lzw::Dict::print() { r->print(); }

Lzw::Lzw(Transform *next) : Transform(next) {}
void Lzw::transform(vector<unique_ptr<Block> > &input){
  Dict dict = Dict(Transform::m_alphabetSize);
  Node* n = dict.root();
  for(const unique_ptr<Block>& line:input){
    vector<long> newData;
    unsigned int i = 0;

    while(i < line->getDataSize()) {
      try { // find longest match
        n = dict.root();
        while (1) {
          n = n->getNext(line->getData()[i]);
          ++i;
        }
      } catch (...) { // does not exist
        if (n != dict.root()) {
          newData.push_back(n->getKey());
        }

        dict.add(n, line->getData()[i]);
      }
    }
    line->setData(std::move(newData));
  }

  Transform::m_alphabetSize = dict.nextId();
}
void Lzw::decode(vector<unique_ptr<Block>>& input) {
  unsigned int id = Transform::m_alphabetSize;
  unordered_map<unsigned int, pair<unsigned int, char>> m;
  for (unique_ptr<Block>& line:input) {
    line->setData(decodeLine(line->getData(), id, m));
  }
}
vector<long> Lzw::decodeLine(const vector<long>& line, unsigned int& id, unordered_map<unsigned int, pair<unsigned int, char>>& m) {
  vector<long> newData;
  unsigned int i = 0;

  long code = line[i++];
  vector<long> str = decodeLookup(code, m);
  newData.push_back(str[0]);

  while (i < line.size()) {
    unsigned int prevcode = code;
    code = line[i];

    if (code < id) {
      str = decodeLookup(code, m);
    } else if (code == id) {
      str.push_back(str[0]);
    } else {
      throw new Error("invalid encoding");
    }

    for (auto x : str) newData.push_back(x);
    m[id++] = {prevcode, str[0]};
    ++i;
  }

  return newData;
}
vector<long> Lzw::decodeLookup(unsigned int code, unordered_map<unsigned int, pair<unsigned int, char>>& m) {
  vector<long> res;
  while(code >= Transform::m_alphabetSize) {
    pair<unsigned int, char> p = m[code];
    code = p.first;
    res.insert(res.begin(), p.second);
  }

  try {
    res.insert(res.begin(), m.at(code).second);
  } catch(...) {
    m[code] = {0, code};
    res.insert(res.begin(), code);
  }

  return res;
}
void Lzw::applyTo(vector<long>& data) {}
