SET SERVEROUTPUT ON;

CREATE TABLE Customer (
    Cust_ID    NUMBER,
    Cust_Name  VARCHAR2(50),
    Address    VARCHAR2(100),
    Phone      VARCHAR2(15),
    Email      VARCHAR2(50),
    DOB        DATE
);

CREATE TABLE Branch (
    Branch_ID   NUMBER,
    Branch_Name VARCHAR2(50),
    Location    VARCHAR2(50)
);

CREATE TABLE Account (
    Acc_No     NUMBER,
    Cust_ID    NUMBER,
    Branch_ID  NUMBER,
    Acc_Type   VARCHAR2(20),
    Balance    NUMBER(12,2),
    Open_Date  DATE
);

CREATE TABLE Loan (
    Loan_ID    NUMBER,
    Cust_ID    NUMBER,
    Branch_ID  NUMBER,
    Loan_Type  VARCHAR2(20),
    Amount     NUMBER(12,2),
    Issue_Date DATE
);

CREATE TABLE Payment (
    Loan_ID      NUMBER,
    Payment_No   NUMBER,
    Payment_Date DATE,
    Amount       NUMBER(12,2)
);

CREATE TABLE Transaction (
    Trans_ID   NUMBER,
    Acc_No     NUMBER,
    Trans_Type VARCHAR2(20),
    Amount     NUMBER(12,2),
    Trans_Date DATE
);

CREATE TABLE Item_Master (
    Item_ID   NUMBER,
    Item_Name VARCHAR2(50),
    Price     NUMBER(10,2)
);

CREATE TABLE Item_Transaction (
    Trans_ID    NUMBER,
    Cust_ID     NUMBER,
    Item_ID     NUMBER,
    Quantity    NUMBER,
    Description VARCHAR2(100)
);


INSERT INTO Customer VALUES (101, 'Alice', 'Bangalore', '9876543210', 'alice@mail.com', TO_DATE('15-MAY-1990','DD-MON-YYYY'));
INSERT INTO Customer VALUES (102, 'Bob', 'Chennai', '9876500001', 'bob@mail.com', TO_DATE('10-JUL-1985','DD-MON-YYYY'));
INSERT INTO Customer VALUES (103, 'Charlie', 'Delhi', '9876500002', 'charlie@mail.com', TO_DATE('25-DEC-1992','DD-MON-YYYY'));

INSERT INTO Branch VALUES (201, 'Main Branch', 'Bangalore');
INSERT INTO Branch VALUES (202, 'City Branch', 'Chennai');

INSERT INTO Account VALUES (1001, 101, 201, 'SAVINGS', 50000, SYSDATE);
INSERT INTO Account VALUES (1002, 102, 201, 'CURRENT', 120000, SYSDATE);
INSERT INTO Account VALUES (1003, 103, 202, 'SAVINGS', 80000, SYSDATE);

INSERT INTO Loan VALUES (301, 101, 201, 'HOME', 500000, SYSDATE);
INSERT INTO Loan VALUES (302, 102, 201, 'CAR', 200000, SYSDATE);
INSERT INTO Loan VALUES (303, 103, 202, 'PERSONAL', 100000, SYSDATE);

INSERT INTO Payment VALUES (301, 1, SYSDATE, 100000);
INSERT INTO Payment VALUES (301, 2, SYSDATE, 150000);
INSERT INTO Payment VALUES (302, 1, SYSDATE, 50000);
INSERT INTO Payment VALUES (303, 1, SYSDATE, 20000);

INSERT INTO Transaction VALUES (401, 1001, 'DEPOSIT', 20000, SYSDATE);
INSERT INTO Transaction VALUES (402, 1002, 'WITHDRAWAL', 10000, SYSDATE);
INSERT INTO Transaction VALUES (403, 1003, 'DEPOSIT', 15000, SYSDATE);

INSERT INTO Item_Master VALUES (501, 'Laptop', 60000);
INSERT INTO Item_Master VALUES (502, 'Mobile', 20000);
INSERT INTO Item_Master VALUES (503, 'Headphones', 3000);

INSERT INTO Item_Transaction VALUES (601, 101, 501, 1, 'Alice buys 1 Laptop');
INSERT INTO Item_Transaction VALUES (602, 101, 503, 2, 'Alice buys 2 Headphones');
INSERT INTO Item_Transaction VALUES (603, 102, 502, 3, 'Bob buys 3 Mobiles');
INSERT INTO Item_Transaction VALUES (604, 103, 503, 1, 'Charlie buys 1 Headphone');


-- Q1
CREATE OR REPLACE PROCEDURE good_day IS 
BEGIN
  DBMS_OUTPUT.PUT_LINE('Good Day to You!');
END;
/
-- Q2.a
CREATE OR REPLACE PROCEDURE Get_Customer_Loan_Info (
    p_CustID   IN  Customer.Cust_ID%TYPE
) AS
    v_LoanAmount   NUMBER := 0;
    v_PaymentTotal NUMBER := 0;
BEGIN
    SELECT SUM(Amount) INTO v_LoanAmount FROM Loan WHERE Cust_ID = p_CustID;
    SELECT SUM(Amount) INTO v_PaymentTotal FROM Payment WHERE Loan_ID IN (SELECT Loan_ID FROM Loan WHERE Cust_ID = p_CustID);
    DBMS_OUTPUT.PUT_LINE('Customer ID: ' || p_CustID);
    DBMS_OUTPUT.PUT_LINE('Total Loan Amount Issued: ' || v_LoanAmount);
    DBMS_OUTPUT.PUT_LINE('Total Payment Done: ' || v_PaymentTotal);
END;
/
-- Q2.b
CREATE OR REPLACE PROCEDURE Get_Branch_Info (
    p_BranchID IN Branch.Branch_ID%TYPE
) AS
    v_TotalDeposit NUMBER := 0;
    v_TotalLoan    NUMBER := 0;
BEGIN
    SELECT SUM(Balance) INTO v_TotalDeposit FROM Account WHERE Branch_ID = p_BranchID;
    SELECT SUM(Amount) INTO v_TotalLoan FROM Loan WHERE Branch_ID = p_BranchID;
    DBMS_OUTPUT.PUT_LINE('Branch ID: ' || p_BranchID);
    DBMS_OUTPUT.PUT_LINE('Total Deposits: ' || v_TotalDeposit);
    DBMS_OUTPUT.PUT_LINE('Total Loans: ' || v_TotalLoan);
END;
/


-- Q3
CREATE OR REPLACE PROCEDURE Get_Customers_By_Branch (
    p_BranchName IN Branch.Branch_Name%TYPE
) AS
BEGIN
    FOR rec IN (
        SELECT DISTINCT c.Cust_Name
        FROM Customer c
        JOIN Account a ON c.Cust_ID = a.Cust_ID
        JOIN Branch b ON a.Branch_ID = b.Branch_ID
        WHERE b.Branch_Name = p_BranchName
    ) LOOP
        DBMS_OUTPUT.PUT_LINE('Customer: ' || rec.Cust_Name);
    END LOOP;
END;
/
-- Q4
CREATE OR REPLACE FUNCTION get_loan_balance(p_loan_id IN NUMBER)
RETURN NUMBER
IS
    v_total_loan    NUMBER := 0;
    v_total_payment NUMBER := 0;
BEGIN
    SELECT SUM(amount) INTO v_total_loan FROM Loan WHERE Loan_ID = p_loan_id;
    SELECT SUM(amount) INTO v_total_payment FROM Payment WHERE Loan_ID = p_loan_id;
    RETURN v_total_loan - v_total_payment;
END;
/

-- Q5
CREATE OR REPLACE FUNCTION GetBranchDepositBalance(p_Branch_ID IN NUMBER)
RETURN NUMBER
IS
    v_TotalDeposit NUMBER := 0;
BEGIN
    SELECT NVL(SUM(Balance), 0)
    INTO v_TotalDeposit
    FROM Account
    WHERE Branch_ID = p_Branch_ID
      AND Acc_Type = 'Deposit'; 

    RETURN v_TotalDeposit;
END;
/


CREATE OR REPLACE PROCEDURE Calc_Interest(
    p_principal IN NUMBER,
    p_rate      IN NUMBER,
    p_years     IN NUMBER,
    p_simple    OUT NUMBER,
    p_compound  OUT NUMBER,
    p_total     IN OUT NUMBER
) IS
BEGIN
    p_simple := (p_principal * p_rate * p_years) / 100;
    p_compound := p_principal * (POWER((1 + (p_rate / 100)), p_years) - 1);
    p_total := p_principal + p_compound;
END;
/

DECLARE
  v_balance NUMBER;
  bal NUMBER;
    v_principal NUMBER := 10000;
    v_rate      NUMBER := 5;
    v_years     NUMBER := 3;
    v_simple    NUMBER;
    v_compound  NUMBER;
    v_total     NUMBER := 0;
BEGIN
  good_day;
  Get_Customer_Loan_Info(101);
  Get_Branch_Info(201);
  Get_Customers_By_Branch('Main Branch');
  v_balance := get_loan_balance(301);
  DBMS_OUTPUT.PUT_LINE('Outstanding Loan Balance for Loan_ID 301: ' || v_balance);
  bal:=GetBranchDepositBalance(201);
  DBMS_OUTPUT.PUT_LINE('Branch deposit balance: ' || v_balance);
   Calc_Interest(v_principal, v_rate, v_years, v_simple, v_compound, v_total);
    DBMS_OUTPUT.PUT_LINE('Principal      : ' || v_principal);
    DBMS_OUTPUT.PUT_LINE('Rate (%)       : ' || v_rate);
    DBMS_OUTPUT.PUT_LINE('Years          : ' || v_years);
    DBMS_OUTPUT.PUT_LINE('Simple Interest: ' || v_simple);
    DBMS_OUTPUT.PUT_LINE('Compound Int.  : ' || v_compound);
    DBMS_OUTPUT.PUT_LINE('Total (P+CI)   : ' || v_total);
END;
/
DROP TABLE Customer;
DROP TABLE Branch;
DROP TABLE Account;
DROP TABLE Loan;
DROP TABLE Payment;
DROP TABLE Transaction;
DROP TABLE Item_Master;
DROP TABLE Item_Transaction;

