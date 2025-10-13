CREATE TABLE Department( 
    Dept_ID VARCHAR2(10) PRIMARY KEY, 
    Dept_Name VARCHAR2(50) NOT NULL UNIQUE, 
    HOD_ID INT UNIQUE,   -- allow NULL temporarily
    Year_of_Establishment INT CHECK (Year_of_Establishment BETWEEN 1950 AND 2025), 
    Dept_Address VARCHAR2(50) NOT NULL);

CREATE TABLE Faculty( 
    Faculty_ID INT PRIMARY KEY, 
    Full_Name VARCHAR2(50) UNIQUE NOT NULL, 
    Faculty_Mobile VARCHAR2(10) NOT NULL, 
    Faculty_Mail VARCHAR2(50) UNIQUE NOT NULL, 
    Dept_ID VARCHAR2(10) NOT NULL, 
    Faculty_cabin VARCHAR2(10), 
    CONSTRAINT faculty_fk_dept FOREIGN KEY (Dept_ID) REFERENCES Department(Dept_ID) ON DELETE CASCADE);

CREATE TABLE Course( 
    Course_ID VARCHAR2(20) PRIMARY KEY, 
    Course_name VARCHAR2(50) NOT NULL UNIQUE, 
    Course_coord_ID INT,  -- allow NULL temporarily
    Dept_ID VARCHAR2(10) NOT NULL, 
    CONSTRAINT course_fk_dept 
        FOREIGN KEY (Dept_ID) REFERENCES Department(Dept_ID) ON DELETE CASCADE,
    CONSTRAINT course_fk_coord FOREIGN KEY (Course_coord_ID) REFERENCES Faculty(Faculty_ID) ON DELETE SET NULL);

CREATE TABLE Student( 
    Reg_no INT PRIMARY KEY, 
    Full_Name VARCHAR2(50) NOT NULL, 
    Semester INT NOT NULL CHECK (Semester BETWEEN 1 AND 8), 
    Course_ID VARCHAR2(20) NOT NULL, 
    CONSTRAINT stud_fk_cnstrnt FOREIGN KEY (Course_ID) REFERENCES Course(Course_ID) ON DELETE CASCADE);

CREATE TABLE Details( 
    Reg_no INT PRIMARY KEY, 
    Application_no INT NOT NULL UNIQUE, 
    Gender VARCHAR2(1) NOT NULL CHECK (Gender IN ('M','F')), 
    GPA FLOAT DEFAULT 0.0 CHECK (GPA BETWEEN 0 AND 10), 
    Learner_mail VARCHAR2(50) UNIQUE NOT NULL, 
    DOB DATE NOT NULL, 
    NRI_Category INT NOT NULL CHECK (NRI_Category BETWEEN 0 AND 1), 
    Address VARCHAR2(100) NOT NULL, 
    CONSTRAINT details_fk_cnstrnt FOREIGN KEY (Reg_no) REFERENCES Student(Reg_no) ON DELETE CASCADE);

CREATE TABLE History_details (
    reg_no        VARCHAR2(10),
    old_gpa       NUMBER(4,2),
    new_gpa       NUMBER(4,2),
    old_address   VARCHAR2(100),
    new_address   VARCHAR2(100),
    updated_on    DATE );

-- Department
INSERT INTO Department VALUES ('CSE', 'Computer Science', NULL, 2001, 'Block A');
INSERT INTO Department VALUES ('ECE', 'Electronics and Communication', NULL, 2003, 'Block B');
INSERT INTO Department VALUES ('ME', 'Mechanical Engineering', NULL, 1998, 'Block C');
INSERT INTO Department VALUES ('CIV', 'Civil Engineering', NULL, 2005, 'Block D');
-- 2️⃣ Faculty
INSERT INTO Faculty VALUES (101, 'Dr. Priya Menon', '9876543210', 'priya@college.edu', 'CSE', 'A101');
INSERT INTO Faculty VALUES (102, 'Dr. Arun Kumar', '9876501234', 'arun@college.edu', 'ECE', 'B202');
INSERT INTO Faculty VALUES (103, 'Dr. Kiran Joseph', '9998854321', 'kiran@college.edu', 'ME', 'C105');
INSERT INTO Faculty VALUES (104, 'Dr. Neha Varma', '9887765432', 'neha@college.edu', 'CIV', 'D301');
INSERT INTO Faculty VALUES (105, 'Prof. John Mathew', '9876123450', 'john@college.edu', 'CSE', 'A102');
INSERT INTO Faculty VALUES (106, 'Prof. Sneha Iyer', '9865123490', 'sneha@college.edu', 'ECE', 'B204');
-- 3️⃣ Update HODs
UPDATE Department SET HOD_ID = 101 WHERE Dept_ID = 'CSE';
UPDATE Department SET HOD_ID = 102 WHERE Dept_ID = 'ECE';
UPDATE Department SET HOD_ID = 103 WHERE Dept_ID = 'ME';
UPDATE Department SET HOD_ID = 104 WHERE Dept_ID = 'CIV';
-- 4️⃣ Add foreign key constraint for HOD
ALTER TABLE Department ADD CONSTRAINT dept_fk_hod FOREIGN KEY (HOD_ID) REFERENCES Faculty(Faculty_ID) ON DELETE CASCADE;
-- 5️⃣ Courses
INSERT INTO Course VALUES ('CS101', 'Database Systems', 101, 'CSE');
INSERT INTO Course VALUES ('CS102', 'Operating Systems', 105, 'CSE');
INSERT INTO Course VALUES ('EC201', 'Digital Circuits', 102, 'ECE');
INSERT INTO Course VALUES ('EC202', 'Signals and Systems', 106, 'ECE');
INSERT INTO Course VALUES ('ME301', 'Thermodynamics', 103, 'ME');
INSERT INTO Course VALUES ('CIV401', 'Structural Analysis', 104, 'CIV');
-- 6️⃣ Students
INSERT INTO Student VALUES (22001, 'Rahul Raj', 5, 'CS101');
INSERT INTO Student VALUES (22002, 'Meera Nair', 5, 'CS101');
INSERT INTO Student VALUES (22003, 'Akhil Das', 3, 'CS102');
INSERT INTO Student VALUES (22004, 'Divya Suresh', 6, 'EC201');
INSERT INTO Student VALUES (22005, 'Vishnu Prasad', 4, 'EC202');
INSERT INTO Student VALUES (22006, 'Aditya Menon', 2, 'ME301');
INSERT INTO Student VALUES (22007, 'Keerthi Raj', 8, 'CIV401');
-- 7️⃣ Details
INSERT INTO Details VALUES (22001, 5001, 'M', 8.2, 'rahul@edu.com', TO_DATE('2003-03-15','YYYY-MM-DD'), 0, 'Kochi, Kerala');
INSERT INTO Details VALUES (22002, 5002, 'F', 9.0, 'meera@edu.com', TO_DATE('2003-07-21','YYYY-MM-DD'), 0, 'TVM, Kerala');
INSERT INTO Details VALUES (22003, 5003, 'M', 7.8, 'akhil@edu.com', TO_DATE('2004-01-10','YYYY-MM-DD'), 0, 'Ernakulam, Kerala');
INSERT INTO Details VALUES (22004, 5004, 'F', 8.6, 'divya@edu.com', TO_DATE('2002-09-22','YYYY-MM-DD'), 1, 'Kollam, Kerala');
INSERT INTO Details VALUES (22005, 5005, 'M', 7.9, 'vishnu@edu.com', TO_DATE('2003-11-13','YYYY-MM-DD'), 0, 'Thrissur, Kerala');
INSERT INTO Details VALUES (22006, 5006, 'M', 8.5, 'aditya@edu.com', TO_DATE('2004-04-05','YYYY-MM-DD'), 0, 'Alappuzha, Kerala');
INSERT INTO Details VALUES (22007, 5007, 'F', 9.1, 'keerthi@edu.com', TO_DATE('2002-06-19','YYYY-MM-DD'), 1, 'Kottayam, Kerala');
-- Deletion operations
DELETE FROM Details WHERE Reg_no = 22002;
DELETE FROM Student WHERE Reg_no = 22001;
DELETE FROM Course WHERE Course_ID = 'EC201';
DELETE FROM Faculty WHERE Faculty_ID = 102;

-- Retrieve HOD Contact details for all branches
-- SELECT D.Dept_Name, F.Full_Name, F.Faculty_Mobile, F.Faculty_Mail FROM Department D JOIN Faculty F ON D.HOD_ID = F.Faculty_ID;

-- Course Wise avg GPA
--SELECT C.Course_ID, C.Course_name, AVG(GPA) AS Branch_Avg_GPA FROM Details DT JOIN Student S ON DT.Reg_no = S.Reg_no
--JOIN Course C ON S.Course_ID = C.Course_ID GROUP BY C.Course_ID, C.Course_name;
-- Show Students BY Department 
--SELECT  S.Full_Name, C.Course_name, D.Dept_Name FROM Student S
--JOIN Course C ON S.Course_ID = C.Course_ID
--JOIN Department D ON C.Dept_ID = D.Dept_ID WHERE D.Dept_Name = 'Computer Science';

-- Show Students BY COURSE 
--SELECT S.Full_Name, C.Course_name, D.Dept_Name FROM Student S
--JOIN Course C ON S.Course_ID = C.Course_ID
--JOIN Department D ON C.Dept_ID = D.Dept_ID WHERE C.Course_name = 'Database Systems';

-- Show Students BY SEMESTER
--SELECT S.Full_Name, S.Semester, C.Course_name FROM Student S 
--JOIN Course C ON S.Course_ID = C.Course_ID WHERE S.Semester = 5;

-- RETRIVE DETAILS BY REGISTRATON NUMBER
--SELECT S.Reg_no,S.Full_Name,S.Semester,C.Course_name,D.Dept_Name,DT.GPA,DT.Learner_mail FROM Student S
--JOIN Course C ON S.Course_ID = C.Course_ID
--JOIN Department D ON C.Dept_ID = D.Dept_ID
--JOIN Details DT ON S.Reg_no = DT.Reg_no WHERE S.Reg_no = 22003;

-- SELECT ALL STUDENTS IN A BRANCH AND ORDER BY GPA
--SELECT S.Reg_no,S.Full_Name,S.Semester,C.Course_name,DT.GPA FROM Student S
--JOIN Course C ON S.Course_ID = C.Course_ID
--JOIN Department D ON C.Dept_ID = D.Dept_ID
--JOIN Details DT ON S.Reg_no = DT.Reg_no
--WHERE D.Dept_Name = 'Civil Engineering' ORDER BY DT.GPA DESC;

-- SELECT ALL STUDENTS IN A BRANCH AND A SEMESTER AND ORDER BY GPA 
--SELECT S.Reg_no,S.Full_Name,S.Semester,C.Course_name,DT.GPA FROM Student S
--JOIN Course C ON S.Course_ID = C.Course_ID
--JOIN Department D ON C.Dept_ID = D.Dept_ID
--JOIN Details DT ON S.Reg_no = DT.Reg_no
--WHERE D.Dept_Name = 'Computer Science' AND S.Semester = 3 ORDER BY DT.GPA DESC;

-- SELECT ALL STUDENTS IN COLLEGE AND SORT BY GPA
--SELECT S.Reg_no, S.Full_Name, S.Semester, C.Course_name, D.Dept_Name, DT.GPA FROM Student S
--JOIN Course C ON S.Course_ID = C.Course_ID
--JOIN Department D ON C.Dept_ID = D.Dept_ID
--JOIN Details DT ON S.Reg_no = DT.Reg_no ORDER BY DT.GPA DESC; 


--CALCULATE SCHOLARSHIP BASED ON GPA
CREATE OR REPLACE PROCEDURE calc_scholarship (p_regno IN Student.Reg_no%TYPE) IS
    v_gpa     Details.GPA%TYPE;
    v_amount  NUMBER;
BEGIN
    SELECT d.GPA INTO v_gpa FROM Details d WHERE d.Reg_no = p_regno;
    IF v_gpa = 10 THEN
        v_amount := 50000;
    ELSIF v_gpa >= 9.5 THEN
        v_amount := 20000;
    ELSIF v_gpa >= 9.0 THEN
        v_amount := 15000;
    ELSE
        v_amount := 0;
    END IF;
    DBMS_OUTPUT.PUT_LINE('Reg no : ' || p_regno);
    DBMS_OUTPUT.PUT_LINE('GPA: ' || v_gpa);
    DBMS_OUTPUT.PUT_LINE('Scholarship Amount: ₹' || v_amount);
EXCEPTION
    WHEN NO_DATA_FOUND THEN
        DBMS_OUTPUT.PUT_LINE('No student found with Reg_no: ' || p_regno);
    WHEN OTHERS THEN
        DBMS_OUTPUT.PUT_LINE('Error: ' || SQLERRM);
END;
/

-- GIVING BACK OR PROMOTING A STUDENT BASED ON SEM MARKS
CREATE OR REPLACE TRIGGER promote_stud BEFORE UPDATE ON Details FOR EACH ROW
BEGIN
    IF :NEW.GPA <= 4 THEN
        DBMS_OUTPUT.PUT_LINE('Reg : ' || :NEW.Reg_no || ' → UNSATISFACTORY [BACK]');
    ELSE
        UPDATE Student SET Semester = Semester + 1 WHERE Reg_no = :NEW.Reg_no;
        DBMS_OUTPUT.PUT_LINE('Reg : ' || :NEW.Reg_no || ' → SATISFACTORY [PROMOTED]');
    END IF;
END;
/

-- ADDING HISTORY OF UPDATES ON STUDENTS TABLE
CREATE OR REPLACE TRIGGER history_update AFTER UPDATE ON Details FOR EACH ROW
BEGIN
INSERT INTO history_details VALUES (:OLD.Reg_no,:OLD.GPA,:NEW.GPA,:OLD.Address,:NEW.Address,SYSDATE);
END;
/

-- SHOW STUDENT DETAILS 
CREATE OR REPLACE PROCEDURE show_student (p_regno IN Student.Reg_no%TYPE) IS
    v_name       Student.Full_Name%TYPE;
    v_semester   Student.Semester%TYPE;
    v_course     Student.Course_ID%TYPE;
    v_dob        Details.DOB%TYPE;
    v_gender     Details.Gender%TYPE;
    v_learner    Details.Learner_mail%TYPE;
    v_gpa        Details.GPA%TYPE;
    v_address    Details.Address%TYPE;
BEGIN
SELECT s.Full_Name,s.Semester,s.Course_ID,dt.DOB,dt.Gender,dt.Learner_mail,dt.GPA,dt.Address INTO 
v_name,v_semester,v_course,v_dob,v_gender,v_learner,v_gpa,v_address FROM Student s JOIN Details dt ON s.Reg_no = dt.Reg_no WHERE s.Reg_no = p_regno;

    DBMS_OUTPUT.PUT_LINE('--- STUDENT DETAILS ---');
    DBMS_OUTPUT.PUT_LINE('Reg No      : ' || p_regno);
    DBMS_OUTPUT.PUT_LINE('Name        : ' || v_name);
    DBMS_OUTPUT.PUT_LINE('Semester    : ' || v_semester);
    DBMS_OUTPUT.PUT_LINE('Course ID   : ' || v_course);
    DBMS_OUTPUT.PUT_LINE('DOB         : ' || v_dob);
    DBMS_OUTPUT.PUT_LINE('Gender      : ' || v_gender);
    DBMS_OUTPUT.PUT_LINE('Learner     : ' || v_learner);
    DBMS_OUTPUT.PUT_LINE('GPA         : ' || v_gpa);
    DBMS_OUTPUT.PUT_LINE('Address     : ' || v_address);
EXCEPTION
    WHEN NO_DATA_FOUND THEN
        DBMS_OUTPUT.PUT_LINE('No student found with Reg_no: ' || p_regno);
    WHEN OTHERS THEN
        DBMS_OUTPUT.PUT_LINE('Error: ' || SQLERRM);
END;
/

-- DISPLAY FACULTY DETAILS
CREATE OR REPLACE PROCEDURE show_faculty (p_faculty_id IN Faculty.Faculty_ID%TYPE) IS
    v_name     Faculty.Full_Name%TYPE;
    v_phone    Faculty.Faculty_Mobile%TYPE;
    v_email    Faculty.Faculty_Mail%TYPE;
    v_dept     Faculty.Dept_ID%TYPE;
    v_room     Faculty.Faculty_cabin%TYPE;
BEGIN
    SELECT Full_Name, Faculty_Mobile, Faculty_Mail, Dept_ID, Faculty_cabin
    INTO v_name, v_phone, v_email, v_dept, v_room FROM Faculty WHERE Faculty_ID = p_faculty_id;

    DBMS_OUTPUT.PUT_LINE('--- FACULTY DETAILS ---');
    DBMS_OUTPUT.PUT_LINE('Faculty ID  : ' || p_faculty_id);
    DBMS_OUTPUT.PUT_LINE('Name         : ' || v_name);
    DBMS_OUTPUT.PUT_LINE('Phone        : ' || v_phone);
    DBMS_OUTPUT.PUT_LINE('Email        : ' || v_email);
    DBMS_OUTPUT.PUT_LINE('Department   : ' || v_dept);
    DBMS_OUTPUT.PUT_LINE('Room No      : ' || v_room);
EXCEPTION
    WHEN NO_DATA_FOUND THEN
        DBMS_OUTPUT.PUT_LINE('No faculty found with ID ' || p_faculty_id);
END;
/

-- DISPLAY COURSE DETAILS
CREATE OR REPLACE PROCEDURE show_course (p_course_id IN Course.Course_ID%TYPE) IS
    v_name   Course.Course_Name%TYPE;
    v_dept   Course.Dept_ID%TYPE;
BEGIN
    SELECT Course_Name, Dept_ID INTO v_name, v_dept FROM Course WHERE Course_ID = p_course_id;
    DBMS_OUTPUT.PUT_LINE('--- COURSE DETAILS ---');
    DBMS_OUTPUT.PUT_LINE('Course ID   : ' || p_course_id);
    DBMS_OUTPUT.PUT_LINE('Course Name : ' || v_name);
    DBMS_OUTPUT.PUT_LINE('Department  : ' || v_dept);
EXCEPTION
    WHEN NO_DATA_FOUND THEN
        DBMS_OUTPUT.PUT_LINE('No course found with ID ' || p_course_id);
END;
/

-- DISPLAY DEPARTMENT DETAILS
CREATE OR REPLACE PROCEDURE show_department (p_dept_id IN Department.Dept_ID%TYPE)
IS
    v_name     Department.Dept_Name%TYPE;
    v_head     Department.HOD_ID%TYPE;
    v_year     Department.Year_of_Establishment%TYPE;
    v_block    Department.Dept_Address%TYPE;
BEGIN
    SELECT Dept_Name, HOD_ID, Year_of_Establishment, Dept_Address INTO v_name, v_head, v_year, v_block FROM Department WHERE Dept_ID = p_dept_id;
    DBMS_OUTPUT.PUT_LINE('--- DEPARTMENT DETAILS ---');
    DBMS_OUTPUT.PUT_LINE('Department ID   : ' || p_dept_id);
    DBMS_OUTPUT.PUT_LINE('Name            : ' || v_name);
    DBMS_OUTPUT.PUT_LINE('HOD ID          : ' || v_head);
    DBMS_OUTPUT.PUT_LINE('Established Year: ' || v_year);
    DBMS_OUTPUT.PUT_LINE('Block           : ' || v_block);
EXCEPTION
    WHEN NO_DATA_FOUND THEN
        DBMS_OUTPUT.PUT_LINE('No department found with ID ' || p_dept_id);
END;
/



DECLARE
BEGIN
DBMS_OUTPUT.PUT_LINE('1hello');
show_department('CSE');
DBMS_OUTPUT.PUT_LINE('2hello');
show_course(101);
DBMS_OUTPUT.PUT_LINE('hello');
show_faculty(101);
DBMS_OUTPUT.PUT_LINE('4hello');
show_student(22002);
DBMS_OUTPUT.PUT_LINE('5hello');
END;
/

*/

-- DISPLAY ALL FIVE TABLES
/*
SELECT * FROM Student;
SELECT * FROM Faculty;
SELECT * FROM Course;
SELECT * FROM Department;
SELECT * FROM Details;
SELECT * FROM history_details;
*/
DROP TABLE Details;
DROP TABLE Student;
DROP TABLE Course;
ALTER TABLE Department DROP CONSTRAINT dept_fk_hod;
DROP TABLE Faculty;
DROP TABLE Department;
DROP TABLE History_details;

