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
Account_number varchar(20)
);

CREATE TABLE Borrower(
customer_name varchar(20), 
loan_number varchar(20)
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
INSERT INTO Customer values('Johnson' ,'Alma Palo' ,'Alto' );

INSERT INTO Loan values('L-17','Downtown', 1000 );
INSERT INTO Loan values('L-23','Perryridge', 2000  );
INSERT INTO Loan values('L-15','Brighton', 1500 );
INSERT INTO Loan values('L-14','Mianus', 1500);
INSERT INTO Loan values('L-93','Round Hill' ,500 );

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

--
SELECT Branch_name,Assets FROM Branch;
--
SELECT Account.Account_no,Branch.Branch_city FROM Account,Branch WHERE Branch.Branch_name = Account.Branch_name AND Branch.Branch_city = 'Brooklyn';
--
SELECT Loan.loan_number FROM Loan WHERE Loan.Amount > 1000;
--
SELECT Account_no FROM Account WHERE Account.Branch_name = 'Perryridge' AND Account.Balance < 1000;
--
SELECT Account_no FROM Account WHERE Account.Balance >= 700 AND Account.Balance <= 900;
--


UPDATE Branch SET Assets=340000000 WHERE Branch.Branch_name='Perryridge';
--
UPDATE Account SET Branch_name = 'Downtown' WHERE Branch_name = 'Perryridge';
--
UPDATE Loan SET Amount = Amount + (SELECT Amount FROM Loan WHERE Loan.Branch_name = 'Perryridge') WHERE Loan.Branch_name = 'Downtown';
--
UPDATE Account SET Balance = Balance + 100 WHERE Account.Account_no = 'A-215';
UPDATE Account SET Balance = Balance - 100 WHERE Account.Account_no = 'A-101';
--
DELETE FROM Loan WHERE Branch_name = 'Perryridge';
--
DELETE FROM Loan WHERE Amount < 1000;
--
DELETE FROM Loan WHERE Branch_name = 'Downtown';
DELETE FROM Branch WHERE Branch_name = 'Downtown';
DELETE FROM Account WHERE Branch_name = 'Downtown';
--
--
ALTER table Customer ADD (Phone_no varchar(10));
--
ALTER table Branch MODIFY(Branch_city varchar(20));
--
ALTER table Customer DROP COLUMN Phone_no;

SELECT * FROM Loan;
SELECT * FROM Depositor;
--
SELECT Borrower.Customer_name,Loan.Loan_number,Loan.Amount FROM Borrower JOIN Loan ON Borrower.Loan_number = Loan.Loan_number;
--
SELECT Borrower.Customer_name,Loan.Loan_number,Loan.Amount FROM Borrower JOIN Loan ON Borrower.Loan_number = Loan.Loan_number WHERE Loan.Branch_name = 'Perryridge';

SELECT Customer_name,Loan_number AS Loan_Id FROM Borrower;
--
SELECT Branch_name FROM Branch WHERE Branch.Assets > 900000;
--
SELECT customer_name FROM customer WHERE Customer_street LIKE '%Main%';

--SELECT * FROM Account;
--SELECT * FROM Branch;
--SELECT * FROM Customer;
--SELECT * FROM Loan;
--SELECT * FROM Depositor;
--SELECT * FROM Borrower;

DROP TABLE Account;
DROP TABLE Branch;
DROP TABLE Customer;
DROP TABLE Loan;
DROP TABLE Depositor;
DROP TABLE Borrower;


