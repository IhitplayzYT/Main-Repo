##### Nested Queries

First the nested query followed by the outer queries.
- Used with **Where** and **In**

##### Usage :
```sql
Select salary from emp where emp.salary > (select avg(salary) from emp);
Select salary from emp where emp.salary > SOME(select salary from emp where dept = 'cse');
Select * from emp where emp.eid in (select eid from dept);
```

[[Correlation Queries]]
[[SQL_Node]] : Return to  SQL Root Node.