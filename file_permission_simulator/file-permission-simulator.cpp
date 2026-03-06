#include <iostream>

const unsigned int READ = 1;
const unsigned int WRITE = 1 << 1;      // 2
const unsigned int DELETE = 1 << 2;     // 4
const unsigned int EXECUTE = 1 << 3;    // 8

const unsigned int ROOT = 0 | READ | WRITE | DELETE | EXECUTE;       // 16

class User {
    public: 
        unsigned int permission = 0;    // initially 0

        bool can(unsigned int p) { return (permission & p);}
};

int main() {
    User a;
    a.permission |= READ;
    a.permission |= WRITE;
    a.permission |= DELETE;

    std::cout << "User a can read? " << a.can(EXECUTE) << "\n";

    return 0;
}