#ifndef ___OFILE
#define ___OFILE


#include <fstream>
#include <string.h>
#include <bitset>
#include <iostream>
#include "../global.h"
#include <queue>
#include <math.h>
#include "../input/block.h"
#include <memory>
/**
 * Ofile produces the actual file output using data provided by the program
 * 
 * - members: 
 *  LOCAL:
 *      - m_FILE_SIG[3] : the signatue of the file
 *      - m_FILE_NAME_LENGTH: the length of the file name
 *      - m_TRANSFORM_LENGTH: the length of the Tseq(transforamtion sequence)
 *      - m_FILE_NAME: the file name
 *      - m_TRANSFORM_ARR: the Tseq or transformaton array(records what happened to the data)
 *      - m_COMP_CHAR_COUNT: the number of chars used by data ?
 *      - m_FILE_BYTE_COUNT: original byte count;
 *      - m_huff_byte: how many bytes actuallyed used by huffman encodemap arr
 *      - m_DATA_TRIO_COUNT: the number of trios that represented a unit in the encodeMap arr
 *      - m_HUFF_TRIOS: the encodeMap arr
 *      - m_data_byte: actual byte usage of data
 *      - m_data_ptr: the ptr to the data
 */
class Ofile{
    //following needs use of strcpy to rid of \0

    //signature
    char FILE_SIG[3] = "LT"; //2byte  

    //name length
    char FILE_NAME_LENGTH; //1byte

    //transform history length
    char TRANSFORM_LENGTH; //1byte

    //name of file
    const char* FILE_NAME = nullptr; //inited by ctor

    //transofrm history
    char *TRANSFORM_ARR = nullptr; // inited by ctor //must be freed

    //how many chars we should MATCH with data
    unsigned int COMP_CHAR_COUNT = 0;        // inited by ctor  4bytes

    //actual data char count, compare after decompress for sanity check
    unsigned int FILE_BYTE_COUNT; // inited by ctor 4bytes

    long huffbyte = 0; //how many size of byte is hufftrio data
    //huff trio length
    unsigned char DATA_QUADRO_COUNT = 0;  // inited by ctor

    //huff trio
    char *HUFF_QUADRO = nullptr; // inited by ctor //must be freed

    //data file
    unsigned long databyte = 0; //actual byte usage of data write
    char *dataPtr = nullptr; //inited by ctor //must be freed

    /**
     * initializes the transformation arrray using a Tseq arr given.
     * @param: Tseq arr representing the sequence of operation
     */
    void initTransformArr(vector<Transformation> &Tseq);

    /**
     * inserts "n", which is larger than a char, into result using multiple bytes
     * @param: result: what processed to be written already
     * @param: n the larger than byte thingy that needs to be represented
     */
    void insertBigChar(vector<unsigned char> &result, unsigned long n);

    /**
     * initailizes the huffman encoding data to be ready for write
     * format as follow:
     * 0mod4 - bit length of the alphabet
     * 1mod4 - the alphabet with length specificed in 0mod4
     * 2mod4 - length of encoding alphabet
     * 3mod4 - the encoded alphabet
     * @param: the huffman arrray representing the encodeMap
     */
    void init_huff_quadro(vector<long> encodeMapArr);

    /**
     * writes to file as request in encode mode
     */
    void writeAsEncodedFile();

    /**
     * initializes the data to be ready for write
     * @param: data: the representation of the file
     * @param: encodingLength: the length of each encoded char in the data, this needs to be written
     * along side with the data(actual before it to tell the decode reader how many bits to read)
     */
    void initData(vector<unique_ptr<Block> > &data, const vector<long> &encodingLength);

    /**
     * writes out a file as if data is the file already. 
     * @param: data to be written out to a file(used in decoding)
     */
    void writeAsRawFile(const vector<long> &data);

public:
    //encode ctor
    Ofile(vector<unique_ptr<Block> > &data, vector<long> encodeMapArr, vector<Transformation> Tseq, unsigned int originalSize, const char *FILE_NAME, size_t FILE_NAME_LENGTH);
    
    //decode ctor
    Ofile(vector<long> &data, const char *FILE_NAME);
};
#endif