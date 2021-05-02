#include "./interface/program.h"
#include <iostream>

using namespace std;

int main(int c, char** argv){
    try{
        Program program{c, argv};
        program.run();
    }catch(Error& e){
        cout << e.what() << endl;
    }
}
