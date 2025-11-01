##### Cartesian Product:
It is an old way to join tables but is often inefficient since it does matrix multiplication i.e if two table m x n and p x q it creates a temp table of m * p x n + q and to use such join generally a where conditional is required.

```sql
select emp.empid,emp.empname,dep.depname,dep.salary from emp,dept where dep.depid = emp.empid  
```

##### Joins:
Newer and recommended way to join/combine two tables and avoid the scenario of making a temp table of m * p  x n * q. ON is used to specify the common column on which we must join.
```sql
select emp.empid,emp.empname,dep.depname,dep.salary from emp JOIN dep ON dep.depid = emp.empid   -- Controlled Join.
```

##### Natural Join:
Natural joins join two tables implicitly by a common column, this can be problematic if more then one common column exist between both.
```sql
SELECT column1, column2, ... FROM table1 NATURAL JOIN table2;  --Auto joins on common attributes.
```

[[Constraint]]
[[SQL_Node]] : Return to  SQL Root Node.