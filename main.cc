#include "./interface/comp.h"
using namespace std;
int main(int c, char** argv){
    try{
        Comp program{c, argv};
        program.run();
    }catch(Error& e){
        cout << e.what() << endl;
    }
}

