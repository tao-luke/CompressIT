#include <iostream>
#include <string>
using namespace std;
int main(int c, char **argv)
{
    cout << c << endl;
    cout << string(*(argv+1)) << endl;
}