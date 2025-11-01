##### Default:
It is used to set the default value in-case a value isn't provided.
- Default **IS NOT**  an Integrity Constraint.
- To use default add it before the constraints.

##### Usage :
eg:-
```sql
create table mytable (
salary int default 2000,
id varchar(10) default '1111111111',
status varchar(10) default 'Employee'
);
```


[[SQL_Node]] : Return to  SQL Root Node.