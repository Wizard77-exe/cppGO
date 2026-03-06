#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

const unsigned int READ = 1;
const unsigned int WRITE = 1 << 1;   // 2
const unsigned int DELETE = 1 << 2;  // 4
const unsigned int EXECUTE = 1 << 3; // 8

const unsigned int ROOT = 0 | READ | WRITE | DELETE | EXECUTE; // 16

const string RED = "\x1b[31m";
const string GREEN = "\x1b[32m";
const string BLUE = "\x1b[34m";
const string RESET = "\x1b[0m";

class User
{
public:
    unsigned int permission = 0; // initially 0
    string username;
    string password;
    unsigned int id;

    bool can(unsigned int p) { return (permission & p); }
};

class NormalUser : public User
{
public:
    void action(const unsigned int p)
    {
        string success, failure;
        switch (p)
        {
        case DELETE:
            success = "DELETED";
            failure = "DELETE";
            break;
        case READ:
            success = "READ";
            failure = "READ";
            break;
        case WRITE:
            success = "WRITTEN";
            failure = "WRITE";
            break;
        case EXECUTE:
            success = "EXECUTED";
            failure = "EXECUTE";
            break;
        default:
            cout << "Undefined Action!\n";
            return;
        }

        if (can(p))
        {
            cout << GREEN << "File " << success << " successfully!\n"
                 << RESET;
        }
        else
        {
            cout << RED << "You don't have " << failure << " permission\n"
                 << RESET;
        }
    }
};

class Admin : public User
{
public:
    void setPermission(NormalUser *x, const unsigned int p)
    {
        x->permission |= p;
    }
    void togglePermission(NormalUser *x, const unsigned int p)
    {
        x->permission ^= p;
    }
    void revokePermission(NormalUser *x, const unsigned int p)
    {
        x->permission &= ~p;
    }
};

vector<Admin> adminList;
vector<NormalUser> normalUserList;

void addAdmin(vector<Admin> *x)
{
    string name, pass;
    cout << GREEN << "Create Your First Account!\n" << RESET;
    cout << "==========================\n";
    do {
        cout << GREEN << "Enter Username: " << RESET;
        getline(cin, name);
    } while (name.empty());
    do {
        cout << GREEN << "Enter Password: " << RESET;
        getline(cin, pass);
    } while (pass.empty());

    Admin a;
    a.username = name;
    a.password = pass;
    a.permission = ROOT;

    x->push_back(a);
}

void addNormalUser(vector<NormalUser> *x)
{
    string name, pass;
    cout << GREEN << "Create Your First Account!\n" << RESET;
    cout << "==========================\n";
    do {
        cout << GREEN << "Enter Username: " << RESET;
        getline(cin, name);
    } while (name.empty());
    do {
        cout << GREEN << "Enter Password: " << RESET;
        getline(cin, pass);
    } while (pass.empty());

    NormalUser a;
    a.username = name;
    a.password = pass;

    x->push_back(a);
}

int main()
{
    string name, password;
    unsigned int uid;

    if (adminList.size() == 0 && normalUserList.size() == 0)
    {
        addAdmin(&adminList);      // adds an initial user (admin);
    }

    return 0;
}