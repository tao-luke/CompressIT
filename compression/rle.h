#ifndef ___RLE
#define ___RLE
#include "transform.h"
/**
 * Rle is the subclass of Transform that encodes and decodes rle in 2's bijective numeration
 * - members: 
 *  INHERITED:
 *     - m_next: a transform * pointing to the next Transformation.
 *     - m_encode: a boolean used to indicate if the transformation is to decode or encode
 *     - m_encodeMap: a mapping of (encoded value, encoded value length in bits)->actual char
 *                    #reasoning: we have at most 256 chars to be encoded, thus the length of the encoded val
 *                    # in huffman is no greater than 255. but the actual number represented by this 255length bit
 *                    much greater than a char, so we store both the encoded val and length of encoding
 *     - m_original_size:  the unsigned int count of the original file(also accurate in byte)
 *     - m_end_valid_bits: an unsigned char representing the number of bits in the last byte that are valid.(used in decoding)
 *  LOCAL:
 */
class Rle: public Transform
{
    /**
     * encodes a vector of ptrs to Block inplace using rle encoding
     *  this overrides from the parent Transform
     * @param: a vector of block ptrs
     */
    void transform(vector<unique_ptr<Block> > &input) override;

    /**
     * encodes a vector of long
     *  this overrides from the parent Transform
     * @param: a vector of block ptrs
     */
    void applyTo(vector<long>& data) override;

    /**
     * decodes a vector of ptrs to Block inplace using rle encoding
     *  this overrides from the parent Transform
     * @param: a vector of block ptrs
     */
    void decode(vector<unique_ptr<Block> > &input) override;

    /**
     * decodes a vector of long using rle decoding
     *  this overrides from the parent Transform
     * @param: a vector of block ptrs
     */
    void deplyTo(vector<long> &data, vector<long> &copy);

    /**
     * inserts the 2's bijective numeration rep of a number in the "data"
     * @return: the number of bytes to skip forward
     * @param: a vector of block ptrs
     */
    size_t bbnRep(vector<long> &data, int n, int &slow);

    /**
     * inserts the number in base10 from a 2's bijective num in the "data"
     * @return: the number of bytes to skip forward
     * @param: a vector of block ptrs
     */
    size_t longRep(vector<long> &data, int& index);

public:
    Rle(Transform *next);
};

#endif