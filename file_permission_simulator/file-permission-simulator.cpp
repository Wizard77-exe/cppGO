#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

const unsigned int READ = 1;
const unsigned int WRITE = 1 << 1;      // 2
const unsigned int DELETE = 1 << 2;     // 4
const unsigned int EXECUTE = 1 << 3;    // 8

const unsigned int ROOT = 0 | READ | WRITE | DELETE | EXECUTE;       // 16

const string RED = "\x1b[31m";
const string GREEN = "\x1b[32m";
const string BLUE = "\x1b[34m";
const string RESET = "\x1b[0m";

class User {
    public: 
        unsigned int permission = 0;    // initially 0
        string username;
        string password;

        bool can(unsigned int p) { return (permission & p);}
};

class NormalUser : public User {
    public: 
        void action(const unsigned int p) {
            string act;
            switch (p) {
                case DELETE: 
                    act = "DELETED";
                    break;
                case READ:
                    act = "READ";
                    break;
                case WRITE:
                    act = "WRITTEN";
                    break;
                case EXECUTE:
                    act = "EXECUTED";
                    break;
                default:
                    cout << "Undefined Action!\n";
                    return;
            }

            if (can(p)) {
                cout << GREEN << "File " << act << " successfully!\n" << RESET;
            }else{
                cout << RED << "You don't have DELETE permission\n" << RESET;
            }
        }
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

vector<Admin> adminList;
vector<NormalUser> normalUserList;

int main() {
    NormalUser a;
    Admin b;

    b.setPermission(&a, READ);

    a.action(READ);

    cout << "User a can read? " << a.can(READ) << "\n";
    cout << "User a can write? " << a.can(WRITE) << "\n";

    return 0;
}