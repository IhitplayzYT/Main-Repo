##### Primary Key:
- Implicitly makes a column NOT NULL and UNIQUE
- Used to map to a foreign key in another separate table.
- This allows mapping of data from one table to another.
- The primary key being used a foreign key in another table.
- The primary key table is called **Parent/Referenced table** where as foreign key table is called as **Child/Referencing table**.
- If a foreign key is referencing a primary key parent table the child table has to be dropped first in comparison to parent.

##### Usage:
eg:-
```sql
create table mytable(
id varchar(10) Primary key,   -- Now id can be used a relation to a foreign key.
eid varchar(20),
Constraint pk_cnstrnt primary key (eid,id), -- Create a compound key
...
);
```


[[NOT NULL]]
[[UNIQUE]]
[[CHECK]]
[[FOREIGN_KEYS]]
[[SQL_Node]] : Return to  SQL Root Node.
