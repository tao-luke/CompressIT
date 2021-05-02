#include "lzw.h"
#include <stdexcept>
#include <iostream>

//Lempel-Ziv-Welch: suppose an input S, encode S using an adaptive dictionary for longer strings of characters
// given some text input S, suppose some patterns of text appears, BWT ensures a reversible text
// that has lots of high runlength texts. ex: aaabsbdbbbbbb has a long runs of a and b

//Implementation:
// generate suffix array , O(n)
// worst case: O(nlog^2n)
//expected O(n)

// Dictionary Trie Implementation:

Lzw::Node::Node() {}
Lzw::Node::Node(unsigned int k): k{k} {}
// Lzw::Node::Node(std::initializer_list<std::pair<char const, Lzw::Node*>> init): m{init} {}
Lzw::Node* Lzw::Node::add(char link, unsigned int k) {
  return (m[link] = std::unique_ptr<Lzw::Node>(new Node(k))).get();
}
Lzw::Node* Lzw::Node::getNext(char c) {
  return m.at(c).get();
}
unsigned int Lzw::Node::getKey() { return k; }
void Lzw::Node::print(int depth = 0) {
  for (std::pair<char const, std::unique_ptr<Lzw::Node>> &x:m) {
    if ((depth != 0 || !x.second->m.empty()) && x.first >= 32) {
      for (int i = 0; i < depth; ++i) std::cout << " ";
      std::cout << x.first << " -> " << x.second->getKey() << std::endl;
      x.second->print(depth + 1);
    }
  }
}

Lzw::Dict::Dict(): r{new Node()} {
  for (int i = 0; i < 128; ++i) {
    r->add(i, id++);
  }
}
Lzw::Node* Lzw::Dict::add(Node* n, char c) { return n->add(c, id++); }
Lzw::Node* Lzw::Dict::root() { return r.get(); }
void Lzw::Dict::print() { r->print(); }

Lzw::Lzw(Transform *next) : Transform(next){};

void Lzw::transform(vector<unique_ptr<Block> > &input){
  std::cout << "LZW" << std::endl;

  vector<long> newData;
  Node* n = dict.root();
  for(const unique_ptr<Block>& line:input){
    unsigned int i = 0;
    while(i < line->getDataSize()) {
      try {
        n = n->getNext(line->getData()[i]);
        ++i;
      } catch (...) {
        newData.push_back(n->getKey());

        dict.add(n, line->getData()[i]);
        n = dict.root();
      }
    }

    for (auto x:newData) std::cout << x << " ";
    std::cout << std::endl;
  }
  input.clear();
  input.push_back(unique_ptr<Block>(new Block(std::move(newData))));

  dict.print();
}
void Lzw::applyTo(vector<long>& data){
}
void Lzw::decode(vector<unique_ptr<Block>>& input){
}