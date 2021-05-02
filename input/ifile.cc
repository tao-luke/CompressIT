#include "ifile.h"
#include <fstream>


Ifile::Ifile(bool c,const vector<string>& fileNames):fileName(fileNames[0]){
    //! extend this to handle multiple files!

    run(c);
    fileName = fileNames[0]; //default to first file name, this will be useful when we combine more files later on
}

void Ifile::read(){
    std::ifstream ifs;
    try{
        ifs.open (fileName.data(), std::ifstream::in);
    }catch(...){
        throw Error("bad file name given");
    }
    unsigned char c = ifs.get();
    fileName.clear(); // use fileName to store the char given.
    while (ifs.good())
    {
        fileName.push_back(c);
        c = ifs.get();
    }
    insertToData(std::unique_ptr<Block>(new Line(std::move(fileName))));
}
void Ifile::decodeRead(){
 //! not yet added
}