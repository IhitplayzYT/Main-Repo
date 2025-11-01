#### Unique
Implies that the value has to be unique in its column, use case are in fields like phone no, aadhar card no,etc

##### Usage:
```sql
create table mytable(
name varchar(50),
aadhar_no numeric(12,0) UNIQUE,  -- Implies aadhar card has to be unique
);
```

[[NOT NULL]]
[[CHECK]]
[[PRIMARY_KEYS]]
[[FOREIGN_KEYS]]
[[SQL_Node]] : Return to  SQL Root Node.