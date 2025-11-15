CREATE TABLE Salary(
empid INT,
ename VARCHAR(20),
sal INT
);

CREATE TABLE Salary_audit(
empid INT,
ename VARCHAR(20),
old_salary INT,
new_salary INT,
modified_on DATE
);

CREATE TABLE ITMMAST(
itemno INT,
itemname VARCHAR(20),
itmstock INT);

CREATE TABLE TRANSACTION(
transid INT,
itemid INT,
quantity INT);

CREATE TABLE ACCOUNT (
    AccNo    NUMBER PRIMARY KEY,
    AccName  VARCHAR2(50),
    Balance  NUMBER
);

CREATE TABLE TRANSACTION_T (
    TransID   NUMBER PRIMARY KEY,
    AccNo     NUMBER REFERENCES ACCOUNT(AccNo),
    TransType VARCHAR2(10),  
    Amount    NUMBER
);

CREATE TABLE Branch (
    Branch_ID   NUMBER PRIMARY KEY,
    Branch_Name VARCHAR2(50),
    Location    VARCHAR2(50)
);

CREATE TABLE Customer (
    Cust_ID   NUMBER PRIMARY KEY,
    Cust_Name VARCHAR2(50),
    Branch_ID NUMBER REFERENCES Branch(Branch_ID)
);


-- Q1
CREATE OR REPLACE TRIGGER trg_salary_audit
AFTER UPDATE OF sal ON Salary
FOR EACH ROW
BEGIN
    INSERT INTO Salary_audit (empid, ename, old_salary, new_salary, modified_on)
    VALUES (:OLD.empid, :OLD.ename, :OLD.sal, :NEW.sal, SYSDATE);
END;
/

-- Q2
CREATE OR REPLACE TRIGGER trg_update_stock
AFTER INSERT ON TRANSACTION
FOR EACH ROW
BEGIN
    UPDATE ITMMAST
    SET ITMStock = ITMStock - :NEW.Quantity
    WHERE ItemNo = :NEW.ItemID;
END;
/

-- Q3
CREATE OR REPLACE TRIGGER trg_check_transaction
BEFORE INSERT ON TRANSACTION_T
FOR EACH ROW
DECLARE
    v_balance ACCOUNT.Balance%TYPE;
BEGIN
    SELECT Balance INTO v_balance
    FROM ACCOUNT
    WHERE AccNo = :NEW.AccNo;

    IF :NEW.Amount <= 0 THEN
        RAISE_APPLICATION_ERROR(-20001, 'Transaction amount must be greater than zero');
    END IF;

    IF UPPER(:NEW.TransType) = 'WITHDRAW' THEN
        IF :NEW.Amount > v_balance THEN
            RAISE_APPLICATION_ERROR(-20002, 'Withdrawal amount exceeds account balance');
        END IF;
    END IF;

EXCEPTION
    WHEN NO_DATA_FOUND THEN
        RAISE_APPLICATION_ERROR(-20003, 'Account number does not exist');
END;
/

-- Q4
CREATE OR REPLACE VIEW All_Customers AS
SELECT b.Branch_ID, b.Branch_Name, b.Location,c.Cust_ID, c.Cust_Name FROM Branch b LEFT JOIN Customer c ON b.Branch_ID = c.Branch_ID;

CREATE OR REPLACE TRIGGER trg_delete_branch_customers
INSTEAD OF DELETE ON All_Customers
FOR EACH ROW
BEGIN
    DELETE FROM Customer
    WHERE Branch_ID = :OLD.Branch_ID;
    DELETE FROM Branch
    WHERE Branch_ID = :OLD.Branch_ID;
END;
/


-- Q1
INSERT INTO Salary VALUES (101, 'Amit', 50000);
INSERT INTO Salary VALUES (102, 'Neha', 60000);
UPDATE Salary SET sal = 55000 WHERE EmpID = 101;
UPDATE Salary SET sal = 65000 WHERE EmpID = 102;
SELECT * FROM Salary_Audit;


-- Q2
INSERT INTO ITMMAST VALUES (1, 'Laptop', 50);
INSERT INTO ITMMAST VALUES (2, 'Mouse', 100);
INSERT INTO TRANSACTION VALUES (1001, 1, 5);
INSERT INTO TRANSACTION VALUES (1002, 2, 10);
SELECT * FROM ITMMAST;

-- Q3
INSERT INTO ACCOUNT VALUES (101, 'Amit', 5000);
INSERT INTO ACCOUNT VALUES (102, 'Neha', 2000);
INSERT INTO TRANSACTION_T VALUES (1, 101, 'DEPOSIT', 1000);
INSERT INTO TRANSACTION_T VALUES (2, 999, 'DEPOSIT', 500);
INSERT INTO TRANSACTION_T VALUES (3, 101, 'DEPOSIT', 0);
INSERT INTO TRANSACTION_T VALUES (4, 102, 'WITHDRAW', 3000);

-- Q4
INSERT INTO Branch VALUES (1, 'Main Branch', 'Mumbai');
INSERT INTO Branch VALUES (2, 'West Branch', 'Pune');
INSERT INTO Customer VALUES (101, 'Amit', 1);
INSERT INTO Customer VALUES (102, 'Neha', 1);
INSERT INTO Customer VALUES (103, 'Rohit', 2);
DELETE FROM All_Customers WHERE Branch_ID = 1;
SELECT * FROM Branch;
SELECT * FROM Customer;

DROP TABLE Salary;
DROP TABLE Salary_audit;
DROP TABLE ITMMAST;
DROP TABLE TRANSACTION;
DROP TABLE TRANSACTION_T;
DROP TABLE Account;
DROP TABLE Customer;
DROP TABLE Branch;

