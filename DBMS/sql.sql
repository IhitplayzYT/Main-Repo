create table emp(
empid numeric(10,0),
ename varchar(20),
salary float 
);

insert into emp values(&empid,&ename,&salary);
select * from emp;
