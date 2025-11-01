##### Constraint keyword:
- Used to name a constraint.
- Used to add ON DELETE options in Foreign keys.
##### Usage :
eg:-
```sql

create table mytable(
id int CONSTRAINT id_cnstrt UNIQUE,
CONSTRAINT <user_defined_name> <constraint> (<attrib>),
CONSTRAINT id_fk FOREIGN KEY (id) REFERENCES departments(dep_id) --Special case for declaring a foreign key
);
```

[[FOREIGN_KEYS]]
[[SQL_Node]] : Return to  SQL Root Node.