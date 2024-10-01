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
    string password, confirmPassword;
public:
    admin() {
        try {
            // Initialize the MySQL driver and connection
            driver = sql::mysql::get_mysql_driver_instance();
            con = driver->connect("tcp://127.0.0.1:3306", "root", "12345"); // Change the username and password as needed
            con->setSchema("GPA_db"); // Replace 'student_db' with your database name
        }
        catch (sql::SQLException& e) {
            cerr << "SQL Error: " << e.what() << endl;
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
            //deleteStudent();
        }
        else if (choice == 4) {
            //registerTeacher();
        }
        else if (choice == 5) {
            //showTeacherList();
        }
        else if (choice == 6) {
            //deleteTeacher();
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
        do {                                                                // using do while loop for double confirmation of password
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

    void showStudentList() {
        try {
            std::unique_ptr<sql::Statement> stmt(con->createStatement());

            // SQL query to retrieve student data 
            std::string query = "SELECT enrollmentno, CONCAT(fname, ' ', mname, ' ', lname) AS Name, "
                "department, year, emailid, mobile, birthdate, address "
                "FROM students";

            // Executing the query
            std::unique_ptr<sql::ResultSet> res(stmt->executeQuery(query));
            const int widthEnrollment = 15; // enrollmentno (bigint)
            const int widthName = 30;       // Name (concatenation of fname, mname, lname)
            const int widthDepartment = 10; // department (varchar(2))
            const int widthYear = 6;        // year (varchar(2))
            const int widthEmail = 30;      // emailid (varchar(50))
            const int widthMobile = 15;     // mobile (bigint)
            const int widthBirthdate = 12;  // birthdate (varchar(10))
            const int widthAddress = 35;    // address (varchar(100))

            // Display header without adding extra lines
            std::cout << std::left
                << std::setw(15) << "Enrollment No"
                << std::setw(30) << "Name"
                << std::setw(10) << "Dept"
                << std::setw(6) << "Year"
                << std::setw(30) << "Email ID"
                << std::setw(15) << "Mobile"
                << std::setw(12) << "Birthdate"
                << std::setw(35) << "Address"
                << std::endl;

            // Display separator line
            std::cout << std::string(widthEnrollment, '-')
                << std::string(widthName, '-')
                << std::string(widthDepartment, '-')
                << std::string(widthYear, '-')
                << std::string(widthEmail, '-')
                << std::string(widthMobile, '-')
                << std::string(widthBirthdate, '-')
                << std::string(widthAddress, '-')
                << std::endl;

            // Fetch and display each row of data without adding extra lines
            while (res->next()) {
                std::cout << std::left
                    << std::setw(widthEnrollment) << res->getString("enrollmentno")
                    << std::setw(widthName) << res->getString("Name")
                    << std::setw(widthDepartment) << res->getString("department")
                    << std::setw(widthYear) << res->getString("year")
                    << std::setw(widthEmail) << res->getString("emailid")
                    << std::setw(widthMobile) << res->getString("mobile")
                    << std::setw(widthBirthdate) << res->getString("birthdate")
                    << std::setw(widthAddress) << res->getString("address")
                    << std::endl;
            }
            int choice;
            cout << endl << endl;
            cout << "1. Back to admin menu" << endl;
            cout << "2. Logout" << endl;
            cout << "Enter a option:" << endl;;
            cin >> choice;
            cout << endl << endl;
            clearscreen();
            if (choice == 1) {
                adminmenu();
            }
            else if (choice == 2) {
                exit(0);

            }
            else {
                cout << "Invalid choice. Please try again." << endl;
                showStudentList();
            }


        }
        catch (sql::SQLException& e) {
            std::cerr << "SQLException: " << e.what() << std::endl;
            std::cerr << "MySQL error code: " << e.getErrorCode() << std::endl;
            std::cerr << "SQLState: " << e.getSQLState() << std::endl;
        }
    }
};

void mainscreen() {
    admin a;
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
            a.adminmenu();
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