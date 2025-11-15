
-- Step 1: Create tables WITHOUT foreign key constraints

DELETE TABLE Dependent;
DELETE TABLE Works_on;
DELETE TABLE Project;
DELETE TABLE Dept_Locations;
DELETE TABLE Employee;
DELETE TABLE Department;

CREATE TABLE Department (
    Dname VARCHAR2(20),
    Dnumber INT PRIMARY KEY,
    MGRSSN INT,
    MGRStartDate VARCHAR2(10)
);

CREATE TABLE Employee (
    Fname VARCHAR2(20),
    Minit CHAR(1),
    Lname VARCHAR2(20),
    SSN INT PRIMARY KEY,
    Bdate VARCHAR2(10),
    Address VARCHAR2(50),
    Sex CHAR(1) CHECK (Sex IN ('M','F')),
    Salary INT,
    SuperSSN INT,
    Dno INT
);

CREATE TABLE Dept_Locations (
    Dnumber INT,
    Dlocation VARCHAR2(20),
    PRIMARY KEY (Dnumber, Dlocation)
);

CREATE TABLE Project (
    Pname VARCHAR2(20),
    Pnumber INT PRIMARY KEY,
    Plocation VARCHAR2(20),
    Dnum INT
);

CREATE TABLE Works_on (
    ESSN INT,
    Pno INT,
    Hours FLOAT,
    PRIMARY KEY (ESSN, Pno)
);

CREATE TABLE Dependent (
    ESSN INT,
    Dependent_name VARCHAR2(20),
    Sex CHAR(1) CHECK (Sex IN ('M','F')),
    Bdate VARCHAR2(10),
    Relationship VARCHAR2(20),
    PRIMARY KEY (ESSN, Dependent_name)
);


-- Departments (insert without FK validation on MGRSSN)
INSERT INTO Department VALUES ('Research', 5, 333445555, '1988-05-22');
INSERT INTO Department VALUES ('Administration', 4, 987654321, '1995-01-01');
INSERT INTO Department VALUES ('Headquarters', 1, 888665555, '1981-06-19');

-- Employees
INSERT INTO Employee VALUES ('John', 'B', 'Smith', 123456789, '1965-01-09', '731 Fondren, Houston, TX', 'M', 30000, 333445555, 5);
INSERT INTO Employee VALUES ('Franklin', 'T', 'Wong', 333445555, '1955-12-08', '638 Voss, Houston, TX', 'M', 40000, 888665555, 5);
INSERT INTO Employee VALUES ('Alicia', 'J', 'Zelaya', 999887777, '1968-07-19', '3321 Castle, Spring, TX', 'F', 25000, 987654321, 4);
INSERT INTO Employee VALUES ('Jennifer', 'S', 'Wallace', 987654321, '1941-06-20', '291 Berry, Bellaire, TX', 'F', 43000, 888665555, 4);
INSERT INTO Employee VALUES ('Ramesh', 'K', 'Narayan', 666884444, '1962-09-15', '975 Fire Oak, Humble, TX', 'M', 38000, 333445555, 5);
INSERT INTO Employee VALUES ('Joyce', 'A', 'English', 453453453, '1972-07-31', '5631 Rice, Houston, TX', 'F', 25000, 333445555, 5);
INSERT INTO Employee VALUES ('Ahmad', 'V', 'Jabbar', 987987987, '1969-03-29', '980 Dallas, Houston, TX', 'M', 25000, 987654321, 4);
INSERT INTO Employee VALUES ('James', 'E', 'Borg', 888665555, '1937-11-10', '450 Stone, Houston, TX', 'M', 55000, NULL, 1);

-- Dept_Locations
INSERT INTO Dept_Locations VALUES (1, 'Houston');
INSERT INTO Dept_Locations VALUES (4, 'Stafford');
INSERT INTO Dept_Locations VALUES (5, 'Bellaire');
INSERT INTO Dept_Locations VALUES (5, 'Sugarland');
INSERT INTO Dept_Locations VALUES (5, 'Houston');

-- Projects
INSERT INTO Project VALUES ('ProductX', 1, 'Bellaire', 5);
INSERT INTO Project VALUES ('ProductY', 2, 'Sugarland', 5);
INSERT INTO Project VALUES ('ProductZ', 3, 'Houston', 5);
INSERT INTO Project VALUES ('Computerization', 10, 'Stafford', 4);
INSERT INTO Project VALUES ('Reorganization', 20, 'Houston', 1);
INSERT INTO Project VALUES ('Newbenefits', 30, 'Stafford', 4);

-- Works_on
INSERT INTO Works_on VALUES (123456789, 1, 32.5);
INSERT INTO Works_on VALUES (123456789, 2, 7.5);
INSERT INTO Works_on VALUES (666884444, 3, 40.0);
INSERT INTO Works_on VALUES (453453453, 2, 20.0);
INSERT INTO Works_on VALUES (453453453, 3, 20.0);
INSERT INTO Works_on VALUES (333445555, 2, 10.0);
INSERT INTO Works_on VALUES (333445555, 3, 10.0);
INSERT INTO Works_on VALUES (333445555, 10, 10.0);
INSERT INTO Works_on VALUES (333445555, 20, 10.0);
INSERT INTO Works_on VALUES (999887777, 10, 10.0);
INSERT INTO Works_on VALUES (987987987, 10, 35.0);
INSERT INTO Works_on VALUES (987987987, 30, 5.0);
INSERT INTO Works_on VALUES (987654321, 30, 20.0);
INSERT INTO Works_on VALUES (987654321, 20, 15.0);
INSERT INTO Works_on VALUES (888665555, 20, NULL);

-- Dependents
INSERT INTO Dependent VALUES (333445555, 'Alice', 'F', '1986-04-05', 'DAUGHTER');
INSERT INTO Dependent VALUES (333445555, 'Theodore', 'M', '1983-10-25', 'SON');
INSERT INTO Dependent VALUES (333445555, 'Joy', 'F', '1958-05-03', 'SPOUSE');
INSERT INTO Dependent VALUES (987654321, 'Abner', 'M', '1942-02-28', 'SPOUSE');
INSERT INTO Dependent VALUES (123456789, 'Michael', 'M', '1988-01-04', 'SON');
INSERT INTO Dependent VALUES (123456789, 'Alice', 'F', '1988-12-30', 'DAUGHTER');
INSERT INTO Dependent VALUES (123456789, 'Elizabeth', 'F', '1967-05-05', 'SPOUSE');


-- Step 3: Add foreign key constraints AFTER inserts

ALTER TABLE Employee
ADD CONSTRAINT fk_emp_dept FOREIGN KEY (Dno) REFERENCES Department(Dnumber) ON DELETE CASCADE;

ALTER TABLE Department
ADD CONSTRAINT fk_mgrssn FOREIGN KEY (MGRSSN) REFERENCES Employee(SSN)
DEFERRABLE INITIALLY DEFERRED;

ALTER TABLE Dept_Locations
ADD CONSTRAINT fk_dept_loc_dept FOREIGN KEY (Dnumber) REFERENCES Department(Dnumber) ON DELETE CASCADE;

ALTER TABLE Project
ADD CONSTRAINT fk_proj_dept FOREIGN KEY (Dnum) REFERENCES Department(Dnumber) ON DELETE CASCADE;

ALTER TABLE Works_on
ADD CONSTRAINT fk_works_emp FOREIGN KEY (ESSN) REFERENCES Employee(SSN) ON DELETE CASCADE;

ALTER TABLE Works_on
ADD CONSTRAINT fk_works_proj FOREIGN KEY (Pno) REFERENCES Project(Pnumber) ON DELETE CASCADE;

ALTER TABLE Dependent
ADD CONSTRAINT fk_dep_emp FOREIGN KEY (ESSN) REFERENCES Employee(SSN) ON DELETE CASCADE;
--Q1
SELECT Bdate,Address FROM Employee WHERE Fname = 'John' and Minit = 'B' and Lname = 'Smith';
SELECT Bdate,Address FROM Employee JOIN Department ON Employee.Dno = Department.Dnumber WHERE Department.Dname = 'Research';
--Q2
SELECT Pnumber,Dnum,Lname,Address,Bdate FROM Employee JOIN Project ON Employee.Dno = Project.Dnum WHERE Plocation = 'Stafford';
--Q3
SELECT e.Fname AS Employee_FirstName, e.Lname AS Employee_LastName, s.Fname AS Supervisor_FirstName, s.Lname AS Supervisor_LastName FROM Employee e JOIN Employee s ON e.SuperSSN = s.SSN;
--Q4
SELECT DISTINCT P.Pnumber FROM Project P JOIN Department D ON P.Dnum = D.Dnumber JOIN Employee E1 ON D.MGRSSN = E1.SSN WHERE E1.Lname = 'Smith' UNION SELECT DISTINCT P.Pnumber FROM Works_on W JOIN Employee E2 ON W.ESSN = E2.SSN JOIN Project P ON W.Pno = P.Pnumber WHERE E2.Lname = 'Smith';
--Q5
SELECT e.Fname, e.Lname, e.Salary AS OLD_SALARY, e.Salary * 1.10 AS NEW_SALARY FROM Employee e JOIN Works_on w ON e.SSN = w.ESSN JOIN Project p ON w.Pno = p.Pnumber WHERE p.Pname = 'ProductX';
--Q6
SELECT d.Dname, e.Lname, e.Fname, p.Pname FROM Employee e JOIN Department d ON e.Dno = d.Dnumber JOIN Works_on w ON e.SSN = w.ESSN JOIN Project p ON w.Pno = p.Pnumber ORDER BY d.Dname, e.Lname, e.Fname;
--Q7
SELECT e.Fname, e.Lname FROM Employee e JOIN Dependent d ON e.SSN = d.ESSN WHERE e.Fname = d.Dependent_name AND e.Sex = d.Sex;
--Q8
SELECT e.Fname, e.Lname FROM Employee e JOIN Dependent d ON e.SSN = d.ESSN WHERE d.ESSN IS NULL;
--Q9
SELECT DISTINCT m.Fname, m.Lname FROM Department d JOIN Employee m ON d.MGRSSN = m.SSN JOIN Dependent dep ON m.SSN = dep.ESSN;
--Q10
SELECT SUM(Salary), MAX(Salary), MIN(Salary), AVG(Salary) FROM Employee;
--Q11
SELECT P.Pnumber, P.Pname, COUNT(W.ESSN) AS Num_Employees FROM Project P JOIN Works_on W ON P.Pnumber = W.Pno GROUP BY P.Pnumber, P.Pname;
--Q12
SELECT p.Pnumber, p.Pname, COUNT(w.ESSN) AS Num_Employees FROM Project p JOIN Works_on w ON p.Pnumber = w.Pno GROUP BY p.Pnumber, p.Pname HAVING COUNT(w.ESSN) > 2;
--Q13
SELECT e.Dno, COUNT(*) FROM Employee e WHERE e.SALARY > 40000 GROUP BY e.Dno HAVING COUNT(*) > 5;

DROP TABLE Dependent CASCADE CONSTRAINTS;
DROP TABLE Works_on CASCADE CONSTRAINTS;
DROP TABLE Project CASCADE CONSTRAINTS;
DROP TABLE Dept_Locations CASCADE CONSTRAINTS;
DROP TABLE Employee CASCADE CONSTRAINTS;
DROP TABLE Department CASCADE CONSTRAINTS;
