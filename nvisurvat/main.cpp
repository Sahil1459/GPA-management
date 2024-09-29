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

sql::mysql::MySQL_Driver* driver; // Global variables to establish connection between Database and program
sql::Connection* con;
sql::Statement* stmt;

class common_variables { //Class used for variables which are used through out the program
public:
    int choice;
};

void clearscreen() {
    system("cls");
}

class admin : public common_variables {
private:
    string fname, mname, lname;
    string department;
    string year;
    string emailid;
    string birthdate;
    string address;
    long long int enrollmentno;
    long long int mobile;
    int teacherid;
public:
    admin() {
        try {
            driver = sql::mysql::get_mysql_driver_instance();
            con = driver->connect("tcp://127.0.0.1:3306", "root", "12345"); // Exception handling by using try-catch statement
            con->setSchema("GPA_db"); 
        }
        catch (sql::SQLException& e) {
            cerr << "SQL Error: " << e.what() << endl; // displaying  error received from database
            exit(EXIT_FAILURE);
        }
    }

    void adminmenu() {
        cout << "Admin Menu" << endl << endl;
        cout << "1. Register Student" << endl;
        cout << "2. View Student list" << endl;
        cout << "3. Delete Student" << endl;
        cout << "4. Add Teacher" << endl;
        cout << "5. View Teacher list" << endl;
        cout << "6. Delete Teacher" << endl;
        cout << "7. Log out" << endl;
        cout << "Enter choice: ";
        cin >> choice;
        clearscreen();

        if (choice == 1) {
            registerStudent();
        }
        else if (choice == 2) {
            showStudentList();
        }
        else if (choice == 3) {
            deleteStudent();
        }
        else if (choice == 4) {
            registerTeacher();
        }
        else if (choice == 5) {
            showTeacherList();
        }
        else if (choice == 6) {
            deleteTeacher();
        }
        else if (choice == 7) {
            cout << "Log out" << endl;
            exit(EXIT_SUCCESS);
        }
    }

    bool registerStudent() {
        cout << "Enter first name: ";
        cin >> fname;
        cout << "Enter middle name: ";
        cin >> mname;
        cout << "Enter last name: ";
        cin >> lname;
        cout << "Enter enrollment number: ";
        cin >> enrollmentno;
        cout << "Enter department (IF / CO / EJ / CE / ME / EE /AE): ";
        cin >> department;
        cout << "Enter year (FY / SY / TY): ";
        cin >> year;
        cout << "Enter email id: ";
        cin >> emailid;
        cout << "Enter mobile number: ";
        cin >> mobile;
        cout << "Enter birthdate (dd/mm/yyyy): ";
        cin >> birthdate;
        cin.ignore();
        cout << "Enter address: ";
        getline(cin, address);

        string password, confirmPassword;
        do {
            cout << "Enter password: ";
            cin >> password;
            cout << "Confirm password: ";
            cin >> confirmPassword;

            if (password != confirmPassword) {
                cout << "Passwords do not match! Please try again." << endl;
            }
        } while (password != confirmPassword);

        try {
            stmt = con->createStatement();
            string insertQuery = "INSERT INTO students (enrollmentno, fname, mname, lname, department, year, emailid, mobile, birthdate, address, password) VALUES (" +
                to_string(enrollmentno) + ",'" + fname + "','" + mname + "','" + lname + "','" + department + "','" + year + "','" + emailid + "'," +
                to_string(mobile) + ",'" + birthdate + "','" + address + "','" + password + "')";
            stmt->execute(insertQuery);

            cout << endl << "Registration successful!" << endl << endl;
            adminmenu();
        }
        catch (sql::SQLException& e) {
            cerr << "SQL Error: " << e.what() << endl;
        }
        return true;
    }
};

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