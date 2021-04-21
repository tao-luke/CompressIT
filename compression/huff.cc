#include "huff.h"
#include <cmath>
class Error{
};
long Huff::leftChild(long i)
{
    return (2 * i) + 1;
}
long Huff::rightChild(long i){
	return (2 * i) + 2;
}
long Huff::parent(long i){
	return (i-1)/ 2;
}
Huff::Huff(Transform* next):Transform(next){
    freqMap.reserve(256); //make it fast 
    freqMap.resize(256); //fill with 0s
}

void Huff::fixDown(long i,long cap){
    while(leftChild(i) < cap){ //not leaf
		if (rightChild(i) < cap && minHeap[rightChild(i)]->getFreq() < minHeap[leftChild(i)]->getFreq()){ //if right child exists and right is more suited than left
			if (minHeap[i]->getFreq() > minHeap[rightChild(i)]->getFreq()){
				swap(minHeap[i], minHeap[rightChild(i)]);
				i = rightChild(i);
			}else{
				break;
			}
		}else if (minHeap[i]->getFreq() > minHeap[leftChild(i)]->getFreq()){
			swap(minHeap[i], minHeap[leftChild(i)]);
			i = leftChild(i);
		}else{
			break;
		}
	}
}

void Huff::heapify(){
	for (int i = parent(size - 1); i >= 0; i--){
		fixDown(i, size);
	}
}
Node* Huff::getMin(){
	return minHeap[0];
}

void Huff::pop(){
	if (size == 0)
		throw Error();
	swap(minHeap[0], minHeap[size- 1]); //swap first with last
	size--;
	fixDown(0, size);
}

void Huff::fixUp(long i){
	while(parent(i) >= 0){
		if (minHeap[i]->getFreq() < minHeap[parent(i)]->getFreq()){
			swap(minHeap[i], minHeap[parent(i)]);
			i = parent(i);
		}else{
			break;
		}
	}
}
long Huff::getEncode(long c){
    string result = "";
    long end = 0;
    for (Node *p : charMap)
    {
		if (p->getChar() == c)
		{
			while(p != nullptr && p->getPLink() != -1){ //traverse up
				result.push_back((p->getPLink() + 48));
				p = p->getParent();
			}
            int power = 0;
            for (int i = 0; i < result.size(); i++)
            {
                if (result[i] == '1')
                    end += pow(2, power);
                power++;
            }
            return end;
		}
	}
    return -1;
}
int Huff::getEncodingLength(long n){
    int counter = 0;
    for (Node *p : charMap)
    {
		if (p->getChar() == n)
		{
			while(p != nullptr && p->getPLink() != -1){ //traverse up
                counter++;
                p = p->getParent();
            }
            break;
        }
    }
    return counter;
}

long Huff::decodeChar(pair<int,int> i){
    return encodeMap.find(i)->second;
}

void Huff::transform(vector<unique_ptr<Block> > &input){
    //create frquency map
    for(const unique_ptr<Block>& line:input){
        for (const auto &item : line->getData())
        {
            freqMap[item]++;
        }
    }

    int c = 0;
    for (const auto &freq : freqMap) //insert to our heap
	{
		if (freq != 0)
		{
			LeafNode *tmp = new LeafNode(c, freq);
			minHeap.push_back(tmp);
			charMap.push_back(tmp);
		}
		c++;
	}
    freqMap.clear(); //clear out the freq map to clear mem
    size = minHeap.size(); //initing a size
    heapify();
    while (size > 1) //keep combining nodes
	{
		int Npaths = min(2, size);
		InsideNode* root = new InsideNode(Npaths);
		while (Npaths >= 1)
		{
			getMin()->setParent(make_pair(root,Npaths-1));
			root->addChild(Npaths - 1, getMin());
			pop();
			Npaths--;
		}
		minHeap[size] = root;
		fixUp(size);
		size++;
	}

    for(Node* p:charMap){
        encodeMap.insert(make_pair(make_pair(getEncode(p->getChar()), getEncodingLength(p->getChar())), p->getChar()));
    }

    for (const unique_ptr<Block> &line : input)
    {
        for (auto &item : line->getData())
        {
            item = getEncode(item);
        }
    } //create encode data;
}

void Huff::decode(vector<unique_ptr<Block>>& input){
}

void Huff::deplyTo(vector<long> & line){

}

void Huff::applyTo(vector<long>& line){
}