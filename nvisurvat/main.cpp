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
void mainscreen(); // Declaration of mainscreen function

sql::mysql::MySQL_Driver* driver; // Global variables to establish connection between Database and program
sql::Connection* con;
sql::PreparedStatement* pstmt;
sql::ResultSet* res;

class Database { // Database class for managing connection


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
public: int choice;
};
void clearscreen() { // function from clearinng screen by using sysytem("cls") frm <cstlib> file
    system("cls");
}

class admin : public common_variables { //This class handles all the functions for the admin and holds variables for the admin
protected:
    string fname, mname, lname;
    string department;
    string year;
    string emailid;
    string birthdate;
    string address;
    long long int enrollmentno;
    long long int mobile;
    int faculty_id;
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
    string post;

public:
    void adminmenu() { //Displays the admin menu
        cout << "Welcome to Admin Dashboard" << endl << endl;
    admin_choice_re:
        cout << "1. Register Student" << endl;
        cout << "2. View Student list" << endl;
        cout << "3. Delete Student" << endl;
        cout << "4. Add Faculty" << endl;
        cout << "5. View Faculty list" << endl;
        cout << "6. Delete Faculty" << endl;
        cout << "7. Log out" << endl;
        cout << "Enter choice: ";
        cin >> choice;
        clearscreen();

        if (choice == 1) {
            registerStudent(); //else_if ladder as subtitute of switch case statement
        }
        else if (choice == 2) {
            showStudentList();
        }
        else if (choice == 3) {
            deleteStudent();
        }
        else if (choice == 4) {
            registerFaculty();
        }
        else if (choice == 5) {
            showFacultyList();
        }
        else if (choice == 6) {
            deleteFaculty();
        }
        else if (choice == 7) {
            cout << "Logged out" << endl;
            mainscreen();
        }
        else {
            cout << "Enter a Valid option !"; //handle a exception if user entered a wrong option
            goto admin_choice_re;
        }
    }

    bool registerStudent() { // Function to register a student 
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
        cout << "Enter birthdate (dd/mm/yy): ";
        cin >> birthdate;
        cin.ignore();
        cout << "Enter Gender (Male/Female/Other):";
        cin >> gender;
        cout << "Enter admission date(dd/mm/yy):";
        cin >> addmission_date;
        cout << "Enter Guardian Full name:";
        cin.ignore();
        getline(cin, guardian_name);
        cout << "Enter guardian mobile no. :";
        cin >> guardian_mobile;
        cout << "Enter blood group (A+/A-/B+/B-/AB+/AB-/O+/O-):";
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
        do { // using do while loop for double confirmation of password
            cout << "Enter password: ";
            cin >> password;
            cout << "Confirm password: ";
            cin >> confirmPassword;

            if (password != confirmPassword) {
                cout << "Passwords do not match! Please try again." << endl;
            }
        } while (password != confirmPassword);

        try {

            unique_ptr < sql::Statement > stmt(con->createStatement()); //createing statement object for query

            // SQL query to create a table 

            string insertQuery = "INSERT INTO students_info (enrollmentno, fname, mname, lname, department, year, emailid, mobile, birthdate, address, password, gender, admission_date, guardian_name,guardian_contact, blood_group, nationality, category, aadhar_number) VALUES (" +
                to_string(enrollmentno) + ",'" + fname + "','" + mname + "','" + lname + "','" + department + "','" + year + "','" + emailid + "'," +
                to_string(mobile) + ",'" + birthdate + "','" + address + "','" + password + "', '" + gender + "', '" + addmission_date + "', '" + guardian_name + "', " + to_string(guardian_mobile) + ", '" + blood_grp + "', '" + nationality + "', '" + category + "', '" + aadhar_number + "')";
            stmt->execute(insertQuery); //by using execute function query is sent to Database engine

            cout << endl << "Registration successful!" << endl << endl;
        register_re:
            cout << "1. Add another student" << endl <<
                "2. Back to admin menu" << endl <<
                "Enter a option:";
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
        catch (sql::SQLException& e) { //this block prints the error message given by database engine
            cerr << "SQL Error: " << e.what() << endl;
            registerStudent(); // calls registerStudent() again to register the student
        }
        return true;
    }

    void showStudentList() {
        try {
            unique_ptr < sql::Statement > stmt(con->createStatement());

            // SQL query to retrieve student data with additional fields
            string query = "SELECT enrollmentno, CONCAT(fname, ' ', mname, ' ', lname) AS Name, "
                "department, year, emailid, mobile, birthdate, address, gender, admission_date, "
                "guardian_name, guardian_contact, blood_group, nationality, category, aadhar_number "
                "FROM students_info";

            // Executing the query
            unique_ptr < sql::ResultSet > res(stmt->executeQuery(query));

            // Column width settings based on your requirements
            const int widthEnrollment = 15; // enrollmentno (bigint)
            const int widthName = 25; // Name (concatenation of fname, mname, lname)
            const int widthDepartment = 6; // department (varchar(2)) reduced space between dept & name
            const int widthYear = 6; // year (varchar(2)) reduced gap between dept & year
            const int widthEmail = 27; // emailid (varchar(50)) reduced gap between email & mobile
            const int widthMobile = 12; // mobile (bigint)
            const int widthBirthdate = 12; // birthdate (varchar(10))
            const int widthAddress = 30; // address (varchar(100)), nearby 20 characters as you requested
            const int widthGender = 8; // gender (varchar(10))
            const int widthAdmissionDate = 18; // admission_date (date)
            const int widthGuardianName = 20; // guardian_name (varchar(50))
            const int widthGuardianContact = 19; // guardian_contact (bigint)
            const int widthBloodGroup = 10; // blood_group (varchar(3))
            const int widthNationality = 13; // nationality (varchar(30))
            const int widthCategory = 10; // category (varchar(20))
            const int widthAadharNumber = 15; // aadhar_number (bigint)

            // Display header
            cout << left <<
                setw(widthEnrollment) << "Enrollment No" <<
                setw(widthName) << "Name" <<
                setw(widthDepartment) << "Dept" <<
                setw(widthYear) << "Year" <<
                setw(widthEmail) << "Email ID" <<
                setw(widthMobile) << "Mobile" <<
                setw(widthBirthdate) << "Birthdate" <<
                setw(widthAddress) << "Address" <<
                setw(widthGender) << "Gender" <<
                setw(widthAdmissionDate) << "Admission Date" <<
                setw(widthGuardianName) << "Guardian Name" <<
                setw(widthGuardianContact) << "Guardian Contact" <<
                setw(widthBloodGroup) << "Blood G." <<
                setw(widthNationality) << "Nationality" <<
                setw(widthCategory) << "Category" <<
                setw(widthAadharNumber) << "Aadhar No " <<
                endl;

            // Display separator line
            cout << string(widthEnrollment, '-') <<
                string(widthName, '-') <<
                string(widthDepartment, '-') <<
                string(widthYear, '-') <<
                string(widthEmail, '-') <<
                string(widthMobile, '-') <<
                string(widthBirthdate, '-') <<
                string(widthAddress, '-') <<
                string(widthGender, '-') <<
                string(widthAdmissionDate, '-') <<
                string(widthGuardianName, '-') <<
                string(widthGuardianContact, '-') <<
                string(widthBloodGroup, '-') <<
                string(widthNationality, '-') <<
                string(widthCategory, '-') <<
                string(widthAadharNumber, '-') <<
                endl;

            // Fetch and display each row of data
            while (res->next()) {
                cout << left <<
                    setw(widthEnrollment) << res->getString("enrollmentno") <<
                    setw(widthName) << res->getString("Name") <<
                    setw(widthDepartment) << res->getString("department") <<
                    setw(widthYear) << res->getString("year") <<
                    setw(widthEmail) << res->getString("emailid") <<
                    setw(widthMobile) << res->getString("mobile") <<
                    setw(widthBirthdate) << res->getString("birthdate") <<
                    setw(widthAddress) << res->getString("address") <<
                    setw(widthGender) << res->getString("gender") <<
                    setw(widthAdmissionDate) << res->getString("admission_date") <<
                    setw(widthGuardianName) << res->getString("guardian_name") <<
                    setw(widthGuardianContact) << res->getString("guardian_contact") <<
                    setw(widthBloodGroup) << res->getString("blood_group") <<
                    setw(widthNationality) << res->getString("nationality") <<
                    setw(widthCategory) << res->getString("category") <<
                    setw(widthAadharNumber) << res->getString("aadhar_number") <<
                    endl;
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
        unique_ptr < sql::Statement > stmt(con->createStatement());
        cout << "Enter enrollment number of the student to delete: ";
        cin >> enrollmentno;

        try {

            string deleteQuery = "DELETE FROM students_info WHERE enrollmentno = " + to_string(enrollmentno);
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

    void registerFaculty() {
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
        cout << "Enter birthdate (dd/mm/yy): ";
        cin >> birthdate;
        cin.ignore();
        cout << "Enter address: ";
        getline(cin, address);
        cout << "Enter joining date(dd/mm/yy):";
        cin >> joining_date;
        cout << "Enter gender(Male/Female/Other):";
        cin >> gender;
        cout << "Enter experience years :";
        cin >> experience_years;
        cout << "Enter Post(Faculty/HOD):";
        cin >> post;
        do { // using do while loop for double confirmation of password
            cout << "Enter password: ";
            cin >> password;
            cout << "Confirm password: ";
            cin >> confirmPassword;

            if (password != confirmPassword) {
                cout << "Passwords do not match! Please try again." << endl;
            }
        } while (password != confirmPassword);

        try {
            unique_ptr < sql::Statement > stmt(con->createStatement());
            string createQuery = "CREATE TABLE IF NOT EXISTS faculty_info ("
                "faculty_id INT NOT NULL AUTO_INCREMENT,"
                "fname VARCHAR(20),"
                "mname VARCHAR(20),"
                "lname VARCHAR(20),"
                "department ENUM('IF', 'CO', 'EE', 'EJ', 'ME', 'AE', 'CE'),"
                "emailid VARCHAR(50),"
                "mobile BIGINT,"
                "birthdate DATE,"
                "address VARCHAR(200),"
                "joining_date DATE,"
                "gender ENUM('Male', 'Female', 'Other'),"
                "experience_years INT,"
                "post ENUM('Faculty', 'HOD'),"
                "password VARCHAR(20),"
                "PRIMARY KEY(faculty_id)"")";
            stmt->execute(createQuery);

            string insertQuery = "INSERT INTO faculty_info (faculty_id, fname, mname, lname, department, emailid, mobile, birthdate, address, joining_date, gender, experience_years,password,post) VALUES (" +
                to_string(faculty_id) + ",'" + fname + "','" + mname + "','" + lname + "','" + department + "','" + emailid + "'," +
                to_string(mobile) + ",'" + birthdate + "','" + address + "', '" + joining_date + "','" + gender + "'," + to_string(experience_years) + ",'" + password + "','" + post + "')";
            stmt->execute(insertQuery);

            cout << endl << endl << "Faculty registered successfully!" << endl;
        }
        catch (sql::SQLException& e) {
            cerr << "SQL Error: " << e.what() << endl;
        }

        cout << endl << endl <<
            "1. Add another Faculty" << endl <<
            "2. Back to admin menu" << endl <<
            "Enter a option:";
        cin >> choice;
        clearscreen();

        if (choice == 1) {
            clearscreen();
            registerFaculty();
        }

        if (choice == 2) {
            clearscreen();
            adminmenu();
        }

    }

    void showFacultyList() {
        try {
            unique_ptr < sql::Statement > stmt(con->createStatement());
            string query = "SELECT faculty_id, CONCAT(fname, ' ', mname, ' ', lname) AS Name, "
                "department, emailid, mobile, birthdate, address, joining_date, gender, experience_years,post "
                "FROM faculty_info";

            // Executing the query
            unique_ptr < sql::ResultSet > res(stmt->executeQuery(query));

            // Define column widths
            const int widthFacultyID = 12; // Facultyid (int)
            const int widthName = 25; // Name (concatenation of fname, mname, lname)
            const int widthDepartment = 7; // department (varchar(2))
            const int widthEmail = 25; // emailid (varchar(50))
            const int widthMobile = 15; // mobile (bigint)
            const int widthBirthdate = 15; // birthdate (varchar(10))
            const int widthAddress = 40; // address (varchar(200))
            const int widthJoiningDate = 12; // joining_date (date)
            const int widthGender = 10; // gender (varchar(10))
            const int widthExperience = 20; // experience_years (int)
            const int widthPost = 10;

            // Display header without adding extra lines
            cout << left <<
                setw(widthFacultyID) << "Faculty ID" <<
                setw(widthName) << "Name" <<
                setw(widthDepartment) << "Dept" <<
                setw(widthEmail) << "Email ID" <<
                setw(widthMobile) << "Mobile" <<
                setw(widthBirthdate) << "Birthdate" <<
                setw(widthJoiningDate) << "Joining Date" <<
                setw(widthGender) << "Gender" <<
                setw(widthExperience) << "Experience (Years)" <<
                setw(widthPost) << "Post" <<
                endl;

            // Display separator line
            cout << string(widthFacultyID, '-') <<
                string(widthName, '-') <<
                string(widthDepartment, '-') <<
                string(widthEmail, '-') <<
                string(widthMobile, '-') <<
                string(widthBirthdate, '-') <<
                string(widthJoiningDate, '-') <<
                string(widthGender, '-') <<
                string(widthExperience, '-') <<
                string(widthPost, '-') <<
                endl;

            // Fetch and display each row of data without adding extra lines
            while (res->next()) {
                cout << left <<
                    setw(widthFacultyID) << res->getInt("faculty_id") <<
                    setw(widthName) << res->getString("Name") <<
                    setw(widthDepartment) << res->getString("department") <<
                    setw(widthEmail) << res->getString("emailid") <<
                    setw(widthMobile) << res->getInt("mobile") <<
                    setw(widthBirthdate) << res->getString("birthdate") <<
                    setw(widthJoiningDate) << res->getString("joining_date") <<
                    setw(widthGender) << res->getString("gender") <<
                    setw(widthExperience) << res->getInt("experience_years") <<
                    setw(widthPost) << res->getString("post") <<
                    endl;
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
                showFacultyList(); // Refresh the list
            }
            else {
                cout << "Invalid choice. Please try again." << endl;
                showFacultyList(); // Retry showing the list
            }
        }
        catch (sql::SQLException& e) {
            cerr << "SQLException: " << e.what() << endl;
            cerr << "MySQL error code: " << e.getErrorCode() << endl;
            cerr << "SQLState: " << e.getSQLState() << endl;
        }
    }

    void deleteFaculty() {
        cout << "Enter Faculty ID of the Faculty to delete: ";
        cin >> faculty_id;

        try {
            unique_ptr < sql::Statement > stmt(con->createStatement());
            string deleteQuery = "DELETE FROM faculty_info WHERE faculty_id = " + to_string(faculty_id);
            stmt->execute(deleteQuery);

            cout << "Faculty record deleted successfully!" << endl;
        }
        catch (sql::SQLException& e) {
            cerr << "SQL Error: " << e.what() << endl;
        }
        cout << endl << endl <<
            "1. Delete another Faculty" << endl <<
            "2. Back to admin menu" << endl <<
            "Enter a option:";
        cin >> choice;
        clearscreen();

        if (choice == 1) {
            deleteFaculty();

        }

        if (choice == 2) {
            adminmenu();
        }
    }

};

class Faculty : public common_variables, public Database {
private: string emailid,
    password,
    department,
    fname,
    lname;
       admin a;

public:

    Faculty() {
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
    void Facultymenu() {
        clearscreen();
        cout << "Welcome " << fname << "!" << endl << endl << endl;
        cout << "Faculty Dashboard" << endl << endl;
        cout << "1. View my class" << endl <<
            "2. View Exams" << endl <<
            "3. Logout" << endl << endl;
        "Enter a option:";
        cin >> choice;

        if (choice == 1) {
            viewMyclass();
        }
        else if (choice == 2) {
            cout << "Exam Menu " << endl
                << "1. View Exams " << endl
                << "2. Add grades" << endl
                << "3. View Insights" << endl\
                << "Enter a choice:";
            cin >> choice;
            if (choice == 1) {
                viewExams();
            }
        }
        else if (choice == 9) {
            clearscreen();
            mainscreen();
        }
    }
    bool loginFaculty(string& emailid, string& password) {
        int choice;
        try {
            pstmt = con->prepareStatement("SELECT password, department, fname, lname FROM faculty_info WHERE emailid = ?");
            pstmt->setString(1, emailid);

            res = pstmt->executeQuery();

            if (res->next()) {
                string storedPassword = res->getString("password");
                department = res->getString("department");
                fname = res->getString("fname");
                lname = res->getString("lname");

                if (storedPassword == password) {
                    // If the password is correct, go to the Faculty menu
                    Facultymenu();
                    delete res;
                    delete pstmt;
                    delete con;
                    return true; // Login success
                }
                else {
                    // If the password is incorrect
                    cout << "Incorrect password." << endl << endl;
                Faculty_exception1_re:
                    cout <<
                        "1. Go back to main menu" << endl <<
                        "Enter an option: ";
                    cin >> choice;

                    if (choice == 1) {
                        clearscreen();
                        mainscreen(); // Go back to main menu
                    }
                    else {
                        cout << "Invalid option, please try again." << endl;
                        goto Faculty_exception1_re;
                    }
                }
            }
            else {
            Faculty_exception2_re:
                // If the email is not found
                cout << "Email ID not found.\n";
                cout << "1. Go back to main menu" << endl <<
                    "Enter a option:";

                cin >> choice;
                if (choice == 1) {
                    clearscreen();
                    mainscreen();

                }
                else {

                    cout << "Invalid option, please try again." << endl;
                    goto Faculty_exception2_re;
                }
            }

            // Clean up resources after each query attempt
            delete res;
            delete pstmt;

        }
        catch (sql::SQLException& e) {
            std::cerr << "Error during login: " << e.what() << std::endl;
        }


        delete con; // Ensure connection is closed when exiting the loop
        return false; // Login failed
    }

    void viewMyclass() {
        long long int enrollmentno;
        string guardian_name;
        string guardian_contact;
        int mobile;
        string gender;
        string name;
        string email;
        cout << "1. FY" << endl <<
            "2. SY" << endl <<
            "3. TY" << endl <<
            "Enter your choice: ";
        cin >> choice;

        if (choice == 1) {
            string year = "FY";
            viewMyclassQuery(year);
        }
        else if (choice == 2) {
            string year = "SY";
            viewMyclassQuery(year);
        }
        else if (choice == 3) {
            string year = "TY";
            viewMyclassQuery(year);
        }
    }
    void  viewMyclassQuery(string year) {
        sql::Connection* con = getConnection();
        sql::PreparedStatement* pstmt = con->prepareStatement("SELECT enrollmentno,concat(fname,' ',mname,' ',lname) AS name, emailid,mobile,gender,guardian_name,guardian_contact FROM students_info WHERE department = ? AND year = ?;");
        pstmt->setString(1, department);
        pstmt->setString(2, year);
        sql::ResultSet* res = pstmt->executeQuery();
        cout << left <<
            setw(15) << "Enrollment No: " <<
            setw(30) << "Name: " <<
            setw(25) << "Email ID: " <<
            setw(12) << "Mobile: " <<
            setw(5) << "Gender: " <<
            setw(30) << "Guardian Name: " <<
            setw(12) << "Guardian Contact: " <<
            endl;
        cout << "____________________________________________________________________________________________________________________________________________________"
            << endl;
        while (res->next()) {
            cout << setw(15) << res->getInt("enrollmentno")
                << setw(30) << res->getString("name")
                << setw(25) << res->getString("emailid")
                << setw(12) << res->getInt("mobile")
                << setw(5) << res->getString("gender")
                << setw(30) << res->getString("guardian_name")
                << setw(12) << res->getString("guardian_contact")
                << endl;

        }
    view_my_class:
        cout << "1. Refresh" << endl
            << "2. Back to Faculty Dashboard" << endl <<
            "Enter a option: ";
        cin >> choice;
        if (choice == 1) {
            clearscreen();
            viewMyclass();
        }
        else if (choice == 2) {
            clearscreen();
            Facultymenu();
        }
        else {
            cout << "Invalid option, please try again." << endl;
            goto view_my_class;
        }

    }

    void viewExams() {
        cout << "1. FY " << endl
            << "2. SY";
    }

};

class hod : public common_variables, public Database {
private:
    string fname, department = "IF", course;
public:
    string  exam_date, exam_time, exam_duration, course_name, year, dept;
    int max_marks, passing_marks, semester, course_code, faculty;
    void hodmenu() {
        clearscreen();
        cout << "Welcome " << fname << endl << endl;
        cout << "Head of Department Dashboard" << endl;
        cout << "1. View students" << endl <<
            "2. View faculty" << endl <<
            "3. Add course" << endl <<
            "4. View courses" << endl <<
            "5. Delete course" << endl <<
            "6. Add Exam" << endl;
        cout << "7. View exam details" << endl <<
            "8. Logout" << endl <<
            "Enter a option:";
        cin >> choice;
        if (choice == 1) {
            viewStudents();
        }
        else if (choice == 2) {
            viewFaculty();
        }
        else if (choice == 4) {
            viewCourses();
        }
        else if (choice == 3) {
            addCourse();
        }
        else if (choice == 5) {
            //deleteCourse();
        }
        else if (choice == 6) {
            addExam();

        }
        else if (choice == 7) {
            viewExam();
        }
        else if (choice == 8) {
            clearscreen();
            mainscreen();
        }
    }

    void addExam() {
        clearscreen();
        cout << "Add Exam" << endl << endl;
        try {
            cout << left << setw(14) << "Course Code" << setw(14) << "Course Name" << setw(7) << "Year" << setw(12) << "Semester" << setw(30) << "Faculty Name" << endl;
            cout << "----------------------------------------------------------------------------------------------------------" << endl;
            sql::Connection* con = getConnection();
            sql::PreparedStatement* pstmt = con->prepareStatement("SELECT c.course_code, c.course_name, c.year, c.semester, concat (f.fname, ' ' , f.mname, ' ', f.lname) as faculty_name FROM courses c JOIN faculty_info f ON c.faculty_id = f.faculty_id WHERE f.department = ?");
            pstmt->setString(1, department);
            sql::ResultSet* res = pstmt->executeQuery();
            while (res->next()) {
                cout << left << setw(14) << res->getInt("course_code") << setw(14) << res->getString("course_name") << setw(7) << res->getInt("year") << setw(12) << res->getInt("semester") << setw(30) << res->getString("faculty_name")  << endl;
            }
            delete con;
        }
        catch (sql::SQLException& e) {
            cout << "Error: " << e.what() << endl;
        }

        cout << "Enter course code: ";
        cin >> course_code;
        cout << "Enter Year(FY/SY/TY):";
        cin >> year;
        cout << "Enter exam date(DD/MM/YY): ";
        cin >> exam_date;
        cout << "Enter exam time(HH:MM): ";
        cin >> exam_time;
        cout << "Enter exam duration(HH:MM): ";
        cin >> exam_duration;
        cout << "Enter maximum marks(Total): ";
        cin >> max_marks;
        cout << "Enter passing marks(Minimum): ";
        cin >> passing_marks;
        try {
            sql::Connection* con = getConnection();
            sql::PreparedStatement* pstmt = con->prepareStatement("INSERT INTO exams (course_code, exam_date, exam_time, exam_duration, max_marks, passing_marks) VALUES (?,?,?,?,?,?)");
            pstmt->setInt(1, course_code);
            pstmt->setString(2, exam_date);
            pstmt->setString(3, exam_time);
            pstmt->setString(4, exam_duration);
            pstmt->setInt(5, max_marks);
            pstmt->setInt(6, passing_marks);
            pstmt->execute();
            cout << "Exam added successfully." << endl;
            delete pstmt;
            delete con;

            cout << endl;
        hod_add_exam:
            cout << "1. Back to HOD Dashborad" << endl
                << "Enter a choice: ";
            cin >> choice;
            if (choice == 1) {
                clearscreen();
                hodmenu();
            }
            else {
                cout << "Invalid choice !! " << endl;
                goto hod_add_exam;

            }
        }
        catch (sql::SQLException& e) {
            cout << "Error : " << e.what() << endl;
        }

    }

    bool loginhod(string& emailid, string& password) {
        int choice;
        try {
            pstmt = con->prepareStatement("SELECT password, department, fname FROM faculty_info WHERE emailid = ?");
            pstmt->setString(1, emailid);
            res = pstmt->executeQuery();

            if (res->next()) {
                string storedPassword = res->getString("password");
                department = res->getString("department");
                fname = res->getString("fname");

                if (storedPassword == password) {
                    // If the password is correct, go to the Faculty menu
                    hodmenu();
                    delete res;
                    delete pstmt;
                    delete con;
                    return true; // Login success
                }
                else {
                    // If the password is incorrect
                    cout << "Incorrect password." << endl << endl;
                Faculty_exception1_re:
                    cout <<
                        "1. Go back to main menu" << endl <<
                        "Enter an option: ";
                    cin >> choice;

                    if (choice == 1) {
                        clearscreen();
                        mainscreen(); // Go back to main menu
                    }
                    else {
                        cout << "Invalid option, please try again." << endl;
                        goto Faculty_exception1_re;
                    }
                }
            }
            else {
            Faculty_exception2_re:
                // If the email is not found
                cout << "Email ID not found.\n";
                cout << "1. Go back to main menu" << endl <<
                    "Enter a option:";

                cin >> choice;
                if (choice == 1) {
                    clearscreen();
                    mainscreen();

                }
                else {

                    cout << "Invalid option, please try again." << endl;
                    goto Faculty_exception2_re;
                }
            }

            // Clean up resources after each query attempt
            delete res;
            delete pstmt;
        }
        catch (sql::SQLException& e) {
            std::cerr << "Error during login: " << e.what() << std::endl;
        }


        delete con; // Ensure connection is closed when exiting the loop
        return false; // Login failed
    }

    void viewStudents() {
        cout << "View my students" << endl;
        try {
            pstmt = con->prepareStatement("SELECT enrollmentno, CONCAT(fname, ' ', mname, ' ', lname) AS Name, "
                "department, year, emailid, mobile, birthdate, address, gender, admission_date, "
                "guardian_name, guardian_contact, blood_group, nationality, category, aadhar_number "
                "FROM students_info WHERE department = ?");
            pstmt->setString(1, department);
            res = pstmt->executeQuery();

            const int widthEnrollment = 15; // enrollmentno (bigint)
            const int widthName = 25; // Name (concatenation of fname, mname, lname)
            const int widthDepartment = 6; // department (varchar(2)) reduced space between dept & name
            const int widthYear = 6; // year (varchar(2)) reduced gap between dept & year
            const int widthEmail = 27; // emailid (varchar(50)) reduced gap between email & mobile
            const int widthMobile = 12; // mobile (bigint)
            const int widthBirthdate = 12; // birthdate (varchar(10))
            const int widthAddress = 30; // address (varchar(100)), nearby 20 characters as you requested
            const int widthGender = 8; // gender (varchar(10))
            const int widthAdmissionDate = 18; // admission_date (date)
            const int widthGuardianName = 20; // guardian_name (varchar(50))
            const int widthGuardianContact = 19; // guardian_contact (bigint)
            const int widthBloodGroup = 10; // blood_group (varchar(3))
            const int widthNationality = 13; // nationality (varchar(30))
            const int widthCategory = 10; // category (varchar(20))
            const int widthAadharNumber = 15; // aadhar_number (bigint)

            // Display header
            cout << left <<
                setw(widthEnrollment) << "Enrollment No" <<
                setw(widthName) << "Name" <<
                setw(widthDepartment) << "Dept" <<
                setw(widthYear) << "Year" <<
                setw(widthEmail) << "Email ID" <<
                setw(widthMobile) << "Mobile" <<
                setw(widthBirthdate) << "Birthdate" <<
                setw(widthAddress) << "Address" <<
                setw(widthGender) << "Gender" <<
                setw(widthAdmissionDate) << "Admission Date" <<
                setw(widthGuardianName) << "Guardian Name" <<
                setw(widthGuardianContact) << "Guardian Contact" <<
                setw(widthBloodGroup) << "Blood G." <<
                setw(widthNationality) << "Nationality" <<
                setw(widthCategory) << "Category" <<
                setw(widthAadharNumber) << "Aadhar No " <<
                endl;

            // Display separator line
            cout << string(widthEnrollment, '-') <<
                string(widthName, '-') <<
                string(widthDepartment, '-') <<
                string(widthYear, '-') <<
                string(widthEmail, '-') <<
                string(widthMobile, '-') <<
                string(widthBirthdate, '-') <<
                string(widthAddress, '-') <<
                string(widthGender, '-') <<
                string(widthAdmissionDate, '-') <<
                string(widthGuardianName, '-') <<
                string(widthGuardianContact, '-') <<
                string(widthBloodGroup, '-') <<
                string(widthNationality, '-') <<
                string(widthCategory, '-') <<
                string(widthAadharNumber, '-') <<
                endl;

            // Fetch and display each row of data
            while (res->next()) {
                cout << left <<
                    setw(widthEnrollment) << res->getString("enrollmentno") <<
                    setw(widthName) << res->getString("Name") <<
                    setw(widthDepartment) << res->getString("department") <<
                    setw(widthYear) << res->getString("year") <<
                    setw(widthEmail) << res->getString("emailid") <<
                    setw(widthMobile) << res->getString("mobile") <<
                    setw(widthBirthdate) << res->getString("birthdate") <<
                    setw(widthAddress) << res->getString("address") <<
                    setw(widthGender) << res->getString("gender") <<
                    setw(widthAdmissionDate) << res->getString("admission_date") <<
                    setw(widthGuardianName) << res->getString("guardian_name") <<
                    setw(widthGuardianContact) << res->getString("guardian_contact") <<
                    setw(widthBloodGroup) << res->getString("blood_group") <<
                    setw(widthNationality) << res->getString("nationality") <<
                    setw(widthCategory) << res->getString("category") <<
                    setw(widthAadharNumber) << res->getString("aadhar_number") <<
                    endl;
            }
            cout << endl << endl;
            cout << "1. Back to HOD dashboard" << endl;
            cout << "2. Refresh" << endl;
            cout << "Enter an option:" << endl;
            cin >> choice;
            cout << endl << endl;
            clearscreen();

            if (choice == 1) {
                hodmenu();
            }
            else if (choice == 2) {
                viewFaculty();
            }
            else {
                cout << "Invalid choice. Please try again." << endl;
                viewFaculty();
            }



        }
        catch (sql::SQLException& e) {
            cout << "Error : " << e.what() << endl;
        }
    }

    void viewFaculty() {
        cout << "Faculty List" << endl;
        try {
            unique_ptr < sql::Statement > stmt(con->createStatement());
            string query = "SELECT faculty_id, CONCAT(fname, ' ', mname, ' ', lname) AS Name, "
                "department, emailid, mobile, birthdate, address, joining_date, gender, experience_years,post "
                "FROM faculty_info WHERE post = 'Faculty'";

            // Executing the query
            unique_ptr < sql::ResultSet > res(stmt->executeQuery(query));

            // Define column widths
            const int widthFacultyID = 12; // Facultyid (int)
            const int widthName = 25; // Name (concatenation of fname, mname, lname)
            const int widthDepartment = 7; // department (varchar(2))
            const int widthEmail = 25; // emailid (varchar(50))
            const int widthMobile = 15; // mobile (bigint)
            const int widthBirthdate = 15; // birthdate (varchar(10))
            const int widthAddress = 40; // address (varchar(200))
            const int widthJoiningDate = 12; // joining_date (date)
            const int widthGender = 10; // gender (varchar(10))
            const int widthExperience = 20; // experience_years (int)
            const int widthPost = 10;

            // Display header without adding extra lines
            cout << left <<
                setw(widthFacultyID) << "Faculty ID" <<
                setw(widthName) << "Name" <<
                setw(widthDepartment) << "Dept" <<
                setw(widthEmail) << "Email ID" <<
                setw(widthMobile) << "Mobile" <<
                setw(widthBirthdate) << "Birthdate" <<
                setw(widthJoiningDate) << "Joining Date" <<
                setw(widthGender) << "Gender" <<
                setw(widthExperience) << "Experience (Years)" <<
                setw(widthPost) << "Post" <<
                endl;

            // Display separator line
            cout << string(widthFacultyID, '-') <<
                string(widthName, '-') <<
                string(widthDepartment, '-') <<
                string(widthEmail, '-') <<
                string(widthMobile, '-') <<
                string(widthBirthdate, '-') <<
                string(widthJoiningDate, '-') <<
                string(widthGender, '-') <<
                string(widthExperience, '-') <<
                string(widthPost, '-') <<
                endl;

            // Fetch and display each row of data without adding extra lines
            while (res->next()) {
                cout << left <<
                    setw(widthFacultyID) << res->getInt("faculty_id") <<
                    setw(widthName) << res->getString("Name") <<
                    setw(widthDepartment) << res->getString("department") <<
                    setw(widthEmail) << res->getString("emailid") <<
                    setw(widthMobile) << res->getInt("mobile") <<
                    setw(widthBirthdate) << res->getString("birthdate") <<
                    setw(widthJoiningDate) << res->getString("joining_date") <<
                    setw(widthGender) << res->getString("gender") <<
                    setw(widthExperience) << res->getInt("experience_years") <<
                    setw(widthPost) << res->getString("post") <<
                    endl;
            }

            // Prompt for next action
            int choice;
            cout << endl << "1. Back to HOD dashboard" << endl;
            cout << "2. Refresh" << endl;
            cout << "Enter your option: ";
            cin >> choice;
            cout << endl;

            clearscreen(); // Assuming clearscreen() is defined elsewhere
            if (choice == 1) {
                hodmenu(); // Assuming adminmenu() is defined elsewhere
            }
            else if (choice == 2) {
                viewFaculty(); // Refresh the list
            }
            else {
                cout << "Invalid choice. Please try again." << endl;
                viewFaculty(); // Retry showing the list
            }
        }
        catch (sql::SQLException& e) {
            cerr << "SQLException: " << e.what() << endl;
            cerr << "MySQL error code: " << e.getErrorCode() << endl;
            cerr << "SQLState: " << e.getSQLState() << endl;
        }

    }

    void addCourse() {
        clearscreen();
        cout << "Enter Course Details Below " << endl << endl << endl
            << "Enter Course code : ";
        cin >> course_code;
        cin.ignore();
        cout << "Enter Course name : ";
        getline(cin, course_name);
        cout << setw(5) << "Faculty ID : "
            << setw(30) << "Faculty Name" << endl;
        cout << "--------------------------------------------------------------------------------------------------------------------------------" << endl;
        try {
            pstmt = con->prepareStatement("SELECT CONCAT(fname,' ', mname,' ', lname) AS name,faculty_id FROM faculty_info WHERE department = ? ORDER BY faculty_id");
            pstmt->setString(1, department);
            res = pstmt->executeQuery();
            while (res->next()) {
                cout << setw(5) << res->getInt("faculty_id")
                    << setw(30) << res->getString("name") << endl;
            }

        }
        catch (sql::SQLException& e) {
            cerr << "SQL Error: " << e.what() << endl;
        }
        cout << endl << "Enter Faculty ID :";
        cin >> faculty;
        cout << "Enter year (FY,SY,TY) :";
        cin >> year;
        cout << "Enter semester (1,2,3,4,5,6) :";
        cin >> semester;
        try {
            pstmt = con->prepareStatement("INSERT INTO courses (course_code, course_name, department, semester, year, faculty_id) VALUES (? ,?, ?, ?, ?, ?)");
            pstmt->setInt(1, course_code);
            pstmt->setString(2, course_name);
            pstmt->setString(3, department);
            pstmt->setInt(4, semester);
            pstmt->setString(5, year);
            pstmt->setInt(6, faculty);
            pstmt->executeQuery();

            cout << "Course added successfully!" << endl << endl;
        hod_add_course_re:
            cout << "1. Add another course" << endl
                << "2. Back to HOD dashboard" << endl;
            cout << "Enter your option: ";
            cin >> choice;
            if (choice == 1) {
                addCourse();
            }
            else if (choice == 2) {
                hodmenu();
            }
            else {
                cout << "Invalid choice. Please try again." << endl;
                goto hod_add_course_re;
            }

        }
        catch (sql::SQLException& e) {
            cerr << "SQL Error: " << e.what() << endl;
        }

    }

    void viewCourses() {
        clearscreen();
        cout << left << setw(13) << "Course Code" << setw(40) << "Course Name" << setw(13) << "Department" << setw(11) << "Semester" << setw(7) << "Year" << setw(30) << "Faculty Name" << endl;
        cout << "--------------------------------------------------------------------------------------------------------------------------------" << endl;
        try {
            pstmt = con->prepareStatement("SELECT c.course_code,c.course_name,c.department,c.semester,c.year,concat(f.fname, ' ' , f.mname , ' ' , f.lname) as faculty_name FROM courses AS c JOIN  faculty_info AS f ON c.faculty_id = f.faculty_id;");
            res = pstmt->executeQuery();
            while (res->next()) {
                cout << left << setw(13) << res->getInt("course_code") << setw(40) << res->getString("course_name") << setw(13) << res->getString("department") << setw(11) << res->getInt("semester") << setw(7) << res->getString("year") << setw(30) << res->getString("faculty_name") << endl;
            }
            hod_view_course_re:
			cout << "1. Back to HOD Dashboard" << endl;
            cout << "2.Refresh " << endl;
            cout << "Enter a option : ";
			cin >> choice;
            if (choice == 1) {
				clearscreen();
            }
            else if (choice == 2) {
				clearscreen();
                viewCourses();
            }
            else {
				cout << "Invalid choice" << endl;
                goto hod_view_course_re;
            }
        }
        catch (sql::SQLException& e) {
            cerr << "SQL Error: " << e.what() << endl;
        }
    }

    void viewExam() {
        cout << "Previously conducted exams" << endl << endl;
        cout << setw(10) << "Exam ID" << setw(15) << "Course Code" << setw(40) << "Course Name" << setw(11) << "Semester" << setw(10) << "Year" << setw(30) << "Faculty Name" << endl;
        cout << "--------------------------------------------------------------------------------------------------------------------------------" << endl;
        try {
            pstmt = con->prepareStatement("SELECT e.exam_id,c.course_code,c.course_name,c.semester,c.year,concat(f.fname, ' ' , f.mname , ' ' , f.lname) as faculty_name FROM exams AS e JOIN  courses AS c ON e.course_code = c.course_code JOIN  faculty_info AS f ON c.faculty_id = f.faculty_id;");
            res = pstmt->executeQuery();
            while (res->next()) {
                cout << setw(10) << res->getInt("exam_id") << setw(15) << res->getInt("course_code") << setw(40) << res->getString("course_name") << setw(11) << res->getInt("semester") << setw(10) << res->getString("year") << setw(30) << res->getString("faculty_name") << endl;
            }
        hod_view_exam_re:
            cout << "1. Back to HOD dashboard" << endl;
            cout << "2. Refresh" << endl;
            cout << "Enter your option: ";
            cin >> choice;
            if (choice == 1) {
                hodmenu();
            }
            else if (choice == 2) {
                viewExam();
            }
            else {
                cout << "Invalid choice. Please try again." << endl;
                goto hod_view_exam_re;
            }
        }
        catch (sql::SQLException& e) {
            cerr << "SQL Error: " << e.what() << endl;
        }

    }

};

void mainscreen() {
    admin a;
    Faculty t;
    hod h;
    common_variables
        var;
    string username, password;
    h.hodmenu();
mainre:
    cout << "Welcome to Government Polytechnic Awasari" << endl << endl <<
        "1. Student login" << endl <<
        "2. Faculty login" << endl <<
        "3. HOD login" << endl <<
        "4. Admin login" << endl <<
        "5. Exit" << endl <<
        "Enter your choice: ";
    cin >>
        var.choice;
    cout << endl;
    clearscreen();
    if (var.choice == 1) {
        cout << "Service under development!" << endl;
        mainscreen();
    }
    else if (var.choice == 2) {
        string emailid, pass;
        cout << "Enter your email id : ";
        cin >> emailid;
        cout << "Enter password :";
        cin >> pass;
        t.loginFaculty(emailid, pass);
    }
    else if (var.choice == 4) {
    adminreenter: cout << "   Admin login   " << endl << endl;
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
        string emailid, pass;
        cout << "Enter your email id : ";
        cin >> emailid;
        cout << "Enter password :";
        cin >> pass;
        h.loginhod(emailid, pass);
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