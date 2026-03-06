#include <iostream>

using namespace std;


const unsigned int READ = 1;
const unsigned int WRITE = 1 << 1;      // 2
const unsigned int DELETE = 1 << 2;     // 4
const unsigned int EXECUTE = 1 << 3;    // 8

const unsigned int ROOT = 0 | READ | WRITE | DELETE | EXECUTE;       // 16

class User {
    public: 
        unsigned int permission = 0;    // initially 0
        string username;
        string password;

        bool can(unsigned int p) { return (permission & p);}
};

class NormalUser : public User {
    public: 
};

class Admin : public User {
    public: 
        void setPermission(NormalUser *x, const unsigned int p) {
            x->permission |= p;
        }
        void togglePermission(NormalUser *x, const unsigned int p) {
            x->permission ^= p;
        }
        void revokePermission(NormalUser *x, const unsigned int p) {
            x->permission &= ~p;
        }
};

int main() {
    NormalUser a;
    Admin b;

    b.setPermission(&a, READ);

    cout << "User a can read? " << a.can(READ) << "\n";
    cout << "User a can write? " << a.can(WRITE) << "\n";

    return 0;
}