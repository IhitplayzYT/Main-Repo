CREATE TABLE Instructor(
  ID int,
  Name varchar(20),
  Dept_name varchar(20),
  Salary int
);

CREATE TABLE Salary_Raise(
  ID int,
  Name varchar(20),
  Dept_name varchar(20),
  Salary int
);

CREATE TABLE Employee(
  Emp_code int,
  Ename varchar(20),
  Dept_no int,
  Job varchar(20),
  Salary int
);

CREATE TABLE Employee_Raise(
  Emp_code int,
  Raise_date DATE,
  Raise_amt int
);

INSERT INTO Employee VALUES(201,'Ravi',20,'Manager',40000);
INSERT INTO Employee VALUES(202,'Sita',20,'Clerk',25000);
INSERT INTO Employee VALUES(203,'Mohan',10,'Analyst',30000);
INSERT INTO Employee VALUES(204,'Anjali',20,'Salesman',20000);

INSERT INTO Instructor VALUES(101 ,'Anita' ,'Comp.Sci.', 50000);
INSERT INTO Instructor VALUES(102 ,'Rajesh' ,'Comp.Sci.', 60000);
INSERT INTO Instructor VALUES(103 ,'Meera' ,'Physics', 55000);
INSERT INTO Instructor VALUES(104 ,'Vikram', 'Comp.Sci.', 45000);

-- Q1 
UPDATE Instructor SET Salary = Salary + Salary * 0.05 WHERE Dept_name = 'Comp.Sci.';
INSERT INTO Salary_Raise (ID, Name, Dept_name, Salary) SELECT ID, Name, Dept_name, Salary FROM Instructor WHERE Dept_name = 'Comp.Sci.';
--SELECT * FROM Instructor;
--SELECT * FROM Salary_Raise;
-- Q2
UPDATE Employee SET Salary = Salary + Salary * 0.05  WHERE Dept_no = 20;
INSERT INTO Employee_Raise (Emp_code, Raise_date, Raise_amt) SELECT Emp_code, SYSDATE, Salary * 0.05 FROM Employee WHERE Dept_no = 20;
--SELECT * FROM Employee;
--SELECT * FROM Employee_Raise;
-- Q3.a
DECLARE
  CURSOR C1 IS SELECT * FROM Instructor;
  CURSOR C2 IS SELECT * FROM Employee WHERE Dept_no = 20;

  v_raise INT;
BEGIN
  FOR rec IN C1 LOOP
    IF rec.Dept_name = 'Comp.Sci.' THEN
      UPDATE Instructor
      SET Salary = rec.Salary + rec.Salary * 0.05
      WHERE ID = rec.ID;

      INSERT INTO Salary_Raise
      VALUES (rec.ID, rec.Name, rec.Dept_name, rec.Salary + rec.Salary * 0.05);
    END IF;
  END LOOP;

 -- Q3.b 
  FOR rec IN C2 LOOP
    v_raise := rec.Salary * 0.05;

    UPDATE Employee
    SET Salary = rec.Salary + v_raise
    WHERE Emp_code = rec.Emp_code;

    INSERT INTO Employee_Raise
    VALUES (rec.Emp_code, SYSDATE, v_raise);
  END LOOP;
END;
/

SELECT * FROM Salary_Raise;
SELECT * FROM Employee_Raise;


-- Q4
DELETE FROM Salary_Raise;
DELETE FROM Employee_Raise;
DECLARE
  CURSOR C1 IS
    SELECT * FROM Instructor
    FOR UPDATE OF Salary;

  CURSOR C2 IS
    SELECT * FROM Employee
    WHERE Dept_no = 20
    FOR UPDATE OF Salary;

  v_raise INT;
BEGIN
  -- Q4.a 
  FOR rec IN C1 LOOP
    IF rec.Dept_name = 'Comp.Sci.' THEN
      UPDATE Instructor
      SET Salary = rec.Salary + rec.Salary * 0.05
      WHERE CURRENT OF C1;

      INSERT INTO Salary_Raise
      VALUES (rec.ID, rec.Name, rec.Dept_name, rec.Salary + rec.Salary * 0.05);
    END IF;
  END LOOP;

  -- Q4.b
  FOR rec IN C2 LOOP
    v_raise := rec.Salary * 0.05;

    UPDATE Employee
    SET Salary = rec.Salary + v_raise
    WHERE CURRENT OF C2;

    INSERT INTO Employee_Raise
    VALUES (rec.Emp_code, SYSDATE, v_raise);
  END LOOP;
END;
/
SELECT * FROM Salary_Raise;
SELECT * FROM Employee_Raise;


-- Q5
CREATE TABLE item_transaction(
Itemid INT,
Description VARCHAR(20),
Quantity INT
);

CREATE TABLE item_master (
    Itemid INT PRIMARY KEY,
    Description VARCHAR(20),
    Bal_stock INT
);

INSERT INTO item_master VALUES (101, 'Pen', 50);
INSERT INTO item_master VALUES (102, 'Pencil', 100);

INSERT INTO item_transaction VALUES(101,'Pen',5);
INSERT INTO item_transaction VALUES(102,'Pencil',10);

DECLARE
    CURSOR C3 IS
        SELECT Itemid, Description, Quantity
        FROM item_transaction;

    v_count NUMBER;
BEGIN
    FOR rec IN C3 LOOP
        SELECT COUNT(*) INTO v_count
        FROM item_master
        WHERE Itemid = rec.Itemid;

        IF v_count > 0 THEN
            UPDATE item_master
            SET Bal_stock = Bal_stock - rec.Quantity
            WHERE Itemid = rec.Itemid;
        ELSE
            INSERT INTO item_master (Itemid, Description, Bal_stock)
            VALUES (rec.Itemid, rec.Description, rec.Quantity);
        END IF;
    END LOOP;

    COMMIT;
END;
/

-- Q6
CREATE TABLE Emp(
Emp_no int,
Emp_Name varchar(20),
Sal int
);

INSERT INTO Emp VALUES(101,'Anand',6000);
INSERT INTO Emp VALUES(102,'Mohan',5500);
INSERT INTO Emp VALUES(103,'Ravi',4000);

DECLARE
    v_total_sal NUMBER;
BEGIN

    INSERT INTO Emp VALUES (104, 'Sita', 5000);

    UPDATE Emp SET Sal = Sal + 2000 WHERE Emp_Name = 'Anand';

    UPDATE Emp SET Sal = Sal + 1500 WHERE Emp_Name = 'Mohan';

    SELECT SUM(Sal) INTO v_total_sal FROM Emp;

    IF v_total_sal > 20000 THEN
        UPDATE Emp SET Sal = Sal - 2000 WHERE Emp_Name = 'Anand';
        UPDATE Emp SET Sal = Sal - 1500 WHERE Emp_Name = 'Mohan';
        
        DBMS_OUTPUT.PUT_LINE('Total salary exceeded 20000. Updates undone.');
    ELSE
        DBMS_OUTPUT.PUT_LINE('Salary updates successful.');
    END IF;

    COMMIT;
END;
/


SELECT * FROM item_transaction;
SELECT * FROM item_master;
SELECT * FROM Emp;

DROP TABLE Salary_Raise;
DROP TABLE Instructor;
DROP TABLE Employee_Raise;
DROP TABLE Employee;
DROP TABLE item_transaction;
DROP TABLE item_master;
DROP TABLE Emp;
