#include "lzw.h"
#include <iostream>

//Lempel-Ziv-Welch: suppose an input S, encode S using an adaptive dictionary for longer strings of characters

//Implementation:
// Encode is implemented using a trie, and decode using a hash map

// trie impl
Lzw::Node::Node(): k{0} {}
Lzw::Node::Node(unsigned int k): k{k} {}
Lzw::Node* Lzw::Node::add(unsigned int link, unsigned int k) {
  return (m[link] = unique_ptr<Lzw::Node>(new Node(k))).get();
}
Lzw::Node* Lzw::Node::getNext(unsigned int c) {
  return m.at(c).get();
}
unsigned int Lzw::Node::getKey() { return k; }
void Lzw::Node::print(int depth = 0) { // for debugging
  for (pair<unsigned int const, unique_ptr<Lzw::Node>> &x:m) {
    for (int i = 0; i < depth; ++i) cout << " ";
    cout << x.first << " -> " << x.second->getKey() << endl;
    x.second->print(depth + 1);
  }
}

Lzw::Dict::Dict(unsigned int size): id{size}, r{new Node()} {}
Lzw::Node* Lzw::Dict::add(Node* n, unsigned int c) {
  if (n == r.get()) {
    return n->add(c, c);
  } else {
    return n->add(c, id++);
  }
}
Lzw::Node* Lzw::Dict::root() { return r.get(); }
unsigned int Lzw::Dict::nextId() { return id; }
void Lzw::Dict::print() { r->print(); } // for debugging
// end trie impl


Lzw::Lzw(Transform *next) : Transform(next) {}

void Lzw::transform(vector<unique_ptr<Block> > &input){
  Dict dict = Dict(Transform::m_alphabetSize); // create empty dictionary
  
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
        if (n != dict.root()) { // add key to new array
          newData.push_back(n->getKey());
        }

        dict.add(n, line->getData()[i]); // add to dictionary
      }
    }
    line->setData(std::move(newData)); // update data
  }

  // update alphabet size for later transformations
  Transform::m_alphabetSize = dict.nextId();
}

void Lzw::decode(vector<unique_ptr<Block>>& input) {
  unsigned int id = Transform::m_alphabetSize; // first available id

  unordered_map<unsigned int, pair<unsigned int, unsigned int>> m; // lookup map
  for (unique_ptr<Block>& line:input) {
    line->setData(decodeLine(line->getData(), id, m));
  }
}

vector<long> Lzw::decodeLine(const vector<long>& line, unsigned int& id, unordered_map<unsigned int, pair<unsigned int, unsigned int>>& m) {
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

vector<long> Lzw::decodeLookup(unsigned int code, unordered_map<unsigned int, pair<unsigned int, unsigned int>>& m) {
  // build string from code and lookup map
  vector<long> res;
  while(code >= Transform::m_alphabetSize) {
    pair<unsigned int, unsigned int> p = m[code];
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
