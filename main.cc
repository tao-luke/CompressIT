#include "./input/stdin.h"
#include <iostream>
using namespace std;
int main(){

    unique_ptr<Input> input{new Stdin()};
    const std::vector<std::unique_ptr<Block> > &data = input->getInputData();
    for(auto && ptr: data){
        for(const auto& n: ptr->getData()){
            cout << n << " " <<static_cast<char>(n) << endl;
        }
    }
}