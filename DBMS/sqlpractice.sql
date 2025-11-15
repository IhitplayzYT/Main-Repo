CREATE TABLE Emp(
EmpId int PRIMARY KEY,
Ename varchar(20) NOT NULL,
Dept varchar(10) 
);

CREATE TABLE Dep(
DepName varchar(20),
DepId int references Emp(EmpId) 
);


DESC Emp;
DESC Dep;

INSERT INTO  Emp values(91021,'iwdjio','unwefo');
INSERT INTO  Dep values('DEPDDOJDO',91021);
INSERT INTO  Emp values(1,'iwwlvwjio','uwdnwefo');
INSERT INTO  Dep values('DEPDDOcwJDO',1);
INSERT INTO  Emp values(9,'iwdjio','unwefo');
INSERT INTO  Dep values('DEPDDOJDO',910);

select Emp.EmpId,Dep.DepId from Emp,Dep where Dep.DepName = 'DEPDDOJDO';
select Emp.EmpId,Dep.DepId from Emp,Dep;



drop table Dep;
drop table Emp;
