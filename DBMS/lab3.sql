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


-- Q.20
SELECT Customer_name FROM Borrower UNION SELECT Customer_name FROM Depositor;
-- Q.21
SELECT Customer_name FROM Borrower INTERSECT SELECT Customer_name FROM Depositor;
-- Q.22
SELECT Customer_name FROM Depositor MINUS SELECT Customer_name FROM Borrower;
-- Q.23
SELECT Loan_number FROM Loan WHERE Loan.Amount is NULL;
-- Q.24
SELECT Customer_name FROM Account JOIN Depositor ON Depositor.Account_no = Account.Account_no WHERE Account.Branch_name = 'Perryridge' INTERSECT SELECT Customer_name FROM Borrower JOIN Loan ON Loan.Loan_number = Borrower.Loan_number WHERE Loan.Branch_name = 'Perryridge'; 
-- Q.25
SELECT Customer_name FROM Borrower JOIN Loan ON Loan.Loan_number = Borrower.Loan_number WHERE Loan.Branch_name = 'Perryridge' MINUS SELECT Customer_name FROM Account JOIN Depositor ON Depositor.Account_no = Account.Account_no WHERE Account.Branch_name = 'Perryridge'; 
-- Q.26
SELECT Customer_name FROM Borrower WHERE Borrower.Customer_name NOT IN('Smith','Jones');
-- Q.27
SELECT Branch_name FROM Branch WHERE Branch.Assets > SOME(SELECT Assets FROM Branch WHERE Branch.Branch_city = 'Brooklyn');
-- Q.28
SELECT Branch_name FROM Branch WHERE Branch.Assets > ALL(SELECT Assets FROM Branch WHERE Branch.Branch_city = 'Brooklyn');
-- Q.29
SELECT Customer_name FROM Borrower B WHERE EXISTS (SELECT Customer_name FROM Depositor D WHERE D.Customer_name = B.Customer_name);
-- Q.30
SELECT Customer_name FROM Depositor d JOIN Account a ON d.Account_no = a.Account_no JOIN Branch b ON a.Branch_name = b.Branch_name WHERE b.Branch_city = 'Brooklyn' GROUP BY d.Customer_name HAVING COUNT(DISTINCT b.Branch_name) = (SELECT COUNT(*) FROM Branch WHERE Branch.Branch_city = 'Brooklyn');
-- Q.31
SELECT Customer_name FROM Depositor JOIN Account ON Depositor.Account_no = Account.Account_no WHERE Account.Branch_name = 'Perryridge' GROUP BY Depositor.Customer_name HAVING Count(*) <=1;
-- Q.32
SELECT Customer_name FROM Depositor JOIN Account ON Depositor.Account_no = Account.Account_no WHERE Account.Branch_name = 'Perryridge' GROUP BY Depositor.Customer_name HAVING Count(*) >= 2;
-- Q.33
CREATE VIEW all_customers AS (SELECT A.Branch_name, D.Customer_name FROM Account A JOIN Depositor D ON A.Account_no = D.Account_no)
UNION
(SELECT L.Branch_name, B.Customer_name FROM Loan L JOIN Borrower B ON L.Loan_number = B.Loan_number);
-- Q.34
SELECT * FROM all_customers;
-- Q.35
CREATE VIEW Perryridge_customers AS SELECT * FROM all_customers WHERE Branch_name = 'Perryridge';



DROP TABLE Account;
DROP TABLE Branch;
DROP TABLE Customer;
DROP TABLE Loan;
DROP TABLE Depositor;
DROP TABLE Borrower;

