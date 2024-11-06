#include <iostream> //for  input output

#include <string> //  for  string

#include <cstdlib> // for  exit

#include <iomanip> // For output Formatting

#include <string> // for string related  functions

#include <mysql_driver.h> // for mysql driver object

#include <mysql_connection.h> // for mysql connection object

#include <cppconn/statement.h> // for mysql command passing parameters

#include <cppconn/prepared_statement.h> // for prepared statement

#include <cppconn/resultset.h> // for retreving the resul set from database

#include <cppconn/exception.h> // for hadnling error coming through database

#include <fstream> // for file handling

using namespace std;
void mainscreen(); // Declaration of mainscreen function

sql::mysql::MySQL_Driver* driver; // Global variables to establish connection between Database and program
sql::Connection* con;
sql::PreparedStatement* pstmt;
sql::ResultSet* res;

class Database
{ // Database class for managing connection

public:
    sql::Connection* getConnection()
    {
        sql::mysql::MySQL_Driver* driver = sql::mysql::get_mysql_driver_instance();
        sql::Connection* con = driver->connect("tcp://127.0.0.1:3306", "root", "12345");
        con->setSchema("GPA_db");
        return con;
    }
    static void closeConnection()
    {
        delete con;
    }
};

class common_variables
{ // Class used for variables which are used through out the program
public:
    int choice;
};
void clearscreen()
{ // function for clearinng screen by using sysytem("cls") from <cstlib> file
    system("cls");
}

class admin : public common_variables
{ // This class handles all the functions for the admin and holds variables for the admin
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
    void adminmenu()
    { // Displays the admin menu
        cout << "Welcome to Admin Dashboard" << endl
            << endl;
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

        if (choice == 1)
        {
            registerStudent(); // else_if ladder as subtitute of switch case statement
        }
        else if (choice == 2)
        {
            showStudentList();
        }
        else if (choice == 3)
        {
            deleteStudent();
        }
        else if (choice == 4)
        {
            registerFaculty();
        }
        else if (choice == 5)
        {
            showFacultyList();
        }
        else if (choice == 6)
        {
            deleteFaculty();
        }
        else if (choice == 7)
        {
            cout << "Logged out" << endl;
            mainscreen();
        }
        else
        {
            cout << "Enter a Valid option !"; // handle a exception if user entered a wrong option
            goto admin_choice_re;
        }
    }

    bool registerStudent()
    { // Function to register a student
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
        do
        { // using do while loop for double confirmation of password
            cout << "Enter password: ";
            cin >> password;
            cout << "Confirm password: ";
            cin >> confirmPassword;

            if (password != confirmPassword)
            {
                cout << "Passwords do not match! Please try again." << endl;
            }
        } while (password != confirmPassword);

        try
        {

            unique_ptr<sql::Statement> stmt(con->createStatement()); // createing statement object for query

            // SQL query to create a table

            string insertQuery = "INSERT INTO students_info (enrollmentno, fname, mname, lname, department, year, emailid, mobile, birthdate, address, password, gender, admission_date, guardian_name,guardian_contact, blood_group, nationality, category, aadhar_number) VALUES (" +
                to_string(enrollmentno) + ",'" + fname + "','" + mname + "','" + lname + "','" + department + "','" + year + "','" + emailid + "'," +
                to_string(mobile) + ",'" + birthdate + "','" + address + "','" + password + "', '" + gender + "', '" + addmission_date + "', '" + guardian_name + "', " + to_string(guardian_mobile) + ", '" + blood_grp + "', '" + nationality + "', '" + category + "', '" + aadhar_number + "')";
            stmt->execute(insertQuery); // by using execute function query is sent to Database engine

            cout << endl
                << "Registration successful!" << endl
                << endl;
        register_re:
            cout << "1. Add another student" << endl
                << "2. Back to admin menu" << endl
                << "Enter a option:";
            cin >> choice;
            if (choice == 1)
            {
                clearscreen();
                registerStudent();
            }
            else if (choice == 2)
            {
                clearscreen();
                adminmenu();
            }
            else
            {
                cout << "Enter a valid option !";
                goto register_re;
            }
            clearscreen();
            adminmenu();
        }
        catch (sql::SQLException& e)
        { // this block prints the error message given by database engine
            cerr << "SQL Error: " << e.what() << endl;
            registerStudent(); // calls registerStudent() again to register the student
        }
        return true;
    }

    void showStudentList()
    {
        try
        {
            unique_ptr<sql::Statement> stmt(con->createStatement());

            // SQL query to retrieve student data with additional fields
            string query = "SELECT enrollmentno, CONCAT(fname, ' ', mname, ' ', lname) AS Name, "
                "department, year, emailid, mobile, birthdate, address, gender, admission_date, "
                "guardian_name, guardian_contact, blood_group, nationality, category, aadhar_number "
                "FROM students_info";

            // Executing the query
            unique_ptr<sql::ResultSet> res(stmt->executeQuery(query));

            // Column width for each column (Usinf constant in CPP)
            const int widthEnrollment = 15;      // enrollmentno (bigint)
            const int widthName = 25;            // Name (concatenation of fname, mname, lname)
            const int widthDepartment = 6;       // department (varchar(2)) reduced space between dept & name
            const int widthYear = 6;             // year (varchar(2)) reduced gap between dept & year
            const int widthEmail = 27;           // emailid (varchar(50)) reduced gap between email & mobile
            const int widthMobile = 12;          // mobile (bigint)
            const int widthBirthdate = 12;       // birthdate (varchar(10))
            const int widthAddress = 30;         // address (varchar(100)), nearby 20 characters as you requested
            const int widthGender = 8;           // gender (varchar(10))
            const int widthAdmissionDate = 18;   // admission_date (date)
            const int widthGuardianName = 20;    // guardian_name (varchar(50))
            const int widthGuardianContact = 19; // guardian_contact (bigint)
            const int widthBloodGroup = 10;      // blood_group (varchar(3))
            const int widthNationality = 13;     // nationality (varchar(30))
            const int widthCategory = 10;        // category (varchar(20))
            const int widthAadharNumber = 15;    // aadhar_number (bigint)

            cout << left << // Displaing header of the table
                setw(widthEnrollment) << "Enrollment No" << setw(widthName) << "Name" << setw(widthDepartment) << "Dept" << setw(widthYear) << "Year" << setw(widthEmail) << "Email ID" << setw(widthMobile) << "Mobile" << setw(widthBirthdate) << "Birthdate" << setw(widthAddress) << "Address" << setw(widthGender) << "Gender" << setw(widthAdmissionDate) << "Admission Date" << setw(widthGuardianName) << "Guardian Name" << setw(widthGuardianContact) << "Guardian Contact" << setw(widthBloodGroup) << "Blood G." << setw(widthNationality) << "Nationality" << setw(widthCategory) << "Category" << setw(widthAadharNumber) << "Aadhar No " << endl;

            // Display separator line
            cout << string(widthEnrollment, '-') << string(widthName, '-') << string(widthDepartment, '-') << string(widthYear, '-') << string(widthEmail, '-') << string(widthMobile, '-') << string(widthBirthdate, '-') << string(widthAddress, '-') << string(widthGender, '-') << string(widthAdmissionDate, '-') << string(widthGuardianName, '-') << string(widthGuardianContact, '-') << string(widthBloodGroup, '-') << string(widthNationality, '-') << string(widthCategory, '-') << string(widthAadharNumber, '-') << endl;

            // Fetch and display each row from Database
            while (res->next())
            {
                cout << left << setw(widthEnrollment) << res->getString("enrollmentno") << setw(widthName) << res->getString("Name") << setw(widthDepartment) << res->getString("department") << setw(widthYear) << res->getString("year") << setw(widthEmail) << res->getString("emailid") << setw(widthMobile) << res->getString("mobile") << setw(widthBirthdate) << res->getString("birthdate") << setw(widthAddress) << res->getString("address") << setw(widthGender) << res->getString("gender") << setw(widthAdmissionDate) << res->getString("admission_date") << setw(widthGuardianName) << res->getString("guardian_name") << setw(widthGuardianContact) << res->getString("guardian_contact") << setw(widthBloodGroup) << res->getString("blood_group") << setw(widthNationality) << res->getString("nationality") << setw(widthCategory) << res->getString("category") << setw(widthAadharNumber) << res->getString("aadhar_number") << endl;
            }

            int choice;
            cout << endl
                << endl;
            cout << "1. Back to admin menu" << endl;
            cout << "2. Refresh" << endl;
            cout << "Enter an option:" << endl;
            cin >> choice;
            cout << endl
                << endl;
            clearscreen();

            if (choice == 1)
            {
                adminmenu();
            }
            else if (choice == 2)
            {
                showStudentList();
            }
            else
            {
                cout << "Invalid choice. Please try again." << endl;
                showStudentList();
            }
        }
        catch (sql::SQLException& e)
        {
            cerr << "SQLException: " << e.what() << endl;
            cerr << "MySQL error code: " << e.getErrorCode() << endl;
            cerr << "SQLState: " << e.getSQLState() << endl;
        }
    }

    void deleteStudent()
    { // Function to delete a student record
        unique_ptr<sql::Statement> stmt(con->createStatement());
        cout << "Enter enrollment number of the student to delete: ";
        cin >> enrollmentno;

        try
        {

            string deleteQuery = "DELETE FROM students_info WHERE enrollmentno = " + to_string(enrollmentno);
            stmt->execute(deleteQuery);

            cout << "Student record deleted successfully!" << endl;
        }
        catch (sql::SQLException& e)
        {
            cerr << "SQL Error: " << e.what() << endl;
        }
        int choice;
        cout << endl
            << endl;
        cout << "1. Delete another student" << endl;
        cout << "2. Back to admin menu" << endl;
        cout << "Enter a option:" << endl;
        cin >> choice;
        cout << endl
            << endl;
        clearscreen();
        if (choice == 1)
        {
            deleteStudent();
        }
        if (choice == 2)
        {
            adminmenu();
        }
    }

    void registerFaculty()
    { // Function to register a new faculty
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
        do
        { // using do while loop for double confirmation of password
            cout << "Enter password: ";
            cin >> password;
            cout << "Confirm password: ";
            cin >> confirmPassword;

            if (password != confirmPassword)
            {
                cout << "Passwords do not match! Please try again." << endl;
            }
        } while (password != confirmPassword);

        try
        {
            unique_ptr<sql::Statement> stmt(con->createStatement());
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
                "PRIMARY KEY(faculty_id)"
                ")";
            stmt->execute(createQuery);

            string insertQuery = "INSERT INTO faculty_info (faculty_id, fname, mname, lname, department, emailid, mobile, birthdate, address, joining_date, gender, experience_years,password,post) VALUES (" +
                to_string(faculty_id) + ",'" + fname + "','" + mname + "','" + lname + "','" + department + "','" + emailid + "'," +
                to_string(mobile) + ",'" + birthdate + "','" + address + "', '" + joining_date + "','" + gender + "'," + to_string(experience_years) + ",'" + password + "','" + post + "')";
            stmt->execute(insertQuery);

            cout << endl
                << endl
                << "Faculty registered successfully!" << endl;
        }
        catch (sql::SQLException& e)
        {
            cerr << "SQL Error: " << e.what() << endl;
        }

        cout << endl
            << endl
            << "1. Add another Faculty" << endl
            << "2. Back to admin menu" << endl
            << "Enter a option:";
        cin >> choice;
        clearscreen();

        if (choice == 1)
        {
            clearscreen();
            registerFaculty();
        }

        if (choice == 2)
        {
            clearscreen();
            adminmenu();
        }
    }

    void showFacultyList()
    { // Function to display the list of all faculty
        try
        {
            unique_ptr<sql::Statement> stmt(con->createStatement());
            string query = "SELECT faculty_id, CONCAT(fname, ' ', mname, ' ', lname) AS Name, "
                "department, emailid, mobile, birthdate, address, joining_date, gender, experience_years,post "
                "FROM faculty_info";
            unique_ptr<sql::ResultSet> res(stmt->executeQuery(query));
            const int widthFacultyID = 12;   // Facultyid (int)
            const int widthName = 25;        // Name (concatenation of fname, mname, lname)
            const int widthDepartment = 7;   // department (varchar(2))
            const int widthEmail = 25;       // emailid (varchar(50))
            const int widthMobile = 15;      // mobile (bigint)
            const int widthBirthdate = 15;   // birthdate (varchar(10))
            const int widthAddress = 40;     // address (varchar(200))
            const int widthJoiningDate = 12; // joining_date (date)
            const int widthGender = 10;      // gender (varchar(10))
            const int widthExperience = 20;  // experience_years (int)
            const int widthPost = 10;

            cout << left << setw(widthFacultyID) << "Faculty ID" << setw(widthName) << "Name" << setw(widthDepartment) << "Dept" << setw(widthEmail) << "Email ID" << setw(widthMobile) << "Mobile" << setw(widthBirthdate) << "Birthdate" << setw(widthJoiningDate) << "Joining Date" << setw(widthGender) << "Gender" << setw(widthExperience) << "Experience (Years)" << setw(widthPost) << "Post" << endl;

            cout << string(widthFacultyID, '-') << string(widthName, '-') << string(widthDepartment, '-') << string(widthEmail, '-') << string(widthMobile, '-') << string(widthBirthdate, '-') << string(widthJoiningDate, '-') << string(widthGender, '-') << string(widthExperience, '-') << string(widthPost, '-') << endl;

            while (res->next())
            {
                cout << left << setw(widthFacultyID) << res->getInt("faculty_id") << setw(widthName) << res->getString("Name") << setw(widthDepartment) << res->getString("department") << setw(widthEmail) << res->getString("emailid") << setw(widthMobile) << res->getInt("mobile") << setw(widthBirthdate) << res->getString("birthdate") << setw(widthJoiningDate) << res->getString("joining_date") << setw(widthGender) << res->getString("gender") << setw(widthExperience) << res->getInt("experience_years") << setw(widthPost) << res->getString("post") << endl;
            }
            int choice;
            cout << endl
                << "1. Back to admin menu" << endl;
            cout << "2. Refresh" << endl;
            cout << "Enter your option: ";
            cin >> choice;
            cout << endl;

            clearscreen();
            if (choice == 1)
            {
                adminmenu();
            }
            else if (choice == 2)
            {
                showFacultyList();
            }
            else
            {
                cout << "Invalid choice. Please try again." << endl;
                showFacultyList();
            }
        }
        catch (sql::SQLException& e)
        {
            cerr << "SQLException: " << e.what() << endl;
            cerr << "MySQL error code: " << e.getErrorCode() << endl;
            cerr << "SQLState: " << e.getSQLState() << endl;
        }
    }

    void deleteFaculty()
    {
        cout << "Enter Faculty ID of the Faculty to delete: ";
        cin >> faculty_id;

        try
        {
            unique_ptr<sql::Statement> stmt(con->createStatement());
            string deleteQuery = "DELETE FROM faculty_info WHERE faculty_id = " + to_string(faculty_id);
            stmt->execute(deleteQuery);

            cout << "Faculty record deleted successfully!" << endl;
        }
        catch (sql::SQLException& e)
        {
            cerr << "SQL Error: " << e.what() << endl;
        }
        cout << endl
            << endl
            << "1. Delete another Faculty" << endl
            << "2. Back to admin menu" << endl
            << "Enter a option:";
        cin >> choice;
        clearscreen();

        if (choice == 1)
        {
            deleteFaculty();
        }

        if (choice == 2)
        {
            adminmenu();
        }
    }
};

class Faculty : public common_variables, public Database
{
private:
    string emailid,
        password,
        department,
        fname,
        lname;
    int faculty_id;
    admin a;

public:
    Faculty()
    {
        try
        {
            // Initialize the MySQL driver and connection
            driver = sql::mysql::get_mysql_driver_instance();
            con = driver->connect("tcp://127.0.0.1:3306", "root", "12345");
            con->setSchema("GPA_db");
        }
        catch (sql::SQLException& e)
        {
            cerr << "SQL Error: " << e.what() << endl;
            exit(EXIT_FAILURE);
        }
    }
    void Facultymenu()
    { // Function to display the faculty menu
        clearscreen();
        cout << "Welcome " << fname << "!" << endl
            << endl
            << endl;
        cout << "Faculty Dashboard" << endl
            << endl;
        cout << "1. View my class" << endl
            << "2. Exams" << endl
            << "3. Logout" << endl
            << endl;
        "Enter a option:";
        cin >> choice;

        if (choice == 1)
        {
            viewMyclass();
        }
        else if (choice == 2)
        {
            clearscreen();
            cout << "Exam Section" << endl
                << endl
                << endl;
            cout << "1. View Exams" << endl;
            cout << "2. Enter Marks" << endl;
            cout << "3. View Marks" << endl;
            cout << "4. View Stats" << endl;
            cout << "5. Generate Reoport" << endl;
            cout << "Enter choice: ";
            cin >> choice;
            if (choice == 1)
            {
                viewExams();
            }
            else if (choice == 2)
            {
                enterMarks();
            }
            else if (choice == 3)
            {
                viewMarks();
            }
            else if (choice == 4)
            {
                viewStats();
            }
            else if (choice == 5)
            {
                generateCSVReport();
            }
            else
            {
                cout << "Invalid choice. Please try again." << endl;
                Facultymenu();
            }
        }
        else if (choice == 9)
        {
            clearscreen();
            mainscreen();
        }
    }
    bool loginFaculty(string& emailid, string& password)
    { // Function to handle faculty login
        int choice;
        try
        {
            pstmt = con->prepareStatement("SELECT password, department, fname, lname, faculty_id FROM faculty_info WHERE emailid = ?");
            pstmt->setString(1, emailid);

            res = pstmt->executeQuery();

            if (res->next())
            {
                string storedPassword = res->getString("password");
                department = res->getString("department");
                fname = res->getString("fname");
                lname = res->getString("lname");
                faculty_id = res->getInt("faculty_id");

                if (storedPassword == password)
                {
                    Facultymenu();
                    delete res;
                    delete pstmt;
                    delete con;
                    return true; // Login success
                }
                else
                {
                    // If the password is incorrect
                    cout << "Incorrect password." << endl
                        << endl;
                Faculty_exception1_re:
                    cout << "1. Go back to main menu" << endl
                        << "Enter an option: ";
                    cin >> choice;

                    if (choice == 1)
                    {
                        clearscreen();
                        mainscreen(); // Go back to main menu
                    }
                    else
                    {
                        cout << "Invalid option, please try again." << endl;
                        goto Faculty_exception1_re;
                    }
                }
            }
            else
            {
            Faculty_exception2_re:
                cout << "Email ID not found.\n";
                cout << "1. Go back to main menu" << endl
                    << "Enter a option:";

                cin >> choice;
                if (choice == 1)
                {
                    clearscreen();
                    mainscreen();
                }
                else
                {

                    cout << "Invalid option, please try again." << endl;
                    goto Faculty_exception2_re;
                }
            }
        }
        catch (sql::SQLException& e)
        {
            cerr << "Error during login: " << e.what() << endl;
        }

        delete con;
        return false;
    }

    void viewMyclass()
    { // Function to view the classes of the faculty
        long long int enrollmentno;
        string guardian_name;
        string guardian_contact;
        int mobile;
        string gender;
        string name;
        string email;
        cout << "1. FY" << endl
            << "2. SY" << endl
            << "3. TY" << endl
            << "Enter your choice: ";
        cin >> choice;

        if (choice == 1)
        {
            string year = "FY";
            viewMyclassQuery(year);
        }
        else if (choice == 2)
        {
            string year = "SY";
            viewMyclassQuery(year);
        }
        else if (choice == 3)
        {
            string year = "TY";
            viewMyclassQuery(year);
        }
    }
    void viewMyclassQuery(string year)
    { // called from viewMyclass(), written seprately to avoid code duplication
        sql::Connection* con = getConnection();
        sql::PreparedStatement* pstmt = con->prepareStatement("SELECT enrollmentno,concat(fname,' ',mname,' ',lname) AS name, emailid,mobile,gender,guardian_name,guardian_contact FROM students_info WHERE department = ? AND year = ?;");
        pstmt->setString(1, department);
        pstmt->setString(2, year);
        sql::ResultSet* res = pstmt->executeQuery();
        cout << left << setw(15) << "Enrollment No: " << setw(30) << "Name: " << setw(25) << "Email ID: " << setw(12) << "Mobile: " << setw(5) << "Gender: " << setw(30) << "Guardian Name: " << setw(12) << "Guardian Contact: " << endl;
        cout << "____________________________________________________________________________________________________________________________________________________"
            << endl;
        while (res->next())
        {
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
            << "2. Back to Faculty Dashboard" << endl
            << "Enter a option: ";
        cin >> choice;
        if (choice == 1)
        {
            clearscreen();
            viewMyclass();
        }
        else if (choice == 2)
        {
            clearscreen();
            Facultymenu();
        }
        else
        {
            cout << "Invalid option, please try again." << endl;
            goto view_my_class;
        }
    }

    void viewExams()
    {
        cout << "View Exams" << endl;
        try
        {
            sql::Connection* con = getConnection();
            sql::PreparedStatement* pstmt = con->prepareStatement(
                "SELECT e.exam_id, e.exam_type, e.exam_date, e.exam_time, e.exam_duration, "
                "e.max_marks, e.passing_marks, c.course_code, c.course_name, c.year, c.department "
                "FROM exams e "
                "JOIN courses c ON e.course_code = c.course_code "
                "WHERE c.faculty_id = ? AND c.department = ? "
                "ORDER BY c.year, e.exam_date;");

            pstmt->setInt(1, faculty_id);
            pstmt->setString(2, department);

            sql::ResultSet* res = pstmt->executeQuery();

            cout << left << setw(10) << "Exam ID" << setw(15) << "Exam Type" << setw(15) << "Date"
                << setw(10) << "Time" << setw(15) << "Duration" << setw(10) << "Max Marks"
                << setw(15) << "Passing Marks" << setw(14) << "Course Code" << setw(40) << "Course Name"
                << setw(7) << "Year" << endl;

            cout << "--------------------------------------------------------------------------------------------------------------------------------------------------------------------" << endl;

            while (res->next())
            {
                cout << left << setw(10) << res->getInt("exam_id")
                    << setw(15) << res->getString("exam_type")
                    << setw(15) << res->getString("exam_date")
                    << setw(10) << res->getString("exam_time")
                    << setw(15) << res->getString("exam_duration")
                    << setw(10) << res->getInt("max_marks")
                    << setw(15) << res->getInt("passing_marks")
                    << setw(14) << res->getInt("course_code")
                    << setw(40) << res->getString("course_name")
                    << setw(7) << res->getString("year") << endl;
            }

            delete res;
            delete pstmt;
            delete con;

            cout << "Press any key to continue..." << endl;
            cin.ignore();
            cin.get();
            Facultymenu();
        }
        catch (sql::SQLException& e)
        {
            cout << "Error: " << e.what() << endl;
        }
    }

    void enterMarks()
    {
        int selectedExamId;
        clearscreen();
        try
        {
            sql::Connection* con = getConnection();
            sql::PreparedStatement* pstmt = con->prepareStatement(
                "SELECT e.exam_id, e.exam_type, e.exam_date, c.course_code, c.course_name, c.year "
                "FROM exams e "
                "JOIN courses c ON e.course_code = c.course_code "
                "JOIN faculty_info f ON c.faculty_id = f.faculty_id "
                "WHERE f.faculty_id = ? AND c.department = ? "
                "ORDER BY c.year, e.exam_date;");

            pstmt->setInt(1, faculty_id);
            pstmt->setString(2, department);
            sql::ResultSet* res = pstmt->executeQuery();
            cout << setw(10) << "Exam ID" << setw(30) << "Exam Type" << setw(15) << "Date" << setw(15) << "Course Code" << setw(40) << "Course Name" << setw(7) << "Year" << endl;
            cout << "---------------------------------------------------------------------------------------------------------" << endl;

            while (res->next())
            {
                cout << setw(10) << res->getInt("exam_id") << setw(30) << res->getString("exam_type") << setw(15) << res->getString("exam_date") << setw(15) << res->getString("course_code") << setw(40) << res->getString("course_name") << setw(7) << res->getString("year") << endl;
            }

            cout << "Enter the exam ID to enter marks for: ";
            cin >> selectedExamId;
        }
        catch (sql::SQLException& e)
        {
            cout << "Error: " << e.what() << endl;
        }

        cout << "Enter Marks for All Students" << endl
            << endl;

        try
        {
            sql::Connection* con = getConnection();
            sql::PreparedStatement* pstmt = con->prepareStatement(
                "SELECT s.enrollmentno, CONCAT(s.fname, ' ', s.lname) AS full_name "
                "FROM students_info s "
                "JOIN courses c ON s.year = c.year "
                "JOIN exams e ON c.course_code = e.course_code "
                "WHERE e.exam_id = ?;");

            pstmt->setInt(1, selectedExamId);
            sql::ResultSet* res = pstmt->executeQuery();

            cout << left << setw(20) << "Enrollment No" << setw(30) << "Student Name" << setw(10) << "Marks" << endl;
            cout << "-------------------------------------------------------------------------------------------" << endl;

            if (!res->next())
            {
                cout << "No students found for the selected exam." << endl;
                return;
            }

            do
            {

                string enrollmentNo = res->getString("enrollmentno");
                string fullName = res->getString("full_name");

                cout << left << setw(20) << enrollmentNo << setw(30) << fullName;

                int marks;
                cout << "Enter marks: ";
                cin >> marks;

                sql::PreparedStatement* insertPstmt = con->prepareStatement(
                    "INSERT INTO marks (exam_id, enrollmentno, marks) VALUES (?, ?, ?) "
                    "ON DUPLICATE KEY UPDATE marks = VALUES(marks)");
                insertPstmt->setInt(1, selectedExamId);
                insertPstmt->setString(2, enrollmentNo);
                insertPstmt->setInt(3, marks);
                insertPstmt->execute();
                delete insertPstmt;
            } while (res->next());

            delete pstmt;
            delete con;

            cout << "All marks entered successfully." << endl;
        }
        catch (sql::SQLException& e)
        {
            cout << "Error: " << e.what() << endl;
        }
    }

    void viewMarks()
    {
        int selectedExamId;
        clearscreen();
        try
        {
            sql::Connection* con = getConnection();

            sql::PreparedStatement* pstmt = con->prepareStatement(
                "SELECT e.exam_id, e.exam_type, e.exam_date, c.course_code, c.course_name, c.year "
                "FROM exams e "
                "JOIN courses c ON e.course_code = c.course_code "
                "JOIN faculty_info f ON c.faculty_id = f.faculty_id "
                "WHERE f.faculty_id = ? AND c.department = ? "
                "ORDER BY c.year, e.exam_date;");
            pstmt->setInt(1, faculty_id);
            pstmt->setString(2, department);

            sql::ResultSet* res = pstmt->executeQuery();
            cout << setw(10) << "Exam ID" << setw(30) << "Exam Type" << setw(15) << "Date"
                << setw(15) << "Course Code" << setw(40) << "Course Name" << setw(7) << "Year" << endl;
            cout << "---------------------------------------------------------------------------------------------------------" << endl;

            while (res->next())
            {
                cout << setw(10) << res->getInt("exam_id") << setw(30) << res->getString("exam_type")
                    << setw(15) << res->getString("exam_date") << setw(15) << res->getString("course_code")
                    << setw(40) << res->getString("course_name") << setw(7) << res->getString("year") << endl;
            }

            cout << "Enter the exam ID to view marks: ";
            cin >> selectedExamId;

            sql::PreparedStatement* marksPstmt = con->prepareStatement(
                "SELECT s.enrollmentno, CONCAT(s.fname, ' ', s.lname) AS full_name, m.marks "
                "FROM marks m "
                "JOIN students_info s ON m.enrollmentno = s.enrollmentno "
                "WHERE m.exam_id = ?;");
            marksPstmt->setInt(1, selectedExamId);

            sql::ResultSet* marksRes = marksPstmt->executeQuery();
            cout << left << setw(20) << "Enrollment No" << setw(30) << "Student Name" << setw(10) << "Marks" << endl;
            cout << "--------------------------------------------------------------------------------" << endl;

            if (!marksRes->next())
            {
                cout << "No marks found for the selected exam." << endl;
            }
            else
            {
                do
                {
                    string enrollmentNo = marksRes->getString("enrollmentno");
                    string fullName = marksRes->getString("full_name");
                    int marks = marksRes->getInt("marks");
                    cout << left << setw(20) << enrollmentNo << setw(30) << fullName << setw(10) << marks << endl;
                } while (marksRes->next());
            }

            delete marksPstmt;
            delete pstmt;
            delete con;
        }
        catch (sql::SQLException& e)
        {
            cout << "Error: " << e.what() << endl;
        }

        cout << "Press any key to continue...";
        cin.ignore();
        cin.get();
    }

    void viewStats()
    {
        int selectedExamId;
        clearscreen();
        try
        {
            sql::Connection* con = getConnection();

            sql::PreparedStatement* pstmt = con->prepareStatement(
                "SELECT e.exam_id, e.exam_type, e.exam_date, c.course_code, c.course_name, c.year "
                "FROM exams e "
                "JOIN courses c ON e.course_code = c.course_code "
                "JOIN faculty_info f ON c.faculty_id = f.faculty_id "
                "WHERE f.faculty_id = ? AND c.department = ? "
                "ORDER BY c.year, e.exam_date;");
            pstmt->setInt(1, faculty_id);
            pstmt->setString(2, department);

            sql::ResultSet* res = pstmt->executeQuery();
            cout << setw(10) << "Exam ID" << setw(30) << "Exam Type" << setw(15) << "Date"
                << setw(15) << "Course Code" << setw(40) << "Course Name" << setw(7) << "Year" << endl;
            cout << "---------------------------------------------------------------------------------------------------------" << endl;

            while (res->next())
            {
                cout << setw(10) << res->getInt("exam_id") << setw(30) << res->getString("exam_type")
                    << setw(15) << res->getString("exam_date") << setw(15) << res->getString("course_code")
                    << setw(40) << res->getString("course_name") << setw(7) << res->getString("year") << endl;
            }

            cout << "Enter the exam ID to view statistics: ";
            cin >> selectedExamId;

            sql::PreparedStatement* statsPstmt = con->prepareStatement(
                "SELECT AVG(m.marks) AS average_marks, MIN(m.marks) AS min_marks, MAX(m.marks) AS max_marks, "
                "COUNT(m.enrollmentno) AS total_students, "
                "SUM(CASE WHEN m.marks > (SELECT AVG(marks) FROM marks WHERE exam_id = ?) THEN 1 ELSE 0 END) AS students_above_avg, "
                "SUM(CASE WHEN m.marks < (SELECT AVG(marks) FROM marks WHERE exam_id = ?) THEN 1 ELSE 0 END) AS students_below_avg "
                "FROM marks m "
                "WHERE m.exam_id = ?;");
            statsPstmt->setInt(1, selectedExamId);
            statsPstmt->setInt(2, selectedExamId);
            statsPstmt->setInt(3, selectedExamId);

            sql::ResultSet* statsRes = statsPstmt->executeQuery();

            if (statsRes->next())
            {
                double averageMarks = statsRes->getDouble("average_marks");
                int minMarks = statsRes->getInt("min_marks");
                int maxMarks = statsRes->getInt("max_marks");
                int totalStudents = statsRes->getInt("total_students");
                int studentsAboveAvg = statsRes->getInt("students_above_avg");
                int studentsBelowAvg = statsRes->getInt("students_below_avg");

                cout << endl
                    << endl
                    << "Statistics for Exam ID: " << selectedExamId << endl
                    << endl
                    << endl;
                cout << "Average Marks: " << averageMarks << endl;
                cout << "Minimum Marks: " << minMarks << endl;
                cout << "Maximum Marks: " << maxMarks << endl;
                cout << "Total Students: " << totalStudents << endl;
                cout << "Students Scoring Above Average: " << studentsAboveAvg << endl;
                cout << "Students Scoring Below Average: " << studentsBelowAvg << endl;
            }
            else
            {
                cout << endl
                    << "No statistics available for the selected exam." << endl;
            }

            delete statsPstmt;
            delete pstmt;
            delete con;
        }
        catch (sql::SQLException& e)
        {
            cout << "Error: " << e.what() << endl;
        }

        cout << "Press any key to continue...";
        cin.ignore();
        cin.get();

        Facultymenu();
    }

    void generateCSVReport()
    {
        string filePath;
        clearscreen();
        cout << "Enter the file path to save the CSV report  with file name(e.g. C:\\Reports\\exam_report.csv): ";
        cin.ignore();
        getline(cin, filePath);
        int selectedExamId;

        try
        {
            sql::Connection* con = getConnection();

            cout << "Enter the exam ID to generate the report: ";
            cin >> selectedExamId;

            // Verify if exam exists
            sql::PreparedStatement* checkPstmt = con->prepareStatement(
                "SELECT e.exam_type, c.course_name FROM exams e "
                "JOIN courses c ON e.course_code = c.course_code "
                "WHERE e.exam_id = ?");
            checkPstmt->setInt(1, selectedExamId);
            sql::ResultSet* checkRes = checkPstmt->executeQuery();

            if (!checkRes->next())
            {
                cout << "Exam ID not found!" << endl;
                return;
            }

            string examType = checkRes->getString("exam_type");
            string courseName = checkRes->getString("course_name");

            sql::PreparedStatement* pstmt = con->prepareStatement(
                "SELECT s.enrollmentno, CONCAT(s.fname, ' ', s.lname) AS full_name, m.marks "
                "FROM marks m "
                "JOIN students_info s ON m.enrollmentno = s.enrollmentno "
                "WHERE m.exam_id = ? "
                "ORDER BY s.enrollmentno");
            pstmt->setInt(1, selectedExamId);
            sql::ResultSet* res = pstmt->executeQuery();

            ofstream csvFile(filePath);
            if (!csvFile.is_open())
            {
                cout << "Error: Could not open file for writing!" << endl;
                return;
            }

            // Write report header
            csvFile << "Exam Report\n";
            csvFile << "Exam Type: " << examType << "\n";
            csvFile << "Course: " << courseName << "\n";

            // Get current date
            time_t t = time(nullptr);
            tm* tm = localtime(&t);
            csvFile << "Date: " << put_time(tm, "%Y-%m-%d") << "\n\n";

            // Write column headers
            csvFile << "Enrollment No,Student Name,Marks Obtained,Status\n";

            int totalStudents = 0;
            int passedStudents = 0;
            int marksSum = 0;
            int highestMarks = INT_MIN;
            int lowestMarks = INT_MAX;

            while (res->next())
            {
                int marksObtained = res->getInt("marks");
                string status = (marksObtained >= 40) ? "PASS" : "FAIL";

                csvFile << res->getString("enrollmentno") << ","
                    << res->getString("full_name") << ","
                    << marksObtained << ","
                    << status << "\n";

                totalStudents++;
                marksSum += marksObtained;

                if (marksObtained > highestMarks)
                    highestMarks = marksObtained;
                if (marksObtained < lowestMarks)
                    lowestMarks = marksObtained;

                if (marksObtained >= 40)
                    passedStudents++;
            }

            // Calculate average marks
            double averageMarks = totalStudents > 0 ? static_cast<double>(marksSum) / totalStudents : 0.0;

            // Write summary
            csvFile << "\nSummary\n";
            csvFile << "Total Students," << totalStudents << "\n";
            csvFile << "Passed Students," << passedStudents << "\n";
            csvFile << "Average Marks," << fixed << setprecision(2) << averageMarks << "\n";
            csvFile << "Highest Marks," << highestMarks << "\n";
            csvFile << "Lowest Marks," << lowestMarks << "\n";

            csvFile.close();
            cout << "Report successfully generated at: " << filePath << endl;

            delete res;
            delete pstmt;
            delete checkRes;
            delete checkPstmt;
            delete con;
        }
        catch (sql::SQLException& e)
        {
            cout << "Error: " << e.what() << endl;
        }

        cout << "\nPress Enter to continue...";
        cin.ignore();
        cin.get();
        Facultymenu();
    }
};

class hod : public common_variables, public Database
{ // class for hod
private:
    string fname, department, course;

public:
    string exam_date, exam_time, exam_duration, course_name, year, dept;
    int max_marks, passing_marks, semester, course_code, faculty;
    void hodmenu()
    { // function for hod menu
        clearscreen();
        cout << "Welcome " << fname << endl
            << endl;
        cout << "Head of Department Dashboard" << endl;
        cout << "1. View students" << endl
            << "2. View faculty" << endl
            << "3. Add course" << endl
            << "4. View courses" << endl
            << "5. Delete course" << endl
            << "6. Add Exam" << endl;
        cout << "7. View exam details" << endl
            << "8. Logout" << endl
            << "Enter a option:";
        cin >> choice;
        if (choice == 1)
        {
            clearscreen();
            viewStudents();
        }
        else if (choice == 2)
        {
            clearscreen();
            viewFaculty();
        }
        else if (choice == 4)
        {
            clearscreen();
            viewCourses();
        }
        else if (choice == 3)
        {
            clearscreen();
            addCourse();
        }
        else if (choice == 5)
        {
            clearscreen();
            // deleteCourse();
        }
        else if (choice == 6)
        {
            clearscreen();
            addExam();
        }
        else if (choice == 7)
        {
            clearscreen();
            viewExam();
        }
        else if (choice == 8)
        {
            clearscreen();
            mainscreen();
        }
    }

    void addExam()
    { // function for adding exam
        clearscreen();
        cout << "Add Exam" << endl
            << endl;
        try
        {
            cout << left << setw(14) << "Course Code" << setw(14) << "Course Name" << setw(7) << "Year" << setw(12) << "Semester" << setw(30) << "Faculty Name" << endl;
            cout << "----------------------------------------------------------------------------------------------------------" << endl;
            sql::Connection* con = getConnection();
            sql::PreparedStatement* pstmt = con->prepareStatement("SELECT c.course_code, c.course_name, c.year, c.semester, concat (f.fname, ' ' , f.mname, ' ', f.lname) as faculty_name FROM courses c JOIN faculty_info f ON c.faculty_id = f.faculty_id WHERE f.department = ?");
            pstmt->setString(1, department);
            sql::ResultSet* res = pstmt->executeQuery();
            while (res->next())
            {
                cout << left << setw(14) << res->getInt("course_code") << setw(14) << res->getString("course_name") << setw(7) << res->getInt("year") << setw(12) << res->getInt("semester") << setw(30) << res->getString("faculty_name") << endl;
            }
            delete con;
        }
        catch (sql::SQLException& e)
        {
            cout << "Error: " << e.what() << endl;
        }
        string exam_type;
        cout << "Enter course code: ";
        cin >> course_code;
        cin.ignore();
        cout << "Enter Exam Type (Unit Test,Class Test,Final test) :";
        getline(cin, exam_type);
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
        try
        {
            sql::Connection* con = getConnection();
            sql::PreparedStatement* pstmt = con->prepareStatement("INSERT INTO exams (course_code, exam_date, exam_time, exam_duration, max_marks, passing_marks,exam_type) VALUES (?,?,?,?,?,?,?)");
            pstmt->setInt(1, course_code);
            pstmt->setString(2, exam_date);
            pstmt->setString(3, exam_time);
            pstmt->setString(4, exam_duration);
            pstmt->setInt(5, max_marks);
            pstmt->setInt(6, passing_marks);
            pstmt->setString(7, exam_type);
            pstmt->execute();
            cout << "Exam added successfully." << endl;
            delete pstmt;
            delete con;

            cout << endl;
        hod_add_exam:
            cout << "1. Back to HOD Dashborad" << endl
                << "Enter a choice: ";
            cin >> choice;
            if (choice == 1)
            {
                clearscreen();
                hodmenu();
            }
            else
            {
                cout << "Invalid choice !! " << endl;
                goto hod_add_exam;
            }
        }
        catch (sql::SQLException& e)
        {
            cout << "Error : " << e.what() << endl;
        }
    }

    bool loginhod(string& emailid, string& password)
    { // function for login
        int choice;
        try
        {
            pstmt = con->prepareStatement("SELECT password, department, fname FROM faculty_info WHERE emailid = ?");
            pstmt->setString(1, emailid);
            res = pstmt->executeQuery();

            if (res->next())
            {
                string storedPassword = res->getString("password");
                department = res->getString("department");
                fname = res->getString("fname");

                if (storedPassword == password)
                {
                    hodmenu();
                    delete res;
                    delete pstmt;
                    delete con;
                    return true;
                }
                else
                {
                    cout << "Incorrect password." << endl
                        << endl;
                Faculty_exception1_re:
                    cout << "1. Go back to main menu" << endl
                        << "Enter an option: ";
                    cin >> choice;

                    if (choice == 1)
                    {
                        clearscreen();
                        mainscreen(); // Go back to main menu
                    }
                    else
                    {
                        cout << "Invalid option, please try again." << endl;
                        goto Faculty_exception1_re;
                    }
                }
            }
            else
            {
            Faculty_exception2_re:
                // If the email is not found
                cout << "Email ID not found.\n";
                cout << "1. Go back to main menu" << endl
                    << "Enter a option:";

                cin >> choice;
                if (choice == 1)
                {
                    clearscreen();
                    mainscreen();
                }
                else
                {

                    cout << "Invalid option, please try again." << endl;
                    goto Faculty_exception2_re;
                }
            }
            delete res;
            delete pstmt;
        }
        catch (sql::SQLException& e)
        {
            cerr << "Error during login: " << e.what() << endl;
        }

        delete con;
        return false;
    }

    void viewStudents()
    { //  function to view students
        cout << department << " " << "Department Students" << endl
            << endl
            << endl;
        try
        {
            pstmt = con->prepareStatement("SELECT enrollmentno, CONCAT(fname, ' ', mname, ' ', lname) AS Name, "
                "department, year, emailid, mobile, birthdate, address, gender, admission_date, "
                "guardian_name, guardian_contact, blood_group, nationality, category, aadhar_number "
                "FROM students_info WHERE department = ?");
            pstmt->setString(1, department);
            res = pstmt->executeQuery();

            const int widthEnrollment = 15;      // enrollmentno (bigint)
            const int widthName = 25;            // Name (concatenation of fname, mname, lname)
            const int widthDepartment = 6;       // department (varchar(2)) reduced space between dept & name
            const int widthYear = 6;             // year (varchar(2)) reduced gap between dept & year
            const int widthEmail = 27;           // emailid (varchar(50)) reduced gap between email & mobile
            const int widthMobile = 12;          // mobile (bigint)
            const int widthBirthdate = 12;       // birthdate (varchar(10))
            const int widthAddress = 30;         // address (varchar(100)), nearby 20 characters as you requested
            const int widthGender = 8;           // gender (varchar(10))
            const int widthAdmissionDate = 18;   // admission_date (date)
            const int widthGuardianName = 20;    // guardian_name (varchar(50))
            const int widthGuardianContact = 19; // guardian_contact (bigint)
            const int widthBloodGroup = 10;      // blood_group (varchar(3))
            const int widthNationality = 13;     // nationality (varchar(30))
            const int widthCategory = 10;        // category (varchar(20))
            const int widthAadharNumber = 15;    // aadhar_number (bigint)

            cout << left << setw(widthEnrollment) << "Enrollment No" << setw(widthName) << "Name" << setw(widthDepartment) << "Dept" << setw(widthYear) << "Year" << setw(widthEmail) << "Email ID" << setw(widthMobile) << "Mobile" << setw(widthBirthdate) << "Birthdate" << setw(widthAddress) << "Address" << setw(widthGender) << "Gender" << setw(widthAdmissionDate) << "Admission Date" << setw(widthGuardianName) << "Guardian Name" << setw(widthGuardianContact) << "Guardian Contact" << setw(widthBloodGroup) << "Blood G." << setw(widthNationality) << "Nationality" << setw(widthCategory) << "Category" << setw(widthAadharNumber) << "Aadhar No " << endl;

            cout << string(widthEnrollment, '-') << string(widthName, '-') << string(widthDepartment, '-') << string(widthYear, '-') << string(widthEmail, '-') << string(widthMobile, '-') << string(widthBirthdate, '-') << string(widthAddress, '-') << string(widthGender, '-') << string(widthAdmissionDate, '-') << string(widthGuardianName, '-') << string(widthGuardianContact, '-') << string(widthBloodGroup, '-') << string(widthNationality, '-') << string(widthCategory, '-') << string(widthAadharNumber, '-') << endl;

            while (res->next())
            {
                cout << left << setw(widthEnrollment) << res->getString("enrollmentno") << setw(widthName) << res->getString("Name") << setw(widthDepartment) << res->getString("department") << setw(widthYear) << res->getString("year") << setw(widthEmail) << res->getString("emailid") << setw(widthMobile) << res->getString("mobile") << setw(widthBirthdate) << res->getString("birthdate") << setw(widthAddress) << res->getString("address") << setw(widthGender) << res->getString("gender") << setw(widthAdmissionDate) << res->getString("admission_date") << setw(widthGuardianName) << res->getString("guardian_name") << setw(widthGuardianContact) << res->getString("guardian_contact") << setw(widthBloodGroup) << res->getString("blood_group") << setw(widthNationality) << res->getString("nationality") << setw(widthCategory) << res->getString("category") << setw(widthAadharNumber) << res->getString("aadhar_number") << endl;
            }
            cout << endl
                << endl;
            cout << "1. Back to HOD dashboard" << endl;
            cout << "2. Refresh" << endl;
            cout << "Enter an option:" << endl;
            cin >> choice;
            cout << endl
                << endl;
            clearscreen();

            if (choice == 1)
            {
                hodmenu();
            }
            else if (choice == 2)
            {
                viewFaculty();
            }
            else
            {
                cout << "Invalid choice. Please try again." << endl;
                viewFaculty();
            }
        }
        catch (sql::SQLException& e)
        {
            cout << "Error : " << e.what() << endl;
        }
    }

    void viewFaculty()
    { // Function to view all faculties
        cout << department << " " << "Department Faculties" << endl
            << endl
            << endl;
        try
        {
            unique_ptr<sql::Statement> stmt(con->createStatement());
            string query = "SELECT faculty_id, CONCAT(fname, ' ', mname, ' ', lname) AS Name, "
                "department, emailid, mobile, birthdate, address, joining_date, gender, experience_years,post "
                "FROM faculty_info WHERE post = 'Faculty'";
            unique_ptr<sql::ResultSet> res(stmt->executeQuery(query));

            const int widthFacultyID = 12;   // Facultyid (int)
            const int widthName = 25;        // Name (concatenation of fname, mname, lname)
            const int widthDepartment = 7;   // department (varchar(2))
            const int widthEmail = 25;       // emailid (varchar(50))
            const int widthMobile = 15;      // mobile (bigint)
            const int widthBirthdate = 15;   // birthdate (varchar(10))
            const int widthAddress = 40;     // address (varchar(200))
            const int widthJoiningDate = 12; // joining_date (date)
            const int widthGender = 10;      // gender (varchar(10))
            const int widthExperience = 20;  // experience_years (int)
            const int widthPost = 10;

            cout << left << setw(widthFacultyID) << "Faculty ID" << setw(widthName) << "Name" << setw(widthDepartment) << "Dept" << setw(widthEmail) << "Email ID" << setw(widthMobile) << "Mobile" << setw(widthBirthdate) << "Birthdate" << setw(widthJoiningDate) << "Joining Date" << setw(widthGender) << "Gender" << setw(widthExperience) << "Experience (Years)" << setw(widthPost) << "Post" << endl;

            cout << string(widthFacultyID, '-') << string(widthName, '-') << string(widthDepartment, '-') << string(widthEmail, '-') << string(widthMobile, '-') << string(widthBirthdate, '-') << string(widthJoiningDate, '-') << string(widthGender, '-') << string(widthExperience, '-') << string(widthPost, '-') << endl;

            while (res->next())
            {
                cout << left << setw(widthFacultyID) << res->getInt("faculty_id") << setw(widthName) << res->getString("Name") << setw(widthDepartment) << res->getString("department") << setw(widthEmail) << res->getString("emailid") << setw(widthMobile) << res->getInt("mobile") << setw(widthBirthdate) << res->getString("birthdate") << setw(widthJoiningDate) << res->getString("joining_date") << setw(widthGender) << res->getString("gender") << setw(widthExperience) << res->getInt("experience_years") << setw(widthPost) << res->getString("post") << endl;
            }

            int choice;
            cout << endl
                << "1. Back to HOD dashboard" << endl;
            cout << "2. Refresh" << endl;
            cout << "Enter your option: ";
            cin >> choice;
            cout << endl;

            clearscreen();
            if (choice == 1)
            {
                hodmenu();
            }
            else if (choice == 2)
            {
                viewFaculty();
            }
            else
            {
                cout << "Invalid choice. Please try again." << endl;
                viewFaculty();
            }
        }
        catch (sql::SQLException& e)
        {
            cerr << "SQLException: " << e.what() << endl;
            cerr << "MySQL error code: " << e.getErrorCode() << endl;
            cerr << "SQLState: " << e.getSQLState() << endl;
        }
    }

    void addCourse()
    { // Add course
        clearscreen();
        cout << "Enter Course Details Below " << endl
            << endl
            << endl
            << "Enter Course code : ";
        cin >> course_code;
        cin.ignore();
        cout << "Enter Course name : ";
        getline(cin, course_name);
        cout << setw(5) << "Faculty ID : "
            << setw(30) << "Faculty Name" << endl;
        cout << "--------------------------------------------------------------------------------------------------------------------------------" << endl;
        try
        {
            pstmt = con->prepareStatement("SELECT CONCAT(fname,' ', mname,' ', lname) AS name,faculty_id FROM faculty_info WHERE department = ? ORDER BY faculty_id");
            pstmt->setString(1, department);
            res = pstmt->executeQuery();
            while (res->next())
            {
                cout << setw(5) << res->getInt("faculty_id")
                    << setw(30) << res->getString("name") << endl;
            }
        }
        catch (sql::SQLException& e)
        {
            cerr << "SQL Error: " << e.what() << endl;
        }
        cout << endl
            << "Enter Faculty ID :";
        cin >> faculty;
        cout << "Enter year (FY,SY,TY) :";
        cin >> year;
        cout << "Enter semester (1,2,3,4,5,6) :";
        cin >> semester;
        try
        {
            pstmt = con->prepareStatement("INSERT INTO courses (course_code, course_name, department, semester, year, faculty_id) VALUES (? ,?, ?, ?, ?, ?)");
            pstmt->setInt(1, course_code);
            pstmt->setString(2, course_name);
            pstmt->setString(3, department);
            pstmt->setInt(4, semester);
            pstmt->setString(5, year);
            pstmt->setInt(6, faculty);
            pstmt->executeQuery();

            cout << "Course added successfully!" << endl
                << endl;
        hod_add_course_re:
            cout << "1. Add another course" << endl
                << "2. Back to HOD dashboard" << endl;
            cout << "Enter your option: ";
            cin >> choice;
            if (choice == 1)
            {
                addCourse();
            }
            else if (choice == 2)
            {
                hodmenu();
            }
            else
            {
                cout << "Invalid choice. Please try again." << endl;
                goto hod_add_course_re;
            }
        }
        catch (sql::SQLException& e)
        {
            cerr << "SQL Error: " << e.what() << endl;
        }
    }

    void viewCourses()
    { // view courses
        clearscreen();
        cout << left << setw(13) << "Course Code" << setw(40) << "Course Name" << setw(13) << "Department" << setw(11) << "Semester" << setw(7) << "Year" << setw(30) << "Faculty Name" << endl;
        cout << "--------------------------------------------------------------------------------------------------------------------------------" << endl;
        try
        {
            pstmt = con->prepareStatement("SELECT c.course_code,c.course_name,c.department,c.semester,c.year,concat(f.fname, ' ' , f.mname , ' ' , f.lname) as faculty_name FROM courses AS c JOIN  faculty_info AS f ON c.faculty_id = f.faculty_id;");
            res = pstmt->executeQuery();
            while (res->next())
            {
                cout << left << setw(13) << res->getInt("course_code") << setw(40) << res->getString("course_name") << setw(13) << res->getString("department") << setw(11) << res->getInt("semester") << setw(7) << res->getString("year") << setw(30) << res->getString("faculty_name") << endl;
            }
        hod_view_course_re:
            cout << "1. Back to HOD Dashboard" << endl;
            cout << "2.Refresh " << endl;
            cout << "Enter a option : ";
            cin >> choice;
            if (choice == 1)
            {
                clearscreen();
            }
            else if (choice == 2)
            {
                clearscreen();
                viewCourses();
            }
            else
            {
                cout << "Invalid choice" << endl;
                goto hod_view_course_re;
            }
        }
        catch (sql::SQLException& e)
        {
            cerr << "SQL Error: " << e.what() << endl;
        }
    }

    void viewExam()
    { // view exam details
        cout << "Previously conducted exams" << endl
            << endl;
        cout << left << setw(10) << "Exam ID" << setw(15) << "Course Code" << setw(40) << "Course Name" << setw(13) << "Exam Type" << setw(12) << "Exam Date" << setw(15) << "Exam Duration" << setw(12) << "Exam Time" << setw(12) << "Max Marks" << setw(12) << "Pass Marks" << endl;
        cout << "--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------" << endl;
        try
        {
            pstmt = con->prepareStatement("SELECT e.exam_id,e.course_code,c.course_name,e.exam_type,e.exam_date,e.exam_duration,e.exam_time,e.max_marks,e.passing_marks FROM exams AS e JOIN courses AS c ON e.course_code = c.course_code");
            res = pstmt->executeQuery();
            while (res->next())
            {
                cout << left << setw(10) << res->getInt("exam_id") << setw(15) << res->getInt("course_code") << setw(40) << res->getString("course_name") << setw(15) << res->getString("exam_type") << setw(12) << res->getString("exam_date") << setw(15) << res->getString("exam_duration") << setw(12) << res->getString("exam_time") << setw(12) << res->getInt("max_marks") << setw(12) << res->getInt("passing_marks") << endl;
            }
        hod_view_exam_re:
            cout << "1. Back to HOD dashboard" << endl;
            cout << "2. Refresh" << endl;
            cout << "Enter your option: ";
            cin >> choice;
            if (choice == 1)
            {
                hodmenu();
            }
            else if (choice == 2)
            {
                viewExam();
            }
            else
            {
                cout << "Invalid choice. Please try again." << endl;
                goto hod_view_exam_re;
            }
        }
        catch (sql::SQLException& e)
        {
            cerr << "SQL Error: " << e.what() << endl;
        }
    }
};

class student : public common_variables, public Database
{
private:
    string email_id, password, retrivedpass, fname, branch, year, semester, lname, department;
    int enrollmentno;

public:
    void studentmenu()
    {
        cout << "1. View my profile" << endl
            << "2. View courses" << endl
            << "3. View exams" << endl
            << "4. View results" << endl
            << "5. Logout" << endl
            << endl
            << "Enter your choice: ";
        cin >> choice;

        if (choice == 1)
        {
            viewprofile();
        }
        else if (choice == 2)
        {
            viewcourses();
        }
        else if (choice == 3)
        {
            viewExams();
        }
        else if (choice == 4)
        {
            viewresult();
        }
        else if (choice == 5)
        {
            mainscreen();
        }
        else
        {
            cout << "Invalid choice. Please try again." << endl;
            studentmenu();
        }
    }

    void studentLogin()
    {
        clearscreen();
        cout << "Student Login" << endl
            << endl;
        cout << "Enter your Email ID: ";
        cin >> email_id;
        cout << "Enter your password: ";
        cin >> password;

        try
        {
            sql::Connection* con = getConnection();
            // Query to get all needed student information
            sql::PreparedStatement* pstmt = con->prepareStatement(
                "SELECT enrollmentno, password, department, fname, lname, year "
                "FROM students_info WHERE emailid = ?");
            pstmt->setString(1, email_id);
            sql::ResultSet* res = pstmt->executeQuery();

            if (res->next())
            {
                
                enrollmentno = res->getInt("enrollmentno"); 
                retrivedpass = res->getString("password");
                department = res->getString("department");
                fname = res->getString("fname");
                lname = res->getString("lname");
                year = res->getString("year");

                if (password == retrivedpass)
                {
                    cout << "Login successful!" << endl;
                    cout << "Welcome " << fname << " " << lname << "!" << endl;
                    cout << "Enrollment No: " << enrollmentno << endl; 
                    clearscreen();
                    studentmenu();
                }
                else
                {
                    cout << "Invalid password." << endl
                        << endl;
                    cout << "1. Try again" << endl;
                    cout << "2. Back to main menu" << endl;
                    cout << "Enter choice: ";
                    cin >> choice;

                    if (choice == 1)
                    {
                        clearscreen();
                        studentLogin();
                    }
                    else
                    {
                        clearscreen();
                        mainscreen();
                    }
                }
            }
            else
            {
                cout << "Email ID not found." << endl
                    << endl;
                cout << "1. Try again" << endl;
                cout << "2. Back to main menu" << endl;
                cout << "Enter choice: ";
                cin >> choice;

                if (choice == 1)
                {
                    clearscreen();
                    studentLogin();
                }
                else
                {
                    clearscreen();
                    mainscreen();
                }
            }

            delete res;
            delete pstmt;
            delete con;
        }
        catch (sql::SQLException& e)
        {
            cerr << "Database error: " << e.what() << endl;
        }
    }

    void viewprofile()
    {
        clearscreen();
        try
        {
            pstmt = con->prepareStatement(
                "SELECT enrollmentno, CONCAT(fname, ' ', mname, ' ', lname) AS full_name, "
                "department, year, emailid, mobile, birthdate, address, gender, "
                "admission_date, guardian_name, guardian_contact, blood_group, "
                "nationality, category, aadhar_number "
                "FROM students_info WHERE emailid = ?");

            pstmt->setString(1, email_id);
            res = pstmt->executeQuery();

            if (res->next())
            {
                cout << "Student Profile" << endl;
                cout << "----------------------------------------" << endl;
                cout << "Enrollment No: " << res->getString("enrollmentno") << endl;
                cout << "Full Name: " << res->getString("full_name") << endl;
                cout << "Department: " << res->getString("department") << endl;
                cout << "Year: " << res->getString("year") << endl;
                cout << "Email ID: " << res->getString("emailid") << endl;
                cout << "Mobile: " << res->getString("mobile") << endl;
                cout << "Birth Date: " << res->getString("birthdate") << endl;
                cout << "Gender: " << res->getString("gender") << endl;
                cout << "Admission Date: " << res->getString("admission_date") << endl;
                cout << "Guardian Name: " << res->getString("guardian_name") << endl;
                cout << "Guardian Contact: " << res->getString("guardian_contact") << endl;
                cout << "Blood Group: " << res->getString("blood_group") << endl;
                cout << "Nationality: " << res->getString("nationality") << endl;
                cout << "Category: " << res->getString("category") << endl;
                cout << "Aadhar Number: " << res->getString("aadhar_number") << endl;
                cout << "Address: " << res->getString("address") << endl;
                cout << "----------------------------------------" << endl;
            }

            cout << endl
                << "1. Back to Student Menu" << endl;
            cout << "Enter your choice: ";
            cin >> choice;

            if (choice == 1)
            {
                clearscreen();
                studentmenu();
            }
        }
        catch (sql::SQLException& e)
        {
            cerr << "Error viewing profile: " << e.what() << endl;
            studentmenu();
        }
    }

    void viewExams()
    {
        clearscreen();
        cout << "Upcoming Exams" << endl
            << endl;

        try
        {
            // Ensure that `con` is properly initialized
            sql::Connection* con = getConnection();

            sql::PreparedStatement* pstmt = con->prepareStatement(
                "SELECT e.exam_id, e.exam_type, e.exam_date, e.exam_time, "
                "e.exam_duration, e.max_marks, e.passing_marks, "
                "c.course_name, c.course_code "
                "FROM exams e "
                "JOIN courses c ON e.course_code = c.course_code "
                "WHERE c.year = (SELECT year FROM students_info WHERE enrollmentno = ?) "
                "AND c.department = (SELECT department FROM students_info WHERE enrollmentno = ?) "
                "AND STR_TO_DATE(e.exam_date, '%d/%m/%y') >= CURDATE() "
                "ORDER BY STR_TO_DATE(e.exam_date, '%d/%m/%y'), e.exam_time");

            pstmt->setInt(1, enrollmentno);
            pstmt->setInt(2, enrollmentno);
            sql::ResultSet* res = pstmt->executeQuery();

            cout << left
                << setw(10) << "Exam ID"
                << setw(15) << "Course Code"
                << setw(40) << "Course Name"
                << setw(15) << "Exam Type"
                << setw(12) << "Date"
                << setw(10) << "Time"
                << setw(15) << "Duration"
                << setw(12) << "Max Marks"
                << setw(12) << "Pass Marks"
                << endl;

            cout << string(141, '-') << endl;
            while (res->next())
            {
                cout << left
                    << setw(10) << res->getInt("exam_id")
                    << setw(15) << res->getString("course_code")
                    << setw(40) << res->getString("course_name")
                    << setw(15) << res->getString("exam_type")
                    << setw(12) << res->getString("exam_date")
                    << setw(10) << res->getString("exam_time")
                    << setw(15) << res->getString("exam_duration")
                    << setw(12) << res->getInt("max_marks")
                    << setw(12) << res->getInt("passing_marks")
                    << endl;
            }

            delete res;
            delete pstmt;
            delete con;

            int choice;
            cout << endl
                << "1. Back to Student Menu" << endl;
            cout << "2. Refresh" << endl;
            cout << "Enter your choice: ";
            cin >> choice;

            if (choice == 1)
            {
                clearscreen();
                studentmenu();
            }
            else if (choice == 2)
            {
                clearscreen();
                viewExams();
            }
        }
        catch (sql::SQLException& e)
        {
            cerr << "Error viewing exams: " << e.what() << endl;
            studentmenu();
        }
    }

    void viewcourses()
    {
        clearscreen();
        cout << "Your Courses" << endl
            << endl;

        try
        {
            sql::Connection* con = getConnection();

            sql::PreparedStatement* courseStmt = con->prepareStatement(
                "SELECT c.course_code, c.course_name, c.semester, "
                "CONCAT(f.fname, ' ', f.lname) as faculty_name "
                "FROM courses c "
                "JOIN faculty_info f ON c.faculty_id = f.faculty_id "
                "WHERE c.department = ? AND c.year = ? "
                "ORDER BY c.semester");

            courseStmt->setString(1, department);
            courseStmt->setString(2, year);
            sql::ResultSet* courseRes = courseStmt->executeQuery();

            cout << left
                << setw(15) << "Course Code"
                << setw(40) << "Course Name"
                << setw(15) << "Semester"
                << setw(30) << "Faculty"
                << endl;
            cout << string(100, '-') << endl;

            int courseCount = 0;
            while (courseRes->next())
            {
                courseCount++;
                cout << left
                    << setw(15) << courseRes->getString("course_code")
                    << setw(40) << courseRes->getString("course_name")
                    << setw(15) << courseRes->getString("semester")
                    << setw(30) << courseRes->getString("faculty_name")
                    << endl;
            }

            cout << "\nTotal courses: " << courseCount << endl;

            delete courseRes;
            delete courseStmt;
            delete con;

            cout << "\nPress Enter to continue...";
            cin.ignore();
            cin.get();
            clearscreen();
            studentmenu();
        }
        catch (sql::SQLException& e)
        {
            cerr << "Database error: " << e.what() << endl;
            studentmenu();
        }
    }

    void viewresult()
    {
        clearscreen();
        cout << "Your Exam Results" << endl
            << endl;

        try
        {
            sql::Connection* con = getConnection();

            // First display exams the student has taken
            sql::PreparedStatement* examStmt = con->prepareStatement(
                "SELECT DISTINCT e.exam_id, e.exam_type, c.course_name, e.exam_date, "
                "e.max_marks, e.passing_marks "
                "FROM exams e "
                "JOIN courses c ON e.course_code = c.course_code "
                "JOIN marks m ON e.exam_id = m.exam_id "
                "JOIN students_info s ON m.enrollmentno = s.enrollmentno "
                "WHERE s.emailid = ? "
                "ORDER BY STR_TO_DATE(e.exam_date, '%d/%m/%y') DESC");

            examStmt->setString(1, email_id);
            sql::ResultSet* examRes = examStmt->executeQuery();

            cout << left
                << setw(10) << "Exam ID"
                << setw(15) << "Exam Type"
                << setw(40) << "Course Name"
                << setw(15) << "Date"
                << setw(12) << "Max Marks"
                << setw(15) << "Passing Marks"
                << endl;
            cout << string(107, '-') << endl;

            while (examRes->next())
            {
                cout << left
                    << setw(10) << examRes->getInt("exam_id")
                    << setw(15) << examRes->getString("exam_type")
                    << setw(40) << examRes->getString("course_name")
                    << setw(15) << examRes->getString("exam_date")
                    << setw(12) << examRes->getInt("max_marks")
                    << setw(15) << examRes->getInt("passing_marks")
                    << endl;
            }

            int selectedExamId;
            cout << "\nEnter Exam ID to view detailed result: ";
            cin >> selectedExamId;

            // Fetch and display specific exam result
            sql::PreparedStatement* resultStmt = con->prepareStatement(
                "SELECT m.marks, e.max_marks, e.passing_marks, "
                "c.course_name, e.exam_type, "
                "CASE WHEN m.marks >= e.passing_marks THEN 'PASS' ELSE 'FAIL' END as status "
                "FROM marks m "
                "JOIN exams e ON m.exam_id = e.exam_id "
                "JOIN courses c ON e.course_code = c.course_code "
                "JOIN students_info s ON m.enrollmentno = s.enrollmentno "
                "WHERE m.exam_id = ? AND s.emailid = ?");

            resultStmt->setInt(1, selectedExamId);
            resultStmt->setString(2, email_id);
            sql::ResultSet* resultRes = resultStmt->executeQuery();

            if (resultRes->next())
            {
                clearscreen();
                cout << "\nDetailed Result" << endl;
                cout << "----------------------------------------" << endl;
                cout << "Course: " << resultRes->getString("course_name") << endl;
                cout << "Exam Type: " << resultRes->getString("exam_type") << endl;
                cout << "Marks Obtained: " << resultRes->getInt("marks") << endl;
                cout << "Maximum Marks: " << resultRes->getInt("max_marks") << endl;
                cout << "Passing Marks: " << resultRes->getInt("passing_marks") << endl;
                cout << "Status: " << resultRes->getString("status") << endl;
                cout << "----------------------------------------" << endl;
            }

            delete resultRes;
            delete examRes;
            delete resultStmt;
            delete examStmt;
            delete con;

            cout << "\n1. View Another Result" << endl;
            cout << "2. Back to Student Menu" << endl;
            cout << "Enter your choice: ";
            int choice;
            cin >> choice;

            if (choice == 1)
            {
                viewresult();
            }
            else
            {
                clearscreen();
                studentmenu();
            }
        }
        catch (sql::SQLException& e)
        {
            cerr << "Database error: " << e.what() << endl;
            studentmenu();
        }
    }
};

void mainscreen()
{ // Starting of the program
    admin a;
    Faculty t;
    hod h;
    common_variables
        var;
    student s;
    string username, password;
mainre:
    cout << "Welcome to Government Polytechnic Awasari" << endl
        << endl
        << "1. Student login" << endl
        << "2. Faculty login" << endl
        << "3. HOD login" << endl
        << "4. Admin login" << endl
        << "5. Exit" << endl
        << "Enter your choice: "; // choose one from  given roles
    cin >>
        var.choice;
    cout << endl;
    clearscreen();
    if (var.choice == 1)
    {
        cout << "   Student login   " << endl
            << endl;
        s.studentLogin();
    }
    else if (var.choice == 2)
    {
        string emailid, pass;
        cout << "Enter your email id : ";
        cin >> emailid;
        cout << "Enter password :";
        cin >> pass;
        t.loginFaculty(emailid, pass);
    }
    else if (var.choice == 4)
    {
    adminreenter:
        cout << "   Admin login   " << endl
            << endl;
        cout << "Enter admin username: ";
        cin >> username;
        cout << endl;
        cout << "Enter password: ";
        cin >> password;
        cout << endl;
        cout << "Logging in . . .";
        cout << endl
            << endl;
        clearscreen();

        if (username == "admin" && password == "admin")
        {
            cout << "Admin login successful!" << endl
                << endl;
            a.adminmenu();
        }
        else
        {
            cout << "Invalid username or password!" << endl
                << endl;
            cout << "1. Try again" << endl;
            cout << "2. Back to main menu" << endl;
            cout << "Enter your choice: ";
            int choice2;
            cin >> choice2;
            clearscreen();
            if (choice2 == 1)
            {
                goto adminreenter;
            }
            else
            {
                mainscreen();
            }
        }
    }
    else if (var.choice == 5)
    {
        exit(0);
    }
    else if (var.choice == 3)
    {
        string emailid, pass;
        cout << "Enter your email id : ";
        cin >> emailid;
        cout << "Enter password :";
        cin >> pass;
        h.loginhod(emailid, pass);
    }
    else
    {
        cout << "Enter a valid choice !" << endl
            << endl;
        goto mainre;
    }
}

int main()
{                 // seprated mainscreen function beacuse we can't main function outside main function
    mainscreen(); // Displays all User Logins
    return 0;
}