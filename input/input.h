#ifndef ___INPUT
#define ___INPUT

#include <vector>
#include <utility>
#include <memory>
#include "line.h"
#include "block.h"
#include "../global.h"
#include <fstream>
/**
 * Input is a data to encapsulate the data read in.
 * 
 * - members: 
 *  LOCAL:
 *     - m_input_data: the vector of Block ptrs that collectively represent the file read
 *     - m_read_byte: the original file byte count read(when encoding)
 *     - m_input_stream: a reference to the designated input stream
 *     - m_file_ptr: a ptr to the fstream if used. needed to properly free it
 *     - m_decoded_file_name: the name of the decoded file.(for encoding, we just modify to xxx.dat)
 *     - m_end_valid_bit: stores the number of valid bits in the last byte(decoding)
 */
class Input{
    std::vector<std::unique_ptr<Block>> m_input_data;
    unsigned long m_original = 0;
    istream &m_input_stream;
    ifstream *m_fileptr = nullptr;
    char *m_decoded_file_name = nullptr;
protected:
    unsigned char endValidBit = 0;

    /**
     * executes either read or decode read
     */
    void run(bool);

    /**
     * verifies the signature of the file, currently set to LT(we should make it something cool :))
     * @return: 1 if valid
     */
    bool verifySig();

    /**
     * returns the unsigned char rep of the next byte in the m_input_stream
     * @return: next unsigned char
     */
    unsigned char getNextChar();

    /**
     * returns a ptr to the array that represents the file name read, and appends (dec) at the front.
     * @param: number of char to read
     * @return: the file name
     */
    char* getName(unsigned char length);

    /**
     * reads the next "length" chars from m_input_stream and puts them at "mem"; if "typecheck"
     * is enabled, checks if each read results in a Transformation type defined in global.h
     * 
     * @param: mem: the array where we put the next few reads
     * @param: length: the number of chars to read
     * @param: typecheck: if we should check each char against Transformation type
     */
    template <typename T>void readNArr(vector<T> &mem,unsigned int length,bool typecheck);

    /**
     * returns the next int in m_input_stream
     * 
     * @return: the unsigned int read
     */
    unsigned int getInt();

    /**
     * reads in the huffman data to the "mem" array
     * 
     * @param: mem: the array to put the data
     * @param: size: number of byte to read
     */
    void readHuff(vector<long> &mem, unsigned int size);
public:
    /**
     * returns the number of bits TO read from the last byte of the data sequence.
     * 
     * @return: the number of valid bits at the end of the data read.
     */
    unsigned char getEndVal();

    /**
     * returns the original byte size of a file read. (used in encoding)
     * 
     * @return: the byte of the file read
     */
    unsigned long getOriginalSize();

    /**
     * returns the data read(used for both decoding and encoding)
     * 
     * @returns: the vector of block ptrs that represnets the file read
     */
    std::vector<std::unique_ptr<Block> >& getInputData();

    /**
     * inserts a block ptr to data;
     * 
     * @param: the block ptr to add to data
     */
    void insertToData(std::unique_ptr<Block>&&);

    /**
     * returns the file name read(used by external objects)
     * 
     * @return: the char* representing the name;
     */
    char* getDecodeFileName();

    /**
     * reads in the file for ENCODING 
     * 
     */
    virtual void read();
    
    /**
     * reads in the file for DECODING
     * 
     */
    virtual void decodeRead();
    virtual ~Input(){
        if(m_fileptr)
            delete m_fileptr;
        if (m_decoded_file_name)
            delete m_decoded_file_name;
    };
    //cin ctor
    Input(istream &inputStream) : m_input_stream(inputStream){};

    //fstream ctor
    Input(ifstream* i) : m_input_stream(*i),m_fileptr(i){};
};

#endif