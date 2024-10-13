#include <iostream>                         //for  input output
#include <string>                           //  for  string
#include <cstdlib>                          // for  exit  
#include <iomanip>                          // For output Formatting
#include <string>                           // for string related  functions
#include <mysql_driver.h>                   // for mysql driver object
#include <mysql_connection.h>               // for mysql connection object                  
#include <cppconn/statement.h>              // for mysql command passing parameters
#include <cppconn/prepared_statement.h>     // for prepared statement
#include <cppconn/resultset.h>              // for retreving the resul set from database    
#include <cppconn/exception.h>              // for hadnling error coming through database           
           
using namespace std;
void mainscreen();                  // Declaration of mainscreen function

sql::mysql::MySQL_Driver* driver; // Global variables to establish connection between Database and program
sql::Connection* con;
sql::PreparedStatement* pstmt;
sql::ResultSet* res;

class Database {            // Database class for managing connection
public:
    sql::Connection* getConnection() {
    sql::mysql::MySQL_Driver* driver = sql::mysql::get_mysql_driver_instance();
    sql::Connection* con = driver->connect("tcp://127.0.0.1:3306", "root", "12345");
        con->setSchema("GPA_db");
        return con;
    }
    static void closeConnection() {
        delete con;
    }
};

class common_variables { //Class used for variables which are used through out the program
public:
    int choice;
};
void clearscreen() {            // function from clearinng screen by using sysytem("cls") frm <cstlib> file
    system("cls");
}

class admin : public common_variables {         //This class handles all the functions for the admin and holds variables for the admin
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
    string gender;
    string addmission_date;
    string guardian_name;
    long long int guardian_mobile;
    string blood_grp;
    string nationality;
    string category;
    string aadhar_number;
    string joining_date;
    int experience_years;

public:
	void adminmenu() {                                          //Displays the admin menu
        cout << "Welcome to Admin Dashboard" << endl << endl;
        admin_choice_re:
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
            registerStudent();                      //else_if ladder as subtitute of switch case statement
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
            cout << "Logged out" << endl;
            mainscreen();
        }
        else {
			cout << "Enter a Valid option !";       //handle a exception if user entered a wrong option
            goto admin_choice_re;
        }
    }

	bool registerStudent() {                        // Function to register a student 
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
        cout << "Enter Gender (M/F):";
        cin >> gender;
        cout << "Enter admission date:";
        cin >> addmission_date;
        cout << "Enter Guardian Full name:";
        cin.ignore();
        getline(cin, guardian_name);
        cout << "Enter guardian mobile no. :";
        cin >> guardian_mobile;
        cout << "Enter blood group:";
        cin >> blood_grp;
        cout << "Enter nationality:";
        cin >> nationality;
        cout << "Enter category:";
        cin >> category;
        cout << "Enter aadhar number:";
        cin >> aadhar_number;
        cout << "Enter address: ";
        cin.ignore();
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
            unique_ptr<sql::Statement> stmt(con->createStatement());                             //createing statement object for query
            string insertQuery = "INSERT INTO students (enrollmentno, fname, mname, lname, department, year, emailid, mobile, birthdate, address, password, gender, admission_date, guardian_name,guardian_contact, blood_group, nationality, category, aadhar_number) VALUES (" +
                to_string(enrollmentno) + ",'" + fname + "','" + mname + "','" + lname + "','" + department + "','" + year + "','" + emailid + "'," +
                to_string(mobile) + ",'" + birthdate + "','" + address + "','" + password + "', '" + gender + "', '" + addmission_date + "', '" + guardian_name + "', " + to_string(guardian_mobile) + ", '" + blood_grp + "', '" + nationality + "', '" + category + "', '" + aadhar_number + "')";
			stmt->execute(insertQuery);     //by using execute function query is sent to Database engine

            cout << endl << "Registration successful!" << endl << endl;
        register_re:
            cout << "1. Add another student" << endl
                << "2. Back to admin menu" << endl
                << "Enter a option:";
            cin >> choice;
            if (choice == 1) {
                clearscreen();
                registerStudent();

            }
            else if (choice == 2) {
                clearscreen();
                adminmenu();
            }
            else {
                cout << "Enter a valid option !";
                goto register_re;
            }
            clearscreen();
            adminmenu();
        }
		catch (sql::SQLException& e) {                      //this block prints the error message given by database engine
            cerr << "SQL Error: " << e.what() << endl;
			registerStudent(); // calls registerStudent() again to register the student
        }
        return true;
    }

    void showStudentList() {
        try {
            unique_ptr<sql::Statement> stmt(con->createStatement());

            // SQL query to retrieve student data with additional fields
            string query = "SELECT enrollmentno, CONCAT(fname, ' ', mname, ' ', lname) AS Name, "
                "department, year, emailid, mobile, birthdate, address, gender, admission_date, "
                "guardian_name, guardian_contact, blood_group, nationality, category, aadhar_number "
                "FROM students";

            // Executing the query
            unique_ptr<sql::ResultSet> res(stmt->executeQuery(query));

            // Column width settings based on your requirements
            const int widthEnrollment = 15;   // enrollmentno (bigint)
            const int widthName = 27;         // Name (concatenation of fname, mname, lname)
            const int widthDepartment = 7;    // department (varchar(2)) reduced space between dept & name
            const int widthYear = 5;          // year (varchar(2)) reduced gap between dept & year
            const int widthEmail = 27;        // emailid (varchar(50)) reduced gap between email & mobile
            const int widthMobile = 15;       // mobile (bigint)
            const int widthBirthdate = 12;    // birthdate (varchar(10))
            const int widthAddress = 20;      // address (varchar(100)), nearby 20 characters as you requested
            const int widthGender = 8;        // gender (varchar(10))
            const int widthAdmissionDate = 12;// admission_date (date)
            const int widthGuardianName = 25; // guardian_name (varchar(50))
            const int widthGuardianContact = 15; // guardian_contact (bigint)
            const int widthBloodGroup = 5;    // blood_group (varchar(3))
            const int widthNationality = 15;  // nationality (varchar(30))
            const int widthCategory = 12;     // category (varchar(20))
            const int widthAadharNumber = 15; // aadhar_number (bigint)

            // Display header
            cout << left
                << setw(widthEnrollment) << "Enrollment No"
                << setw(widthName) << "Name"
                << setw(widthDepartment) << "Dept"
                << setw(widthYear) << "Year"
                << setw(widthEmail) << "Email ID"
                << setw(widthMobile) << "Mobile"
                << setw(widthBirthdate) << "Birthdate"
                << setw(widthAddress) << "Address"
                << setw(widthGender) << "Gender"
                << setw(widthAdmissionDate) << "Admission Date"
                << setw(widthGuardianName) << "Guardian Name"
                << setw(widthGuardianContact) << "Guardian Contact"
                << setw(widthBloodGroup) << "Blood Group"
                << setw(widthNationality) << "Nationality"
                << setw(widthCategory) << "Category"
                << setw(widthAadharNumber) << "Aadhar No"
                << endl;

            // Display separator line
            cout << string(widthEnrollment, '-')
                << string(widthName, '-')
                << string(widthDepartment, '-')
                << string(widthYear, '-')
                << string(widthEmail, '-')
                << string(widthMobile, '-')
                << string(widthBirthdate, '-')
                << string(widthAddress, '-')
                << string(widthGender, '-')
                << string(widthAdmissionDate, '-')
                << string(widthGuardianName, '-')
                << string(widthGuardianContact, '-')
                << string(widthBloodGroup, '-')
                << string(widthNationality, '-')
                << string(widthCategory, '-')
                << string(widthAadharNumber, '-')
                << endl;

            // Fetch and display each row of data
            while (res->next()) {
                cout << left
                    << setw(widthEnrollment) << res->getString("enrollmentno")
                    << setw(widthName) << res->getString("Name")
                    << setw(widthDepartment) << res->getString("department")
                    << setw(widthYear) << res->getString("year")
                    << setw(widthEmail) << res->getString("emailid")
                    << setw(widthMobile) << res->getString("mobile")
                    << setw(widthBirthdate) << res->getString("birthdate")
                    << setw(widthAddress) << res->getString("address")
                    << setw(widthGender) << res->getString("gender")
                    << setw(widthAdmissionDate) << res->getString("admission_date")
                    << setw(widthGuardianName) << res->getString("guardian_name")
                    << setw(widthGuardianContact) << res->getString("guardian_contact")
                    << setw(widthBloodGroup) << res->getString("blood_group")
                    << setw(widthNationality) << res->getString("nationality")
                    << setw(widthCategory) << res->getString("category")
                    << setw(widthAadharNumber) << res->getString("aadhar_number")
                    << endl;
            }

            int choice;
            cout << endl << endl;
            cout << "1. Back to admin menu" << endl;
            cout << "2. Refresh" << endl;
            cout << "Enter an option:" << endl;
            cin >> choice;
            cout << endl << endl;
            clearscreen();

            if (choice == 1) {
                adminmenu();
            }
            else if (choice == 2) {
                showStudentList();
            }
            else {
                cout << "Invalid choice. Please try again." << endl;
                showStudentList();
            }

        }
        catch (sql::SQLException& e) {
            cerr << "SQLException: " << e.what() << endl;
            cerr << "MySQL error code: " << e.getErrorCode() << endl;
            cerr << "SQLState: " << e.getSQLState() << endl;
        }
    }



    void deleteStudent() {
        unique_ptr<sql::Statement> stmt(con->createStatement());
        cout << "Enter enrollment number of the student to delete: ";
        cin >> enrollmentno;

        try {
            
            string deleteQuery = "DELETE FROM students WHERE enrollmentno = " + to_string(enrollmentno);
            stmt->execute(deleteQuery);

            cout << "Student record deleted successfully!" << endl;
        }
        catch (sql::SQLException& e) {
            cerr << "SQL Error: " << e.what() << endl;
        }
        int choice;
        cout << endl << endl;
        cout << "1. Delete another student" << endl;
        cout << "2. Back to admin menu" << endl;
        cout << "Enter a option:" << endl;
        cin >> choice;
        cout << endl << endl;
        clearscreen();
        if (choice == 1) {
            deleteStudent();
        }
        if (choice == 2) {
            adminmenu();
        }


    }

    void registerTeacher() {
        cout << "Enter first name: ";
        cin >> fname;
        cout << "Enter middle name: ";
        cin >> mname;
        cout << "Enter last name: ";
        cin >> lname;
        cout << "Enter email id: ";
        cin >> emailid;
        cout << "Enter mobile number: ";
        cin >> mobile;
        cout << "Enter department (IF / CO / EJ / CE / ME / EE /AE): ";
        cin >> department;
        cout << "Enter birthdate (dd/mm/yyyy): ";
        cin >> birthdate;
        cin.ignore();
        cout << "Enter address: ";
        getline(cin, address);
        cout << "Enter joining date:";
        cin >> joining_date;
        cout << "Enter gender :";
        cin >> gender;
        cout << "Enter experience years :";
        cin >> experience_years;
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
            unique_ptr<sql::Statement> stmt(con->createStatement());
            string insertQuery = "INSERT INTO teachers (teacherid, fname, mname, lname, department, emailid, mobile, birthdate, address, joining_date, gender, experience_years,password) VALUES (" +
                to_string(teacherid) + ",'" + fname + "','" + mname + "','" + lname + "','" + department + "','" + emailid + "'," +
                to_string(mobile) + ",'" + birthdate + "','" + address + "', '" + joining_date + "','" + gender + "'," + to_string(experience_years) + ",'" + password + "')";
            stmt->execute(insertQuery);

            cout << endl << endl << "Teacher registered successfully!" << endl;
        }
        catch (sql::SQLException& e) {
            cerr << "SQL Error: " << e.what() << endl;
        }

        cout << endl << endl
            << "1. Add another Teacher" << endl
            << "2. Back to admin menu" << endl
            << "Enter a option:";
        cin >> choice;
        clearscreen();

        if (choice == 1) {
            clearscreen();
            registerTeacher();
        }

        if (choice == 2) {
            clearscreen();
            adminmenu();
        }



    }

    void showTeacherList() {
        try {
            unique_ptr<sql::Statement> stmt(con->createStatement());

            // SQL query to retrieve teacher data
            string query = "SELECT teacherid, CONCAT(fname, ' ', mname, ' ', lname) AS Name, "
                "department, emailid, mobile, birthdate, address, joining_date, gender, experience_years "
                "FROM teachers";

            // Executing the query
            unique_ptr<sql::ResultSet> res(stmt->executeQuery(query));

            // Define column widths
            const int widthTeacherID = 12; // teacherid (int)
            const int widthName = 30;       // Name (concatenation of fname, mname, lname)
            const int widthDepartment = 10; // department (varchar(2))
            const int widthEmail = 30;      // emailid (varchar(50))
            const int widthMobile = 15;     // mobile (bigint)
            const int widthBirthdate = 12;  // birthdate (varchar(10))
            const int widthAddress = 40;    // address (varchar(200))
            const int widthJoiningDate = 12; // joining_date (date)
            const int widthGender = 10;     // gender (varchar(10))
            const int widthExperience = 15;  // experience_years (int)

            // Display header without adding extra lines
            cout << left
                << setw(widthTeacherID) << "Teacher ID"
                << setw(widthName) << "Name"
                << setw(widthDepartment) << "Dept"
                << setw(widthEmail) << "Email ID"
                << setw(widthMobile) << "Mobile"
                << setw(widthBirthdate) << "Birthdate"
                << setw(widthJoiningDate) << "Joining Date"
                << setw(widthGender) << "Gender"
                << setw(widthExperience) << "Experience (Years)"
                << endl;

            // Display separator line
            cout << string(widthTeacherID, '-')
                << string(widthName, '-')
                << string(widthDepartment, '-')
                << string(widthEmail, '-')
                << string(widthMobile, '-')
                << string(widthBirthdate, '-')
                << string(widthJoiningDate, '-')
                << string(widthGender, '-')
                << string(widthExperience, '-')
                << endl;

            // Fetch and display each row of data without adding extra lines
            while (res->next()) {
                cout << left
                    << setw(widthTeacherID) << res->getInt("teacherid")
                    << setw(widthName) << res->getString("Name")
                    << setw(widthDepartment) << res->getString("department")
                    << setw(widthEmail) << res->getString("emailid")
                    << setw(widthMobile) << res->getInt("mobile")
                    << setw(widthBirthdate) << res->getString("birthdate")
                    << setw(widthJoiningDate) << res->getString("joining_date")
                    << setw(widthGender) << res->getString("gender")
                    << setw(widthExperience) << res->getInt("experience_years")
                    << endl;
            }

            // Prompt for next action
            int choice;
            cout << endl << "1. Back to admin menu" << endl;
            cout << "2. Refresh" << endl;
            cout << "Enter your option: ";
            cin >> choice;
            cout << endl;

            clearscreen(); // Assuming clearscreen() is defined elsewhere
            if (choice == 1) {
                adminmenu(); // Assuming adminmenu() is defined elsewhere
            }
            else if (choice == 2) {
                showTeacherList(); // Refresh the list
            }
            else {
                cout << "Invalid choice. Please try again." << endl;
                showTeacherList(); // Retry showing the list
            }
        }
        catch (sql::SQLException& e) {
            cerr << "SQLException: " << e.what() << endl;
            cerr << "MySQL error code: " << e.getErrorCode() << endl;
            cerr << "SQLState: " << e.getSQLState() << endl;
        }
    }


    void deleteTeacher() {
        cout << "Enter teacher ID of the teacher to delete: ";
        cin >> teacherid;

        try {
            unique_ptr<sql::Statement> stmt(con->createStatement());
            string deleteQuery = "DELETE FROM teachers WHERE teacherid = " + to_string(teacherid);
            stmt->execute(deleteQuery);

            cout << "Teacher record deleted successfully!" << endl;
        }
        catch (sql::SQLException& e) {
            cerr << "SQL Error: " << e.what() << endl;
        }
        cout << endl << endl
            << "1. Delete another teacher" << endl
            << "2. Back to admin menu" << endl
            << "Enter a option:";
        cin >> choice;
        clearscreen();

        if (choice == 1) {
            deleteTeacher();

        }

        if (choice == 2) {
            adminmenu();
        }
    }

};

class teacher : public common_variables {
private:
    string emailid,password,department,fname,lname;
public:
    void teachermenu() {
        clearscreen();
        cout << "Welcome " << fname << "!" << endl << endl <<endl;
        cout << "Teacher Dashboard" << endl << endl;
        cout << "1. View my class" << endl
            << "2. Etc" << endl 
            << "Enter a option:";
        cin >> choice;

        if (choice == 1) {
            //viewmyclass();
        }
    }
    bool loginTeacher(string& emailid, string& password) {
        int choice;
            try {
                pstmt = con->prepareStatement("SELECT password, department, fname, lname FROM teachers WHERE emailid = ?");
                pstmt->setString(1, emailid);

                res = pstmt->executeQuery();

                if (res->next()) {
                    string storedPassword = res->getString("password");
                    department = res->getString("department");
                    fname = res->getString("fname");
                    lname = res->getString("lname");

                    if (storedPassword == password) {
                        // If the password is correct, go to the teacher menu
                        teachermenu();
                        delete res;
                        delete pstmt;
                        delete con;
                        return true;  // Login success
                    }
                    else {
                        // If the password is incorrect
                        cout << "Incorrect password." << endl << endl;
                        cout << "1. Go back to main menu" << endl
                            << "Enter an option: ";
                        cin >> choice;

                        if (choice == 1) {
                            clearscreen();
                            mainscreen();
                        }
                        else {
                            cout << "Invalid option, please try again." << endl;
                        }
                    }
                }
                else {
                    // If the email is not found
                    cout << "Email ID not found.\n";
                    cout << "1. Go back to main menu" << endl
                        << "Enter a option:";

                    cin >> choice;
                    if (choice == 1) {
                        clearscreen();
                        mainscreen();

                    }
                    else {
                        cout << "Invalid option, please try again." << endl;
                    }
                }

               
                delete res;
                delete pstmt;

            }
            catch (sql::SQLException& e) {
                std::cerr << "Error during login: " << e.what() << std::endl;
            }
        

        delete con;  // Ensure connection is closed when exiting the loop
        return false;  // Login failed
    }
    
    void viewMyclass() {
		cout << "My classes" << endl;
		cout << "1. FY" << endl
            << "2. SY" << endl
			<< "3. TY" << endl
			<< "Enter a option:";
		cin >> choice;
	}
};


void mainscreen() {
    admin a;
    teacher t;
    common_variables var;
    string username, password;
mainre:
    cout << "Welcome to Government Polytechnic Awasari" << endl << endl
        << "1. Student login" << endl
        << "2. Teacher login" << endl
        << "3. HOD login" << endl
        << "4. Admin login" << endl
        << "5. Exit" << endl
        << "Enter your choice: ";
    cin >> var.choice;
    cout << endl;
    clearscreen();
    if (var.choice == 1) {
        cout << "Service under development!" << endl;
        mainscreen();
    }

    else if (var.choice == 2) {
        string emailid,pass;
        cout << "Enter your email id : ";
        cin >> emailid;
        cout << "Enter password :";
        cin >> pass;
        t.loginTeacher(emailid,pass);
    }
    else if (var.choice == 4) {
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
    else if (var.choice == 5) {
        exit(0);
    }
    else if (var.choice == 3) {
        cout << "Service under development!" << endl;
        mainscreen();
    }
    else {
        cout << "Enter a valid choice !" << endl << endl;
        goto mainre;
    }
}





int main() {
    mainscreen();
    return 0;
}