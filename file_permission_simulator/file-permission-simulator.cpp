#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

const unsigned int READ = 1;
const unsigned int WRITE = 1 << 1;   // 2
const unsigned int DELETE = 1 << 2;  // 4
const unsigned int EXECUTE = 1 << 3; // 8

const unsigned int ROOT = 15; // 1111

const char ROLE_ADMIN = 1<<5;           // 00100000
const char ROLE_NORMAL_USER = 1<<4;     // 00010000
const char ROLE_MASK = 0x30;            // 00110000

const string RED = "\x1b[31m";
const string GREEN = "\x1b[32m";
const string BLUE = "\x1b[34m";
const string RESET = "\x1b[0m";

class User
{
protected:
    string password;

public:
    string username;
    char role = 0;
    unsigned int permission = 0; // initially 0

    bool can(unsigned int p) { return (permission & p) == p; }

    void setPassword(string psswd) { password = psswd; }

    void setRole(unsigned char rolebit) {
        role &= ~0x30;          // resets the role first, in case it contains another value;
        role |= rolebit;        // sets the role to the rolebit;
    }

    bool isAdmin() {return (role & 0x20);}          // check bit 5
    bool isNormalUser() {return (role & 0x10);}     // check bit 4
};

vector <User> users;

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
    void setPermission(NormalUser *x)
    {   
        int choice;
        unsigned int p;
        cout << "==============================\n";
        cout << "[1] WRITE\n";
        cout << "[2] READ\n";
        cout << "[3] DELETE\n";
        cout << "[4] EXECUTE\n";
        cout << "[5] ROOT\n";
        cout << "[0] Exit\n";
        cout << "==============================\n";
        while(!(cin >> choice)) {
            cout << "Enter Permission to Set: ";
            cin.clear();
            cin.ignore(10000, '\n');
        }
        if (choice == 0) return;
        switch (choice) {
            case 1: p = WRITE; break;
            case 2: p = READ; break;
            case 3: p = DELETE; break;
            case 4: p = EXECUTE; break;
            case 5: p = ROOT; break;
        }
        x->permission |= p;
        if (x->can(p)) {
            cout << "Setting Permission Successful\n";
        } else {
            cout << "Setting Permission Error!\n";
        }
    }
    void togglePermission(NormalUser *x)
    {
        int choice;
        unsigned int p;
        cout << "==============================\n";
        cout << "[1] WRITE\n";
        cout << "[2] READ\n";
        cout << "[3] DELETE\n";
        cout << "[4] EXECUTE\n";
        cout << "[5] ROOT\n";
        cout << "[0] Exit\n";
        cout << "==============================\n";
        while(!(cin >> choice)) {
            cout << "Enter Permission to TOGGLE: ";
            cin.clear();
            cin.ignore(10000, '\n');
        }
        if (choice == 0) return;
        switch (choice) {
            case 1: p = WRITE; break;
            case 2: p = READ; break;
            case 3: p = DELETE; break;
            case 4: p = EXECUTE; break;
            case 5: p = ROOT; break;
        }
        int currP = x->can(p);

        x->permission ^= p;
        if (x->can(p) != currP) {
            cout << "Setting Permission Successful\n";
        } else {
            cout << "Setting Permission Error!\n";
        }
    }
    void revokePermission(NormalUser *x)
    {
        int choice;
        unsigned int p;
        cout << "==============================\n";
        cout << "[1] WRITE\n";
        cout << "[2] READ\n";
        cout << "[3] DELETE\n";
        cout << "[4] EXECUTE\n";
        cout << "[5] ROOT\n";
        cout << "[0] Exit\n";
        cout << "==============================\n";
        while(!(cin >> choice)) {
            cout << "Enter Permission to REVOKE: ";
            cin.clear();
            cin.ignore(10000, '\n');
        }
        if (choice == 0) return;
        switch (choice) {
            case 1: p = WRITE; break;
            case 2: p = READ; break;
            case 3: p = DELETE; break;
            case 4: p = EXECUTE; break;
            case 5: p = ROOT; break;
        }
        x->permission &= ~p;
        if (!(x->can(p))) {
            cout << "Revoking Permission Successful\n";
        } else {
            cout << "Revoking Permission Error!\n";
        }
    }
    void createAccount();
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
    cout << BLUE << "Login Successful!\n" << RESET;
    return uid;
}

void Admin::createAccount() {
    string role;
    do {
        cout << "Enter Role for New User: ";
        getline(cin, role);
    } while (role.empty());
    if (role == "normal user") {
        addNormalUser(&normalUserList);
    } else {
        addAdmin(&adminList);
    }
}

int main()
{
    string name, password, currUserRole;
    unsigned int uid;
    int index;
    unsigned int choice;

    if (adminList.size() == 0 && normalUserList.size() == 0)
    {
        addAdmin(&adminList); // adds an initial user (admin);
        currUserRole = "admin";
        uid = adminList.size();
        index = uid - 1;

        name = adminList[index].username;
    }

    while (1)
    {
        cout << BLUE << "\nWelcome to File System Simulator\n" << RESET;
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
                Admin user = adminList[uid - 1];
                
                cout << BLUE << "Login Successful\n\n" << RESET;
                cout << "==================================================\n";
                cout << "Welcome " << user.username << endl;
                cout << "==================================================\n";

                while(1) {
                    cout << "[1] CREATE New Account\n";
                    cout << "[2] SET Permission\n";
                    cout << "[3] REVOKE Permission\n";
                    cout << "[4] TOGGLE Permission\n";
                    cout << "[5] LOGOUT\n";
                    cout << "[0] EXIT";
                    cout << "Enter your Choice: ";

                    while(!(cin >> choice)) {
                        cout << "Invalid Choice!\n\n";
                        cin.clear();
                        cin.ignore(10000, '\n');
                    }
                    if (choice == 5) {
                        break;
                    }
                    /*switch(choice) {
                        case 1: user.createAccount(); break;
                        case 2: user.setPermission(normalUserList[0]); break;
                        case 3: user.revokePermission(); break;
                        case 4: user.togglePermission(); break;
                        case 0: exit(0);
                        default: cout << RED << "Invalid Input!\n\n" << RESET;
                    }*/
                }

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
                NormalUser user = normalUserList[uid - 1];
                // successful login
                cout << BLUE << "Login Successful\n\n" << RESET;
                cout << "==================================================\n";
                cout << "Welcome " << user.username << endl;
                cout << "==================================================\n";
                while(1) {
                    cout << "[1] READ file\n";
                    cout << "[2] WRITE file\n";
                    cout << "[3] DELETE file\n";
                    cout << "[4] EXECUTE file\n";
                    cout << "[5] LOGOUT\n";
                    cout << "[0] EXIT";
                    cout << "Enter your Choice: ";

                    while(!(cin >> choice)) {
                        cout << "Invalid Choice!\n\n";
                        cin.clear();
                        cin.ignore(10000, '\n');
                    }
                    if (choice == 5) {
                        break;
                    }
                    switch(choice) {
                        case 1: user.action(READ); break;
                        case 2: user.action(WRITE); break;
                        case 3: user.action(DELETE); break;
                        case 4: user.action(EXECUTE); break;
                        case 0: exit(0);
                        default: cout << RED << "Invalid Input!\n\n" << RESET;
                    }
                }
            }
        }
        else
        {
            break;
        }
    }

    return 0;
}