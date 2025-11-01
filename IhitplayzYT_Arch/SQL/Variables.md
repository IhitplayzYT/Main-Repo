Types:
**char(x)** : Will add padding if string length is less then the specified limit.
**varchar(x)** : Will not add padding even if string is of lesser length then limit.
**numeric(x,y)** : Will hold a number of x digits and y decimal places. The x is total inclusive of y.
**int** : Can store data of decimal type.
**small int** : Smaller sized Integer.
**float** : Can store data of type float.
**Date :** Can store date
##### Usage: 
eg:-
```sql
create table mytable(
name varchar(10),  -- Stores 33827 like this 33827
phone char(10),  -- Stores  33827 like this -----33827
Salary float,
Money numeric(10,2)  -- Supports digit number with 2 decimal places
);
```

[[Basics]]
[[Constraint]]
[[Terms & Terminology]]
[[SQL_Node]] : Return to  SQL Root Node.