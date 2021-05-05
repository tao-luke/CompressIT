#ifndef ___TRANSFORM
#define ___TRANSFORM
#include "../global.h"
#include <memory>
#include "../input/block.h"
#include <map>

#include <iostream>
/**
 * Transform is an abstract class used to organize the compressions. It requires that each 
 * subclass have a transform,decode,applyTo,and decodeTo function. 
 * - members: 
 *  LOCAL:
 *     - m_next: a transform * pointing to the next Transformation.
 *     - m_encode: a boolean used to indicate if the transformation is to decode or encode
 *     - m_encodeMap: a mapping of (encoded value, encoded value length in bits)->actual char
 *                    #reasoning: we have at most 256 chars to be encoded, thus the length of the encoded val
 *                    # in huffman is no greater than 255. but the actual number represented by this 255length bit
 *                    much greater than a char, so we store both the encoded val and length of encoding
 *     - m_original_size:  the unsigned int count of the original file(also accurate in byte)
 *     - m_end_valid_bits: an unsigned char representing the number of bits in the last byte that are valid.(used in decoding)
 */
using namespace std;
class Transform
{
    Transform* m_next; //links to the next Transform, where data will be passed next.
    bool m_encode = true; //whether we are encoding or decoding

    /**
     * encodes a vector of ptrs to Block inplace.(virtual)
     * 
     * since each block could either be a line and pixel at the moment,this function essentialy 
     * applies the encoding of the current Transformation and updates the vector of ptrs 
     * in place.
     * @param: a vector of block ptrs
     */
    virtual void transform(vector<unique_ptr<Block> > &input) = 0;

    /**
     * decodes a vector of ptrs to Block inplace.(virtual)
     * 
     * since each block could either be a line and pixel at the moment,this function essentialy 
     * applies the decoding of the current Transformation and updates the vector of ptrs 
     * in place.
     * @param: a vector of block ptrs
     */
    virtual void decode(vector<unique_ptr<Block> > &input) = 0;

    /**
     * encodes a vector of long in place.
     * @param: a vector of long
     */
    virtual void applyTo(vector<long> &data) = 0;

    /**
     * the function to run to essentially begin the encoding process on a vector of block ptrs
     * 
     * iterates to each block untill nullptr, and applies the current Transformation's rules to the input
     * to encode the data in place, before passing on to the next if not nullptr.
     * 
     * @param: a vector of block ptrs
     */
    void run(vector<unique_ptr<Block> > &input);

    /**
     * encodes a vector of ptrs to Block inplace.(virtual)
     * 
     * since each block could either be a line and pixel at the moment,this function essentialy 
     * applies the encoding of the current Transformation and updates the vector of ptrs 
     * in place.
     * @param: a vector of block ptrs
     */
    void run2(vector<unique_ptr<Block> > &input);

protected:
    map<pair<long,unsigned char>, unsigned char>* m_encodeMap = nullptr; //encoding map to be modified by huffman
    unsigned int m_original_size = 0; //count the original size
    unsigned char m_end_valid_bits = 0; //used in decoding mode, since the encoding data isn't always %8==0
public:
    Transform(Transform* m_next):m_next(m_next){}
    void setEndValidBits(unsigned char n){
        m_end_valid_bits = n;
    }
    void setEncode(bool n){
        m_encode = n;
    }
    unsigned int getOriginalSize(){
        return m_original_size;
    }
    
    //initializes the encodemap ptr
    void initEncodeMap();


    void execute(vector<unique_ptr<Block> > &input)
    {
        if (m_encode)
            run(input);
        else
            run2(input);
    }

    //creates the encodeMapping from a vector. this is used when decoding.
    void setEncodeMap(const vector<long> &enMapArr);

    //creates the vector that represents an encodemapping
    vector<long> getEncodeMap();
    virtual ~Transform()
    {
        if (m_next == nullptr){
            delete m_encodeMap;
        }
        delete m_next;
    };
};

#endif