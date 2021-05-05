#ifndef ___IFILE
#define ___IFILE

#include <utility>
#include "input.h"
#include <fstream>

//subclass of Input, reads a fstream from a fileName
/**
 *   INHERITED:
 *     - m_input_data: the vector of Block ptrs that collectively represent the file read
 *     - m_read_byte: the original file byte count read(when encoding)
 *     - m_input_stream: a reference to the designated input stream
 *     - m_file_ptr: a ptr to the fstream if used. needed to properly free it
 *     - m_decoded_file_name: the name of the decoded file.(for encoding, we just modify to xxx.dat)
 *     - m_end_valid_bit: stores the number of valid bits in the last byte(decoding)
 * 
 */
class Ifile:public Input{

public:
    Ifile(bool c,const vector<string>& fileNames):Input(new ifstream (fileNames[0],std::ifstream::in)){
    //! extend this to handle multiple files!
    //current default to setting name as first file
    run(c);
}
};

#endif