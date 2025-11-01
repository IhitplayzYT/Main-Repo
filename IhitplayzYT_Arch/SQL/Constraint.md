##### Constraints : 
There are 5 basic Integrity Constraints:
1. `NOT NULL` : The entry cannot be NULL/Empty.
2. `UNIQUE` : The entry has to be UNIQUE in it's field.
3. `CHECK` : Checks if value entry fits a range or criteria.
4. `PRIMARY KEY` : The primary key is a column (or a set of columns) that uniquely identifies each row in the table. They are implicitly both `UNIQUE` and `NOT NULL` .
5. `FOREIGN KEY` : Foreign key is a column in a table that links to the primary key of another table, establishing a relationship between the two tables.

- The primary key being used as a foreign key in another table.
- The primary key table is called **Parent/Referenced table** where as foreign key table is called as **Child/Referencing table**.
- If a foreign key is referencing a primary key parent table the child table has to be dropped first in comparison to parent.

##### USAGE : 
eg:-
```sql

CREATE TABLE MYTABLE(
<Entry_Name1> <Entry_Datatype1> <Constraint>,
<Entry_Name2> <Entry_Datatype2> <Constraint>,
...
);

```

[[Constraint_keyword]]
[[DEFAULT]]
[[NOT NULL]]
[[UNIQUE]]
[[CHECK]]
[[PRIMARY_KEYS]]
[[FOREIGN_KEYS]]
[[Set Operations]]
[[SQL_Node]] : Return to  SQL Root Node.
