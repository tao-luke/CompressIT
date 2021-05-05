#ifndef ___STDIN
#define ___STDIN

#include <utility>
#include "input.h"
#include <iostream>
/**
 * Stdin is a subclass of Input and is responsible to be reading a set of data from the input stream
 * - members: 
 *   INHERITED:
 *     - m_input_data: the vector of Block ptrs that collectively represent the file read
 *     - m_read_byte: the original file byte count read(when encoding)
 *     - m_input_stream: a reference to the designated input stream
 *     - m_file_ptr: a ptr to the fstream if used. needed to properly free it
 *     - m_decoded_file_name: the name of the decoded file.(for encoding, we just modify to xxx.dat)
 *     - m_end_valid_bit: stores the number of valid bits in the last byte(decoding)
 *  LOCAL:
 */
class Stdin: public Input{
public:
    Stdin(bool c):Input(cin){
        run(c);
    }
};

#endif