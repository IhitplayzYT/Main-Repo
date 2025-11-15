
DROP TABLE Areas;
DROP TABLE Student;
DROP TABLE Account;

CREATE TABLE Student(
    Rollno INT PRIMARY KEY,
    Name VARCHAR2(20),
    GPA FLOAT,
    Marks INT
);

CREATE TABLE Account(
    Accno INT PRIMARY KEY,
    Balance INT
);

CREATE TABLE Areas(
    Radius INT,
    Area FLOAT
);

-- Insert Data
INSERT INTO Student VALUES(1, 'Kiran', 5.8, 85);
INSERT INTO Student VALUES(2, 'Anusha', 6.5, 90);
INSERT INTO Student VALUES(3, 'Rohit', 3.4, 78);
INSERT INTO Student VALUES(4, 'Sita', 7.8, 88);
INSERT INTO Student VALUES(5, 'Ajay', 9.5, 92);

INSERT INTO Account VALUES(1, 5000);
INSERT INTO Account VALUES(2, 3000);
INSERT INTO Account VALUES(3, 0);

-- Enable DBMS_OUTPUT
SET SERVEROUTPUT ON;

-- Q1: Display GPA of a given student
DECLARE
    v_rollno Student.Rollno%TYPE;
    v_gpa    Student.GPA%TYPE;
BEGIN
    v_rollno := &Enter_RollNo;  
    SELECT GPA INTO v_gpa
    FROM Student
    WHERE RollNo = v_rollno;
    
    DBMS_OUTPUT.PUT_LINE('GPA of student ' || v_rollno || ' is ' || v_gpa);
EXCEPTION
    WHEN NO_DATA_FOUND THEN
        DBMS_OUTPUT.PUT_LINE('No student found with RollNo ' || v_rollno);
END;
/

-- Q2: Debit Rs.2000 if minimum balance >=500
DECLARE
    v_accno Account.Accno%TYPE := &Enter_AccNo;
    v_balance Account.Balance%TYPE;
BEGIN
    SELECT Balance INTO v_balance
    FROM Account
    WHERE Accno = v_accno;
    
    IF v_balance - 2000 >= 500 THEN
        UPDATE Account
        SET Balance = Balance - 2000
        WHERE Accno = v_accno;
        DBMS_OUTPUT.PUT_LINE('Amount debited successfully. New Balance: ' || (v_balance - 2000));
    ELSE
        DBMS_OUTPUT.PUT_LINE('Minimum balance requirement not met. Debit failed.');
    END IF;
EXCEPTION
    WHEN NO_DATA_FOUND THEN
        DBMS_OUTPUT.PUT_LINE('Account not found!');
END;
/

-- Q3: Calculate book fine
DECLARE
    late_days   NUMBER;
    fine        NUMBER := 0;
BEGIN
    late_days := &days_borrowed_for;
    
    IF late_days <= 7 THEN
        fine := 0;
    ELSIF late_days BETWEEN 8 AND 15 THEN
        fine := (late_days) * 1;
    ELSIF late_days BETWEEN 16 AND 30 THEN
        fine := (late_days) * 2;
    ELSE
        fine := (late_days) * 5;
    END IF;
    
    DBMS_OUTPUT.PUT_LINE('Late days: ' || late_days || ', Fine: Rs.' || fine);
END;
/

-- Q4: Print letter grade of all students using LOOP
DECLARE
    v_rollno Student.Rollno%TYPE := 1;
    v_gpa    Student.GPA%TYPE;
BEGIN
    LOOP
        EXIT WHEN v_rollno > 5;
        SELECT GPA INTO v_gpa
        FROM Student
        WHERE RollNo = v_rollno;
        
        IF v_gpa >= 9 THEN
            DBMS_OUTPUT.PUT_LINE('RollNo: '||v_rollno||' Grade: A+');
        ELSIF v_gpa >= 8 THEN
            DBMS_OUTPUT.PUT_LINE('RollNo: '||v_rollno||' Grade: A');
        ELSIF v_gpa >= 7 THEN
            DBMS_OUTPUT.PUT_LINE('RollNo: '||v_rollno||' Grade: B');
        ELSE
            DBMS_OUTPUT.PUT_LINE('RollNo: '||v_rollno||' Grade: C');
        END IF;
        
        v_rollno := v_rollno + 1;
    END LOOP;
END;
/

-- Q5: Calculate area of circles (radius 3 to 7) using WHILE
DECLARE
    v_radius NUMBER := 3;
    v_area   NUMBER;
BEGIN
    WHILE v_radius <= 7 LOOP
        v_area := 3.14159 * v_radius * v_radius;
        INSERT INTO Areas(Radius, Area) VALUES(v_radius, v_area);
        DBMS_OUTPUT.PUT_LINE('Radius: '||v_radius||', Area: '||v_area);
        v_radius := v_radius + 1;
    END LOOP;
END;
/

-- Q7: Handle DUP_VAL_ON_INDEX exception while inserting student
DECLARE
BEGIN
    INSERT INTO Student(Rollno, Name, Marks, GPA) VALUES(1, 'New Student', 75, 6.0);
EXCEPTION
    WHEN DUP_VAL_ON_INDEX THEN
        DBMS_OUTPUT.PUT_LINE('Error: RollNo 1 already exists!');
END;
/

-- Q8: Fetch balance of account 3 and handle NO_DATA_FOUND
DECLARE
    v_balance Account.Balance%TYPE;
BEGIN
    SELECT Balance INTO v_balance
    FROM Account
    WHERE AccNo = 3;
    
    DBMS_OUTPUT.PUT_LINE('Balance: ' || v_balance);
EXCEPTION
    WHEN NO_DATA_FOUND THEN
        DBMS_OUTPUT.PUT_LINE('Account not found!');
END;
/

-- Q10: Select name of student with marks > 70
DECLARE
    v_name Student.Name%TYPE;
BEGIN
    SELECT Name INTO v_name
    FROM Student
    WHERE Marks > 70 AND ROWNUM = 1;  -- ensures only one row
    DBMS_OUTPUT.PUT_LINE('Student name: ' || v_name);
END;
/

-- Q11: Zero divide exception
DECLARE
    a NUMBER := 10;
    b NUMBER := 0;
    c NUMBER;
BEGIN
    c := a / b;
    DBMS_OUTPUT.PUT_LINE('Result: ' || c);
EXCEPTION
    WHEN ZERO_DIVIDE THEN
        DBMS_OUTPUT.PUT_LINE('Error: Division by zero!');
END;
/

-- Q12: Insert student with invalid marks and raise user-defined exception
DECLARE
    v_rollno Student.Rollno%TYPE := 6;
    v_name   Student.Name%TYPE := 'Ajay';
    v_marks  Student.Marks%TYPE := &Enter_Marks;
    
    invalid_marks EXCEPTION;
BEGIN
    IF v_marks < 0 OR v_marks > 100 THEN
        RAISE invalid_marks;
    END IF;
    
    INSERT INTO Student(Rollno, Name, Marks) VALUES(v_rollno, v_name, v_marks);
    DBMS_OUTPUT.PUT_LINE('Student inserted successfully.');
EXCEPTION
    WHEN invalid_marks THEN
        DBMS_OUTPUT.PUT_LINE('Error: Marks should be between 0 and 100.');
END;
/

-- Q13: Withdraw from account with min_balance_exceeded exception
DECLARE
    v_accno Account.Accno%TYPE := &Enter_AccNo;
    v_withdraw NUMBER := &Enter_Amount;
    v_balance Account.Balance%TYPE;
    
    min_balance_exceeded EXCEPTION;
BEGIN
    SELECT Balance INTO v_balance
    FROM Account
    WHERE Accno = v_accno;
    
    IF v_balance - v_withdraw < 500 THEN
        RAISE min_balance_exceeded;
    ELSE
        UPDATE Account
        SET Balance = Balance - v_withdraw
        WHERE Accno = v_accno;
        DBMS_OUTPUT.PUT_LINE('Amount withdrawn. New balance: ' || (v_balance - v_withdraw));
    END IF;
EXCEPTION
    WHEN min_balance_exceeded THEN
        DBMS_OUTPUT.PUT_LINE('Error: Minimum balance of 500 would be violated!');
    WHEN NO_DATA_FOUND THEN
        DBMS_OUTPUT.PUT_LINE('Account not found!');
END;
/

-- Q14: Search for student by RollNo and raise student_not_found exception
DECLARE
    v_rollno Student.Rollno%TYPE := &Enter_RollNo;
    v_name   Student.Name%TYPE;
    
    student_not_found EXCEPTION;
BEGIN
    BEGIN
        SELECT Name INTO v_name
        FROM Student
        WHERE Rollno = v_rollno;
    EXCEPTION
        WHEN NO_DATA_FOUND THEN
            RAISE student_not_found;
    END;
    
    DBMS_OUTPUT.PUT_LINE('Student found: ' || v_name);
EXCEPTION
    WHEN student_not_found THEN
        DBMS_OUTPUT.PUT_LINE('Error: Student with RollNo ' || v_rollno || ' not found!');
END;
/

-- Drop Tables
DROP TABLE Areas;
DROP TABLE Student;
DROP TABLE Account;
