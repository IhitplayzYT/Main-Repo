##### Check:
Checks if value is valid by rechecking if belonging to a range or set of values.

##### Usage:
```sql
create table table_name(
grade char(1) CHECK (grade IN ('A', 'B', 'C', 'D', 'F')),-- can also use between 'A' and 'F'),
roll int CHECK (roll BETWEEN 1 AND 90),
age int CHECK (age NOT BETWEEN 1 AND 15)
);
```

[[NOT NULL]]
[[UNIQUE]]
[[PRIMARY_KEYS]]
[[FOREIGN_KEYS]]
[[SQL_Node]] : Return to  SQL Root Node.
