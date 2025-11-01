##### Foreign key:

- Foreign keys are used to relate / map two tables based on a column.
- A **foreign key** is an attribute (or set of attributes) in one table that refers to the **primary key** (or a unique key) in another table.It is used to **link two tables** and enforce **referential integrity**

- The primary key being used a foreign key in another table.
- The primary key table is called **Parent/Referenced table** where as foreign key table is called as **Child/Referencing table**.
- If a foreign key is referencing a primary key parent table the child table has to be dropped first in comparison to parent.
- Foreign keys can also have an additional behavior attribute connected to them that describes the behavior on deletion of foreign key entry.
	- ON DELETE CASCADE : When a tuple from parent table gets deleted this also causes the deletion tuples containing the value of primary key deleted in all it's child tables.
	- ON DELETE SET NULL : When a tuple from parent table is deleted it causes ONLY the foreign key attributes of child tables to be NULL.

##### Usage :
eg:-
```sql
create table mytable(
eid int primary key
);

create table mytable2(
depeid int,
Constraint fk_cnstrnt Foreign key (depeid) REFERENCES mytable(eid) on delete cascade;
);
```


[[SQL_Node]] : Return to  SQL Root Node.