#include <iostream>
#include <fstream>

using namespace std;

int main() {
    ofstream file("output.txt");

    file << "Hello Hackathon!\n";
    file << "C++ File Handling\n";

    file.close();
    return 0;
}