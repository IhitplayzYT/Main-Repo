SET SERVEROUTPUT ON;


CREATE TABLE Customer(
Name VARCHAR(50) NOT NULL,
Gender VARCHAR(1) CHECK (Gender IN ('M','F')),
Acc_no VARCHAR(10) PRIMARY KEY,
Branch_ID VARCHAR(10) NOT NULL,
Address VARCHAR(50) NOT NULL
);

CREATE TABLE Account(
Acc_no VARCHAR(10) NOT NULL,
Acc_Type VARCHAR(10) CHECK (Acc_Type IN ('Savings','Current')) NOT NULL,
Balance FLOAT CHECK (Balance > 10000) NOT NULL,
CONSTRAINT fk_constraint FOREIGN KEY (Acc_no) REFERENCES Customer(Acc_no) ON DELETE CASCADE
);

CREATE TABLE Transaction(
Acc_no VARCHAR(10) NOT NULL,
Txn_Amt FLOAT CHECK (Txn_Amt > 0) NOT NULL,
Txn_Info VARCHAR(50),
Txn_Date DATE
);

INSERT INTO Customer VALUES('A','M','A-180','B-34','X road');
INSERT INTO Customer VALUES('B','F','A-209','B-65','Y road');
INSERT INTO Customer VALUES('C','F','A-150','B-39','Z road');
INSERT INTO Customer VALUES('D','M','A-140','B-54','U road');

INSERT INTO Account VALUES('A-140','Savings',20050);
INSERT INTO Account VALUES('A-209','Savings',84832);
INSERT INTO Account VALUES('A-180','Current',74160);
INSERT INTO Account VALUES('A-209','Savings',84832);
INSERT INTO Account VALUES('A-150','Current',37811);
INSERT INTO Account VALUES('A-140','Current',45205);


CREATE OR REPLACE PROCEDURE perform_transaction(
    Accno     IN Account.Acc_no%TYPE,
    Amt       IN Account.Balance%TYPE,
    Txn_Type  IN VARCHAR2
)
IS
    Bal Account.Balance%TYPE;
    Insufficient_Balance EXCEPTION;
BEGIN
    SELECT Balance INTO Bal FROM Account WHERE Acc_no = Accno;
    IF Txn_Type = 'WITHDRAW' AND Bal - Amt < 10000 THEN
        RAISE Insufficient_Balance;
    ELSE
        INSERT INTO Transaction VALUES (Accno, Amt, Txn_Type, SYSDATE);
        IF Txn_Type = 'WITHDRAW' THEN
            UPDATE Account SET Balance = Balance - Amt WHERE Acc_no = Accno;
        ELSE
            UPDATE Account SET Balance = Balance + Amt WHERE Acc_no = Accno;
        END IF;
        DBMS_OUTPUT.PUT_LINE('Transaction Successful');
    END IF;

EXCEPTION
    WHEN Insufficient_Balance THEN
        DBMS_OUTPUT.PUT_LINE('Insufficient Balance');
    WHEN NO_DATA_FOUND THEN
        DBMS_OUTPUT.PUT_LINE('Account not found');
    WHEN OTHERS THEN
        DBMS_OUTPUT.PUT_LINE('Unexpected error: ' || SQLERRM);
END;
/


CREATE OR REPLACE TRIGGER Service_charge_trigger
AFTER UPDATE OF Balance ON Account
FOR EACH ROW
BEGIN
  IF ROUND(:OLD.Balance - :NEW.Balance,2) >= 10000 THEN
    INSERT INTO Transaction (Acc_no, Txn_Amt, Txn_Info, Txn_Date)
    VALUES (:OLD.Acc_no, 50, 'Service Charge', SYSDATE);
  END IF;
END;
/

CREATE OR REPLACE FUNCTION calculate_interest(Accno Account.Acc_no%TYPE,rate FLOAT,Duration INT) RETURN FLOAT IS
Val Account.Balance%TYPE;
after_interest Account.Balance%TYPE; 
BEGIN
SELECT Balance INTO Val FROM Account WHERE Acc_no = Accno;
after_interest := (Val * rate * Duration)/100;  
RETURN after_interest;
END;
/


CREATE OR REPLACE FUNCTION calculate_interest(
    Accno    IN Account.Acc_no%TYPE,
    rate     IN FLOAT,
    Duration IN INT
) 
RETURN FLOAT
IS
    Val            Account.Balance%TYPE;
    after_interest FLOAT;
BEGIN
    SELECT Balance INTO Val FROM Account WHERE Acc_no = Accno;
    after_interest := (Val * rate * Duration) / 100;
    RETURN after_interest;
EXCEPTION
    WHEN NO_DATA_FOUND THEN
        RAISE_APPLICATION_ERROR(-20001, 'Account number not found.');
    WHEN OTHERS THEN
        RAISE_APPLICATION_ERROR(-20002, 'Unexpected error: ' || SQLERRM);
END;
/




CREATE OR REPLACE PROCEDURE customer_statement(num Account.Acc_no%TYPE) IS
CURSOR C1 IS SELECT * FROM Transaction  WHERE Acc_no = num;
BEGIN
for x in C1 LOOP
DBMS_OUTPUT.PUT_LINE('Accno : ' || x.Acc_no);
DBMS_OUTPUT.PUT_LINE('Txn Type : ' || x.Txn_Info);
DBMS_OUTPUT.PUT_LINE('Txn Date : ' || TO_CHAR(x.Txn_Date));
DBMS_OUTPUT.PUT_LINE('Txn Amount : ' || x.Txn_Amt);
DBMS_OUTPUT.PUT_LINE('---------------');
END LOOP;
END;
/

CREATE OR REPLACE TRIGGER trg_transfer_limit BEFORE INSERT ON Transaction FOR EACH ROW
DECLARE
varib Account.Balance%TYPE;
last_txn_date Transaction.Txn_Date%TYPE;
BEGIN
 SELECT MAX(Txn_Date) INTO last_txn_date FROM Transaction WHERE Acc_no = :NEW.Acc_no;
 IF (SYSDATE - last_txn_date) > 90 THEN
            RAISE_APPLICATION_ERROR(-20001,'Transaction blocked: Account inactive (no activity in last 90 days).');
 END IF;

SELECT SUM(Txn_Amt) INTO varib FROM Transaction WHERE Acc_no = :NEW.Acc_no AND TRUNC(Txn_Date) = TRUNC(SYSDATE);
IF varib + :NEW.Txn_Amt > 50000 THEN 
    RAISE_APPLICATION_ERROR(-20002,'Transaction blocked: Daily transfer limit of ₹50,000 exceeded.');
END IF;
END;
/


DECLARE
val FLOAT;
BEGIN
perform_transaction('A-150',20000,'WITHDRAW');
val := calculate_interest('A-150',30,5);
DBMS_OUTPUT.PUT_LINE('Amt after Interest = ' || val);
DBMS_OUTPUT.PUT_LINE('---------------');
customer_statement('A-150');
END;
/

SELECT * FROM Account;
SELECT * FROM Transaction

DROP TABLE Account;
DROP TABLE Transaction;
DROP TABLE Customer;
