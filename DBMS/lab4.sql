SET AUTOCOMMIT OFF;

CREATE TABLE Account(
Account_no varchar(20) ,
Branch_name varchar(15),
Balance int 
);

CREATE TABLE Branch(
Branch_name varchar(15), 
Branch_city varchar(15), 
Assets int
);

CREATE TABLE Customer(
Customer_name varchar(20), 
Customer_street varchar(20), 
Customer_city varchar(20) 
);

CREATE TABLE Loan(
Loan_number varchar(20), 
Branch_name varchar(15),
Amount int 
);

CREATE TABLE Depositor(
Customer_name varchar(20),
Account_no varchar(20)
);

CREATE TABLE Borrower(
Customer_name varchar(20), 
Loan_number varchar(20)
);

INSERT INTO Account values('A-101','Downtown',500);
INSERT INTO Account values('A-102','Perryridge',400);
INSERT INTO Account values('A-201','Brighton',900);
INSERT INTO Account values('A-215','Mianus',700);
INSERT INTO Account values('A-305','Round Hill',350);

INSERT INTO Branch values('Downtown','Brooklyn' ,900000 );
INSERT INTO Branch values('Perryridge','Horseneck' ,1700000  );
INSERT INTO Branch values('Brighton','Brooklyn' ,7100000 );
INSERT INTO Branch values('Mianus','Horseneck' ,400000 );
INSERT INTO Branch values('Round Hill','Horseneck' ,8000000 );

INSERT INTO Customer values('Jones', 'Main' ,'Harrison');
INSERT INTO Customer values('Smith', 'North', 'Rye' );
INSERT INTO Customer values('Hayes' ,'Main','Harrison' );
INSERT INTO Customer values('Curry', 'North' ,'Rye' );
INSERT INTO Customer values('Johnson' ,'Alma' ,'Palo Alto' );

INSERT INTO Loan values('L-17','Downtown', 1000 );
INSERT INTO Loan values('L-23','Redwood', 2000  );
INSERT INTO Loan values('L-15','Perryridge', 1500 );
INSERT INTO Loan values('L-14','Downtown', 1500);
INSERT INTO Loan values('L-93','Mianus' ,500 );

INSERT INTO Depositor values('Jones' ,'A-101');
INSERT INTO Depositor values('Smith' ,'A-215');
INSERT INTO Depositor values('Hayes' ,'A-102');
INSERT INTO Depositor values('Curry' ,'A-201');
INSERT INTO Depositor values('Johnson' ,'A-305');

INSERT INTO Borrower values('Jones' ,'L-17');
INSERT INTO Borrower values('Smith' ,'L-23');
INSERT INTO Borrower values('Hayes' ,'L-15');
INSERT INTO Borrower values('Curry' ,'L-14');
INSERT INTO Borrower values('Johnson' ,'L-93');

SAVEPOINT s1;
-- Q.36
SELECT AVG(Balance) FROM Account GROUP BY Branch_name HAVING Branch_name = 'Perryridge'; 
-- Q.37
SELECT Branch_name,AVG(Balance) FROM Account GROUP BY Branch_name; 
-- Q.38
SELECT DISTINCT Branch_name,COUNT(Customer_name) FROM Depositor JOIN Account ON Depositor.Account_no = Account.Account_no GROUP BY Branch_name;
-- Q.39
SELECT Branch_name,AVG(Balance) FROM Account GROUP BY Branch_name HAVING AVG(Balance) > 1200; 
-- Q.40
SELECT COUNT(Branch_name) FROM Branch;
-- Q.41
SELECT d.Customer_name, AVG(a.Balance) AS avg_balance FROM Depositor d JOIN Customer c ON d.Customer_name = c.Customer_name JOIN Account a ON d.Account_no = a.Account_no GROUP BY d.Customer_name HAVING COUNT(d.Account_no) > 3;
-- Q.42
SELECT * FROM Loan ORDER BY Amount DESC;
SELECT * FROM Loan ORDER BY Amount ASC;
-- Q.43
SELECT Branch_name,AVG(Balance) FROM Account GROUP BY Branch_name HAVING AVG(Balance) > 1200; 
-- Q.44
SELECT MAX(Balance) FROM Account;
-- Q.45
SELECT Account_no,Branch_name, Balance FROM Account WHERE Balance = (SELECT MAX(Balance) FROM Account);
-- Q.46
SELECT Branch_name,Balance FROM Account WHERE Balance >= (SELECT AVG(Balance) FROM Account);
-- Q.47
DELETE FROM Loan WHERE Amount >=1300 AND Amount <= 1500;
ROLLBACK TO SAVEPOINT s1;
-- Q.48
DELETE FROM Account WHERE Branch_name IN (SELECT Branch_name FROM Branch WHERE Branch_city = 'Brooklyn');
ROLLBACK TO SAVEPOINT s1;
-- Q.49
INSERT INTO Account (Account_no, Branch_name, Balance) SELECT Account_no, Branch_name, 200 FROM Account WHERE Branch_name = 'Perryridge';
INSERT INTO Depositor (Customer_name, Account_no) SELECT d.Customer_name, d.Account_no FROM Depositor d JOIN Account a ON a.Account_no = d.Account_no WHERE a.Branch_name = 'Perryridge';
ROLLBACK TO SAVEPOINT s1;
-- Q.50
UPDATE Account SET Balance = Balance * 1.05 WHERE Balance >= 1000;
ROLLBACK TO SAVEPOINT s1;
-- Q.51
UPDATE Account SET Balance = Balance * 1.05 WHERE Balance >= (SELECT AVG(Balance) FROM Account);
ROLLBACK TO SAVEPOINT s1;
--Q.52
UPDATE Account SET Balance = Balance * 
    CASE 
        WHEN Balance > 1000 THEN 1.06
        ELSE 1.05
    END;
ROLLBACK TO SAVEPOINT s1;

DROP TABLE Account;
DROP TABLE Branch;
DROP TABLE Customer;
DROP TABLE Loan;
DROP TABLE Depositor;
DROP TABLE Borrower;
