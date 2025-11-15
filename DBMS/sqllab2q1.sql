CREATE TABLE Department(
Depno int PRIMARY KEY,
Depname varchar(20) UNIQUE NOT NULL,
Address varchar(30)
);

CREATE TABLE Employee(
Empno int PRIMARY KEY,
Empname varchar(20) NOT NULL,
Gender varchar(1) NOT NULL CHECK (Gender in ('M','F')),
Salary float NOT NULL,
Address varchar(30) NOT NULL,
Dno int UNIQUE NOT NULL,
CONSTRAINT fk_constraint FOREIGN KEY (Dno) REFERENCES Department(Depno) 
);

DESC Department;
DESC Employee;

INSERT INTO Department values(1, 'HR', 'Chennai');
INSERT INTO Department values(2 ,'Finance' ,'Mumbai' );
INSERT INTO Department values(3,'IT','Bangalore');
INSERT INTO Department values(4,'Marketing','Delhi');
INSERT INTO Department values(5,'Operations','Hyderabad');

INSERT INTO Employee values(101,'Alice Johnson','F',55000.00 ,'123 Park Street, Chennai ',1);
INSERT INTO Employee values(102,'Ravi Kumar', 'M', 60000.00 ,'45 MG Road, Mumbai', 2 );
INSERT INTO Employee values(103,'Divya Menon', 'F' ,75000.00 ,'8 IT Hub, Bangalore' ,3 );
INSERT INTO Employee values(104,'Arjun Mehta','M', 50000.00 ,'22 Ring Road, Delhi' ,4 );
INSERT INTO Employee values(105,'Sneha Rao', 'F' ,58000.00 ,'56 Industrial Area, Hyderabad' ,5 );


SELECT * FROM Employee;
SELECT * FROM Department;

-- VIOLATE INTEGRITY CONSTRAINTS
INSERT INTO Department values('','Operations','Hyderabad');
-- VIOLATE INTEGRITY CONSTRAINTS
INSERT INTO Employee values(106,'Alc son','X',59000.00 ,'1244444 Street, Chennai ',4);
-- VIOLATE INTEGRITY CONSTRAINTS
INSERT INTO Employee values(106,'Alc son','F',59000.00 ,'1244444 Street, Chennai ',8);
DELETE FROM Department WHERE Depno = 1;
-- VIOLATE INTEGRITY CONSTRAINTS
ALTER TABLE Employee DROP CONSTRAINT fk_constraint;
ALTER TABLE Employee ADD CONSTRAINT fk_new_constraint FOREIGN KEY (Dno) REFERENCES Department(Depno) ON DELETE CASCADE;
DELETE FROM Department WHERE Depno = 1;

SELECT * FROM Employee;
SELECT * FROM Department;

-- Create a named constraint to set the default salary to 10000 and test the constraint  
ALTER TABLE Employee MODIFY Salary DEFAULT 10000;
INSERT INTO Department values(7,'SQL','NOIDA');
INSERT INTO Employee(Empno, Empname, Gender, Address, Dno) values(107,'Ace Json','F','982033 Street, NOIDA ',7);
SELECT * FROM Employee;
SELECT * FROM Department;

DROP TABLE Employee;
DROP TABLE Department;
