#ifndef ___LZW
#define ___LZW

#include <map>
#include <initializer_list>
#include <memory>
#include <vector>
#include "../compression/transform.h"

class Lzw: public Transform
{
  class Node {
    std::map<char, std::unique_ptr<Node>> m;
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
    std::unique_ptr<Node> r;

  public:
    Dict();
    Node* add(Node*, char);
    Node* root();
    void print();
  };

  Dict dict;

  void transform(std::vector<std::unique_ptr<Block> >&) override;
  void applyTo(vector<long>& data) override;
  void decode(std::vector<std::unique_ptr<Block> >&) override;

public:
    Lzw(Transform*);
};

#endif