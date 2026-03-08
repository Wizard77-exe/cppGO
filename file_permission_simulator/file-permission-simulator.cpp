#include <iostream>
#include <vector>
#include <iomanip>

using namespace std;

const unsigned char READ = 0x01;
const unsigned char WRITE = 0x02;   // 2
const unsigned char DELETE = 0x04;  // 4
const unsigned char EXECUTE = 0x08; // 8

const unsigned char ROOT = 0x0f; // 1111

const unsigned char ROLE_NORMAL_USER = 0x10; // 00010000
const unsigned char ROLE_ADMIN = 0x20;       // 00100000
const unsigned char ROLE_MASK = 0x30;        // 00110000

const string RED = "\x1b[31m";
const string YELLOW = "\x1b[33m";
const string GREEN = "\x1b[32m";
const string BLUE = "\x1b[34m";
const string RESET = "\x1b[0m";

class User {
    protected:
        string password;

    public:
        string username;
        unsigned int id;
        unsigned char status = 0x0;

        bool can(unsigned char p) { return (status & p) == p; }

        void setPassword(string psswd) { password = psswd; }

        void setRole(unsigned char rolebit) {
            status &= ~ROLE_MASK; // resets the role first, in case it contains another value;
            status |= rolebit;    // sets the role to the rolebit;
        }

        bool isAdmin() { return (status & ROLE_ADMIN) > 0; }            // check bit 5
        bool isNormalUser() { return (status & ROLE_NORMAL_USER) > 0; } // check bit 4

        void showPermissions() {
            cout << GREEN << "[ID]: " << left << setw(10) << id << "[Username]: " << left << setw(30) << username << "[Permissions]: ";
            cout << (can(READ) ? "R" : "-");
            cout << (can(WRITE) ? "W" : "-");
            cout << (can(DELETE) ? "D" : "-");
            cout << (can(EXECUTE) ? "E" : "-") << RESET << endl;
        }

        bool checkPassword(string pass) {
            if (password == pass)
                return 1;
            return 0;
        }
};

vector<User> users;

class NormalUser : public User {
    public:
        void action(const unsigned char p) {
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
            cout << "==================================================\n";
            if (can(p))
            {
                cout << GREEN << "File " << success << " successfully!\n" << RESET;
            }
            else
            {
                cout << RED << "You don't have " << failure << " permission\n" << RESET;
            }
            cout << "==================================================\n\n";
        }
};

unsigned int checkUserName(const string name)
{
    for (auto &u : users)
    {
        if (u.username == name)
        {
            return u.id;
        }
    }
    return 0;
}

class Admin : public User {
    public:
        void setPermission() {
            if (cin.peek() == '\n')
                cin.ignore(10000, '\n');
            string name;
            int uid;
            do {
                cout << GREEN << "Enter Username to Give Permission: " << RESET;
                getline(cin, name);
            } while (name.empty());

            if ((uid = checkUserName(name)) == 0) {
                return;
            }

            User &x = users[uid - 1];

            int choice;
            unsigned char p;
            cout << "==================SET PERMISSION==================\n";
            cout << BLUE << "[1]" << YELLOW << " WRITE\n";
            cout << BLUE << "[2]" << YELLOW << " READ\n";
            cout << BLUE << "[3]" << YELLOW << " DELETE\n";
            cout << BLUE << "[4]" << YELLOW << " EXECUTE\n";
            cout << BLUE << "[5]" << YELLOW << " ROOT\n";
            cout << BLUE << "[0]" << YELLOW << " Exit\n" << RESET;
            cout << "==================================================\n";

            cout << RED << "[CHOICE:]> " << RESET;
            while (!(cin >> choice))
            {
                cout << RED << "[CHOICE:]> " << RESET;
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
                default: cout << RED << "INVALID INPUT!\n" << RESET; p = 0x00; break;
            }

            x.status |= p;
            if (x.can(p)) {
                cout << GREEN << "Setting Permission Successful\n" << RESET;
            } else {
                cout << "Setting Permission Error!\n";
            }
            cout << "==================================================\n\n";
        }

        void togglePermission() {
            if (cin.peek() == '\n') cin.ignore(10000, '\n');
            string name;
            int uid;
            do {
                cout << GREEN << "Enter Username to Toggle Permission: " << RESET;
                getline(cin, name);
            } while (name.empty());

            if ((uid = checkUserName(name)) == 0) {
                return;
            }

            User &x = users[uid - 1];

            int choice;
            unsigned char p;
            cout << "======================TOGGLE======================\n";
            cout << BLUE << "[1]" << YELLOW << " WRITE\n";
            cout << BLUE << "[2]" << YELLOW << " READ\n";
            cout << BLUE << "[3]" << YELLOW << " DELETE\n";
            cout << BLUE << "[4]" << YELLOW << " EXECUTE\n";
            cout << BLUE << "[5]" << YELLOW << " ROOT\n";
            cout << BLUE << "[0]" << YELLOW << " Exit\n" << RESET;
            cout << "==================================================\n";

            cout << RED << "[CHOICE:]> " << RESET;
            while (!(cin >> choice))
            {
                cout << RED << "[CHOICE:]> " << RESET;
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
                default: cout << RED << "INVALID INPUT!\n" << RESET; p = 0x00; break;
            }

            int currP = x.can(p);
            x.status ^= p;

            if (x.can(p) != currP) {
                cout << GREEN << "Setting Permission Successful\n" << RESET;
            } else {
                cout << RED << "Setting Permission Error!\n" << RESET;
            }
            cout << "==================================================\n\n";
        }
        void revokePermission() {
            if (cin.peek() == '\n') cin.ignore(10000, '\n');
            string name;
            int uid;
            do {
                cout << GREEN << "Enter Username to Revoke Permission: " << RESET;
                getline(cin, name);
            } while (name.empty());

            if ((uid = checkUserName(name)) == 0) return;

            User &x = users[uid - 1];

            int choice;
            unsigned char p;
            cout << "======================REVOKE======================\n";
            cout << BLUE << "[1]" << YELLOW << " WRITE\n";
            cout << BLUE << "[2]" << YELLOW << " READ\n";
            cout << BLUE << "[3]" << YELLOW << " DELETE\n";
            cout << BLUE << "[4]" << YELLOW << " EXECUTE\n";
            cout << BLUE << "[5]" << YELLOW << " ROOT\n";
            cout << BLUE << "[0]" << YELLOW << " Exit\n" << RESET;
            cout << "==================================================\n";
            cout << RED << "[CHOICE:]> " << RESET;
            while (!(cin >> choice)) {
                cout << RED << "[CHOICE:]> " << RESET;
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
                default: cout << RED << "INVALID INPUT!\n" << RESET; p = 0x00; break;
            }

            x.status &= ~p;
            if (!(x.can(p))) {
                cout << GREEN << "\nRevoking Permission Successful\n" << RESET;
            } else {
                cout << RED << "\nRevoking Permission Error!\n" << RESET;
            }
            cout << "==================================================\n\n";
        }
        void createAccount(vector<User> *x)
        {
            cout << "===============CREATE NEW ACCOUNT=================\n";

            string name, pass, role;
            User a;

            do {
                cout << GREEN << "Enter username: " << RESET;
                getline(cin, name);
            } while (name.empty());
            do {
                cout << GREEN << "Enter password: " << RESET;
                getline(cin, pass);
            } while (pass.empty());
            do {
                cout << GREEN << "Enter role [admin | normal user]: " << RESET;
                getline(cin, role);
            } while (role.empty());

            if (role == "admin") {
                a.username = name;
                a.id = x->size() + 1;
                a.setPassword(pass);
                a.setRole(ROLE_ADMIN);
                a.status |= ROOT;
                x->emplace_back(a);
                cout << "==================================================\n\n";
            } else if (role == "normal user") {
                a.username = name;
                a.id = x->size() + 1;
                a.setPassword(pass);
                a.setRole(ROLE_NORMAL_USER);
                x->emplace_back(a);
                cout << "==================================================\n\n";
            } else {
                cout << RED << "INVALID USER TYPE!\n" << RESET;
                cout << "==================================================\n\n";
                return;
            }
        }

        void listAllUsers() {
            cout << "\n";
            for (auto &u : users) {
                u.showPermissions();
            }
            cout << "\n";
        }
};

unsigned int login() {
    string pass, name;
    //cin.ignore();

    cout << "======================LOGIN=======================\n";
    cout << GREEN << "Username: " << RESET;
    getline(cin, name);

    unsigned int id = checkUserName(name);
    if (id == 0) {
        cout << RED << "USER NOT FOUND\n" << RESET;
        cout << "==================================================\n\n";
        return 0;
    }

    cout << GREEN << "Password: " << RESET;
    getline(cin, pass);

    // USE THE ID TO FIND THE ACTUAL USER IN THE VECTOR
    if (users[id - 1].checkPassword(pass)) {
        cout << GREEN << "Access Granted! Welcome, " << name << RESET << endl;
        cout << "==================================================\n\n";
        return id;
    } else {
        cout << RED << "INCORRECT PASSWORD\n" << RESET;
        cout << "==================================================\n\n";
        return 0;
    }
}

int main() {
    int choice;
    unsigned int uid;

    cout << "==================================================\n";
    cout << BLUE << " WELCOME TO FILE PERMISSION SIMULATOR V 1.0.1 \n" << RESET;
    cout << "==================================================\n";

    Admin adminTool;

    while (true) {
        cout << RED << "[1]" << GREEN << " Login\n";
        cout << RED << "[2]" << GREEN << " Create Account\n";
        cout << RED << "[0]" << GREEN << " Exit\n" << RESET;
        cout << "==================================================\n";
        cout << RED << "[CHOICE:]> " << RESET;
        if (!(cin >> choice)) {
            cout << RED << "\nINVALID CHOICE!\n" << RESET;
            cin.ignore(10000, '\n');
            continue;
        }

        // THE FIX: Clean the "trash" immediately after the input
        cin.ignore(10000, '\n');

        if (choice == 1) {
            // login logic
            uid = login();
            if (uid == 0){
                cout << RED << "LOG IN FAILED!\n" << RESET;
                continue;
            }
            if (users[uid - 1].isAdmin()) {
                Admin admin;
                admin.username = users[uid - 1].username;
                admin.status = users[uid - 1].status;
                admin.id = users[uid - 1].id;

                while (1) {
                    cout << GREEN << "ADMIN DASHBOARD\n" << RESET;
                    cout << "==================================================\n";
                    cout << RED << "[1] " << GREEN << "Set Permission\n";
                    cout << RED << "[2] " << GREEN << "Toggle Permission\n";
                    cout << RED << "[3] " << GREEN << "Revoke Permission\n";
                    cout << RED << "[4] " << GREEN << "Create New Account\n";
                    cout << RED << "[5] " << GREEN << "List All Users\n";
                    cout << RED << "[0] " << GREEN << "Logout\n" << RESET;
                    cout << "==================================================\n";
                    cout << GREEN << "Enter Choice: " << RESET;
                    while (!(cin >> choice)) {
                        cout << GREEN << "Enter Choice: " << RESET;
                        cin.clear();
                        cin.ignore(10000, '\n');
                    }
                    cin.ignore(10000, '\n');
                    if (choice == 0) {
                        cout << GREEN << "Log Out Successful\n" << RESET;
                        cout << "==================================================\n\n";
                        break;
                    }
                    switch (choice) {
                        case 1: admin.setPermission(); break;
                        case 2: admin.togglePermission(); break;
                        case 3: admin.revokePermission(); break;
                        case 4: admin.createAccount(&users);  break;
                        case 5: admin.listAllUsers(); break;
                        default: cout << RED << "INVALID CHOICE!\n" << RESET;
                    }
                }
            }
            else if (users[uid - 1].isNormalUser()) {
                NormalUser user;
                user.username = users[uid - 1].username;
                user.status = users[uid - 1].status;
                user.id = users[uid - 1].id;

                while (1) {
                    // normal user logic
                    cout << GREEN << "NORMAL USER DASHBOARD\n" << RESET;
                    cout << "==================================================\n";
                    cout << RED << "[1] " << GREEN << "READ FILE\n";
                    cout << RED << "[2] " << GREEN << "wRITE TO FILE\n";
                    cout << RED << "[3] " << GREEN << "DELETE FILE\n";
                    cout << RED << "[4] " << GREEN << "EXECUTE FILE\n";
                    cout << RED << "[5] " << GREEN << "SEE PERMISSIONS\n";
                    cout << RED << "[0] " << GREEN << "Logout\n" << RESET;
                    cout << "==================================================\n";
                    cout << GREEN << "Enter Choice: " << RESET;
                    while (!(cin >> choice)) {
                        cout << GREEN << "Enter Choice: " << RESET;
                        cin.clear();
                        cin.ignore(10000, '\n');
                    }
                    cin.ignore(10000, '\n');
                    if (choice == 0) {
                        cout << GREEN << "Log Out Successful\n" << RESET;
                        cout << "==================================================\n\n";
                        break;
                    }
                    switch(choice) {
                        case 1: user.action(READ); break;
                        case 2: user.action(WRITE); break;
                        case 3: user.action(DELETE); break;
                        case 4: user.action(EXECUTE); break;
                        case 5: user.showPermissions(); break;
                        default: cout << RED << "INVALID CHOICE!\n" << RESET; break;
                    }
                }
            }
            else {
                cout << RED << "\nUNDEFINED USER!n" << RESET;
                break;
            }
        }
        else if (choice == 2) {
            cout << "\n";
            adminTool.createAccount(&users); // This will now show the prompt INSTANTLY
        } else {
            cout << GREEN << "EXITING APPLICATION....\n" << RESET;
            break;
        }
    }

    return 0;
}