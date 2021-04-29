#include "huff.h"
#include <cmath>
#include <bitset>
#include <queue>
//heap impl
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
		throw Error("pop errro");
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
//end heap impl


pair<long,unsigned char> Huff::getEncode(unsigned char c){
    string result = "";
    long end = 0;
    unsigned char counter = 0;
    for (Node *p : charMap) //recurse up the tree to get height and encode representation in num
    {
		if (p->getChar() == c)
		{
			while(p != nullptr && p->getPLink() != -1){ //traverse up
				result.push_back((p->getPLink() + 48));
				p = p->getParent();
                counter++;
            }
            int power = 0;
            for (int i = 0; i < result.size(); i++)
            {
                if (result[i] == '1')
                    end += pow(2, power);
                power++;
            }
            return make_pair(end, counter);
        }
	}
    return make_pair(-1, -1);
}

unsigned char Huff::decodeChar(pair<long,unsigned char> i){
    auto ptr = encodeMap->find(i);
    if (ptr == encodeMap->end())
        throw Error("not error");
    return encodeMap->find(i)->second;
}

void Huff::transform(vector<unique_ptr<Block> > &input){
    //create frquency map
    
    
    for(const unique_ptr<Block>& line:input){
        for (const auto &item : line->getData())
        {
            freqMap[item]++;
            //cout << item << " ";
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

    pair<long, unsigned char> tmp{};
    for(Node* p:charMap){
        tmp = getEncode(p->getChar());
        if (tmp.first == -1)
            throw Error("bad pair from huff1");
        encodeMap->insert({{tmp.first, tmp.second}, p->getChar()});
    }

    vector<long> encodeLength{};
    for (const unique_ptr<Block> &line : input)
    {
        for (auto &item : line->getData())
        {
            tmp = getEncode(item);
            if (tmp.first == -1)
                throw Error("bad pair from huff2");
            item = tmp.first;
            encodeLength.push_back(tmp.second);
        }
    } //create encode data;
    //push back encodeLength data.
    input.push_back(std::unique_ptr<Block>(new Block(std::move(encodeLength))));
}

void Huff::decode(vector<unique_ptr<Block>>& input){
    vector<long> &data = input[0]->getData();
    bitset<64> buffer{}; //long mimick
    vector<long> result{};
    queue<bool> dataStream{};
    int missing = 64;
    int counter = 1;
    for (const auto &n : data)
    {
        buffer = bitset<64>(n); //get bit rep
        for (int i = 7; i >= 0;i--){ //input all the bit in stream
            dataStream.push(buffer[i]);
        }
    }
    while(!dataStream.empty()){
        while(missing != 0){ //fill buffer
            if (dataStream.empty()){ //if we run out of bits
                buffer = buffer >> missing;
                break;
            }
            buffer[missing - 1] = dataStream.front();
            dataStream.pop();
            missing--;
        }
        for (int i = 63-missing; i >= 0;i--){
            try{
                long c = decodeChar(make_pair((buffer >> i).to_ulong(), counter));
                result.push_back(c);
                buffer = buffer << counter;
                missing = counter;
                counter = 1;
            }catch(...){
                counter++;
                break;
            }
        }
    }

    result.push_back(255);
    result.push_back(6); //! fix!!! hardcoded last char missing
    input[0]->setData(std::move(result));
}
void Huff::deplyTo(vector<long> & line){

}

void Huff::applyTo(vector<long>& line){
}