#include <iostream>                         //for  input output
#include <string>                           //  for  string
#include <cstdlib>                          // for  exit  
#include <iomanip>                          // For output Formatting
#include <cppconn/driver.h>                 // for  mysql connector
#include <cppconn/exception.h>              // for  mysql exception
#include <cppconn/statement.h>              // for  mysql statement
#include <cppconn/prepared_statement.h>
#include <cppconn/resultset.h>
#include <mysql_driver.h>
#include <mysql_connection.h>

using namespace std;

class common_variables {
public:
    int choice;
};

void clearscreen() {
    system("cls");
}

void mainscreen() {
    common_variables var;
    string username, password;
    cout << "Welcome to Government Polytechnic Awasari" << endl << endl
        << "1. Student login" << endl
        << "2. Teacher login" << endl
        << "3. Admin login" << endl
        << "4. Exit" << endl
        << "Enter your choice: ";
    cin >> var.choice;
    cout << endl;
    clearscreen();
    if (var.choice == 1) {
        cout << "Service under development!" << endl;
        mainscreen();
    }

    if (var.choice == 2) {
        cout << "Service under development!" << endl;
        mainscreen();
    }
    if (var.choice == 3) {
    adminreenter:
        cout << "   Admin login   " << endl << endl;
        cout << "Enter admin username: ";
        cin >> username;
        cout << endl;
        cout << "Enter password: ";
        cin >> password;
        cout << endl;
        cout << "Logging in . . .";
        cout << endl << endl;
        clearscreen();

        if (username == "admin" && password == "admin") {
            cout << "Admin login successful!" << endl << endl;
            //a.adminmenu();
        }
        else {
            cout << "Invalid username or password!" << endl << endl;
            cout << "1. Try again" << endl;
            cout << "2. Back to main menu" << endl;
            cout << "Enter your choice: ";
            int choice2;
            cin >> choice2;
            clearscreen();
            if (choice2 == 1) {
                goto adminreenter;
            }
            else {
                mainscreen();
            }
        }
    }
    if (var.choice == 4) {
        exit(0);
    }
}





int main() {
    mainscreen();
    return 0;
}