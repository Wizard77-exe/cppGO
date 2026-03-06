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
protected:
    string password;

public:
    unsigned int permission = 0; // initially 0
    string username;
    unsigned int id;

    bool can(unsigned int p) { return (permission & p); }

    void setPassword(string psswd)
    {
        password = psswd;
    }
    bool checkPassword(string pswd)
    {
        if (password == pswd)
        {
            return 1;
        }
        cout << RED << "Incorrect Password!\n"
             << RESET;
        return 0;
    }
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
    cout << GREEN << "Create Account!\n"
         << RESET;
    cout << "==========================\n";
    do
    {
        cout << GREEN << "Enter Username: " << RESET;
        getline(cin, name);
    } while (name.empty());
    do
    {
        cout << GREEN << "Enter Password: " << RESET;
        getline(cin, pass);
    } while (pass.empty());

    Admin a;
    a.username = name;
    a.permission = ROOT;
    a.setPassword(pass);
    a.id = x->size() + 1;

    x->push_back(a);
}

void addNormalUser(vector<NormalUser> *x)
{
    string name, pass;
    cout << GREEN << "Create Account!\n"
         << RESET;
    cout << "==========================\n";
    do
    {
        cout << GREEN << "Enter Username: " << RESET;
        getline(cin, name);
    } while (name.empty());
    do
    {
        cout << GREEN << "Enter Password: " << RESET;
        getline(cin, pass);
    } while (pass.empty());

    NormalUser a;
    a.username = name;
    a.setPassword(pass);
    a.id = x->size() + 1;

    x->push_back(a);
}

int getUid(const string username, const string role)
{
    if (role == "admin")
    {
        for (int i = 0; i < adminList.size(); i++)
        {
            if (adminList[i].username == username)
            {
                return i + 1;
            }
        }
    }
    else if (role == "normal user")
    {
        for (int i = 0; i < normalUserList.size(); i++)
        {
            if (normalUserList[i].username == username)
            {
                return i + 1;
            }
        }
    }

    cout << RED << "User Not Found!\n"
         << RESET;
    return 0;
}

int adminLogin()
{
    string name, pass;
    do
    {
        cout << GREEN << "Enter Username: " << RESET;
        getline(cin, name);
    } while (name.empty());

    int uid = getUid(name, "admin");
    if (!uid)
    {
        return 0;
    }

    do
    {
        cout << GREEN << "Enter Password: " << RESET;
        getline(cin, pass);
    } while (pass.empty());

    if (!(adminList[uid - 1].checkPassword(pass)))
    {
        return 0;
    }
    cout << BLUE << "Login Successful!\n"
         << RESET;
    return uid;
}

int normalUserLogin()
{
    string name, pass;
    do
    {
        cout << GREEN << "Enter Username: " << RESET;
        getline(cin, name);
    } while (name.empty());

    int uid = getUid(name, "normal user");
    if (!uid)
    {
        return 0;
    }

    do
    {
        cout << GREEN << "Enter Password: " << RESET;
        getline(cin, pass);
    } while (pass.empty());

    if (!(normalUserList[uid - 1].checkPassword(pass)))
    {
        return 0;
    }
    cout << BLUE << "Login Successful!\n"
         << RESET;
    return uid;
}

int main()
{
    string name, password, currUserRole;
    unsigned int uid;
    int index;

    if (adminList.size() == 0 && normalUserList.size() == 0)
    {
        addAdmin(&adminList); // adds an initial user (admin);
        currUserRole = "admin";
        uid = adminList.size();
        index = uid - 1;

        name = adminList[index].username;
    }

    unsigned int choice;

    while (1)
    {
        cout << BLUE << "\nWelcome to File System Simulator\n"
             << RESET;
        cout << "==================================================\n";
        cout << RED << "[Just Click Enter to Exit]\n";
        cout << "[Enter \'admin\' to go admin]\n";
        cout << "[Enter \'normal user\' to login as normal user]\n\n";
        cout << GREEN << "Enter Role: " << RESET;
        getline(cin, currUserRole);

        if (currUserRole == "admin")
        {
            // admin logic
            if (!(uid = adminLogin()))
            {
                // failed login
                cout << "Failed Login" << endl;
                break;
            }
            else
            {
                // successful login
                cout << "Login Successful" << endl;
                cout << adminList[uid - 1].username << " " << adminList[uid - 1].id << endl;
                break;
            }
        }
        else if (currUserRole == "normal user")
        {
            // normal user logic
            if (!(uid = normalUserLogin()))
            {
                // failed login
                cout << "Failed Login" << endl;
                break;
            }
            else
            {
                // successful login
                cout << "Login Successful" << endl;
                cout << normalUserList[uid - 1].username << " " << normalUserList[uid - 1].id << endl;
                break;
            }
        }
        else
        {
            break;
        }
    }

    return 0;
}