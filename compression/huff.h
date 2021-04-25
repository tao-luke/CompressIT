#ifndef ___HUFF
#define ___HUFF
#include "transform.h"
#include "string"

class Node{//superclass node 
	pair<Node *, int> parent;  //each node has a parent and a "link" from the parent to the child, so the pair.second would be the link
	int freq;

public:
	Node(pair<Node *, int> parent,int freq):parent(parent),freq(freq){
	}
	Node* getParent(){
		return parent.first;
	}
	int getPLink(){
		return parent.second;
	}
	void setParent(pair<Node *, int> xparent){
		parent = xparent;
	}
	int getFreq(){
		return freq;
	}
	void upFreq(int f){ //freq++
		freq += f;
	}
	virtual int getChar() = 0; //mark this class as pure virtual
	virtual ~Node(){};
};

class InsideNode: public Node{ //one possible node is insidenode to permit polymorphism
	vector<Node *> next{}; //inside node has childrens
	public:
	InsideNode(int d,Node *parent = nullptr):Node(make_pair(parent,-1),0){
		next.reserve(d); 
		for (int i = 0; i < d; i++){ //fill with nullptr
			next.push_back(nullptr);
		}
	};
	void addChild(int c,Node* child){
		next[c] = child;
		upFreq(child->getFreq());
	}
	~InsideNode(){
		for(auto & n:next){
			delete n;
		}
	}
	int getChar() override{ //invalid char get case
		return -1;
	}
	vector<Node *> &getNext(){
		return next;
	}
};
class LeafNode: public Node{ 
	unsigned char c;
	public:
	LeafNode( char c,int freq,Node* parent = nullptr): Node(make_pair(parent,0),freq),c(c){
	}
	int getChar() override{
		return c;
	}
	~LeafNode(){}
};

class Huff: public Transform
{
    //! huff is the end of the encoding. so we produce a binary encoding data.(goal)
    //! assumption if decode mode: the last vector<long> is of encode length
    //! assmption if encode mode: the last vector<long> is the encode length as well
    vector<Node*> minHeap{}; //the min heap
	vector<int> freqMap{}; //mapping of frequency when first run through input
	vector<LeafNode *> charMap{}; //link the leafnodes to a vector for ease of access

    int size = 0;
    void heapify();
	long leftChild(long i);
	long rightChild(long i);
	long parent(long i);
	void fixDown(long i, long cap);
	void fixUp(long i);
	Node *getMin();
	void pop();
	// string getEncode(int c); //return the upward traversal from a leaf
    pair<long,long> getEncode(long n);
    void transform(vector<unique_ptr<Block> > &input) override;
    void applyTo(vector<long>& data) override;
    void decode(vector<unique_ptr<Block> > &input) override;
    void deplyTo(vector<long> &data) override;
    long decodeChar(pair<int, int> input);
    
public:
    Huff(Transform *next = nullptr); //! there can not be a next after this one. we serialize after this
    ~Huff(){
        delete minHeap[0];
    }

};

#endif