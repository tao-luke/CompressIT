#ifndef ___LZW
#define ___LZW

#include <map>
#include <initializer_list>
#include <memory>
#include <vector>
#include <unordered_map>
#include "../compression/transform.h"

using namespace std;

class Lzw: public Transform
{
  class Node {
    map<char, unique_ptr<Node>> m;
    unsigned int k;

  public:
    Node();
    Node(unsigned int);
    Node* add(char, unsigned int);
    Node* getNext(char);
    void print(int);
    unsigned int getKey();
  };

  class Dict {
    unsigned int id = 0;
    unique_ptr<Node> r;

  public:
    Dict(unsigned int);
    Node* add(Node*, char);
    Node* root();
    void print();
  };

  const unsigned int alphabetSize = 128;

  void transform(vector<unique_ptr<Block> >&) override;
  void applyTo(vector<long>& data) override;
  void decode(vector<unique_ptr<Block> >&) override;

  vector<long> decodeLine(const vector<long>&, unsigned int&, unordered_map<unsigned int, pair<unsigned int, char> >&);
  vector<long> decodeLookup(unsigned int, unordered_map<unsigned int, pair<unsigned int, char> >&);

public:
    Lzw(Transform*);
};

#endif