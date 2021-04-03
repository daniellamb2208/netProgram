#include <iostream>
#include <fstream>
#include <string>
#include <bitset>
using namespace std;

int main(int argc, char *argv[])
{
    ifstream f("b.txt", ios::binary);
    char x;
    while (f.get(x))
        cout << static_cast<int>((unsigned char)(x)) << " " << endl;
    f.close();
    return 0;
}