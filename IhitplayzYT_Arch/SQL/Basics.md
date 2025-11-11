##### Creation of a Table:
```sql
create table <tablename>(
<data_name> <data_type> <Constraints>,
<data_name> <data_type> <Constraints>,
<data_name> <data_type> <Constraints>,
...
);
```

##### Deletion of a table:
```sql
drop table <tablename>;
```

##### Deletions of rows/entries:
```sql
delete from <tablename>;

delete from <tablename> where <datamember> >= (select <table_attrib> from <table2>);
```

##### Printing/Selecting:
```sql
select * from <tablename>;  -- * means every coloumn
```

##### Printing all or distinct:
```sql
select distinct id from tablename;  -- Lists only distinct/unique values.
select all id from tablename;  -- List every entry in id.
```

##### Printing in Ascending and Descending order:
```sql
SELECT NAME FROM EMP ORDER BY NAME DESC  -- ASC for ascending wise by the NAME attribute.
```

##### Printing Schema:
```sql
desc tablename;  -- Prints skeletal/schema of the table
```

##### Add a column:

```sql
alter table <tablename> add(<data_name> <data_type> <Constraints>)
```

##### Dropping a column:
```sql
alter table <tablename> drop column <column_name>; -- Specifically needs the column identifier.
```

##### Modify a column:
```sql
alter table <tablename> modify(<oldname> <new_type> <new_constraints>);
```

##### Conditional Selection:
```sql
select * from <tablename> where (condition);

select * from tabl1,tab2 where (tab1.attrib1,tab2.attrib1) = (tab2.attrib1,tab2.attrib2);  -- Tuple comparison
```

##### Copying a table and it's entries:
```sql
create table <clone_table> as select * from <original_table>
```

##### Renaming Table  & Column & Relations:
```sql
RENAME <oldname> TO <newname>; -- Rename a table

ALTER TABLE employees RENAME COLUMN ename TO empname; -- Rename a Column permanently & also needs column keyword.

SELECT ename as Empname FROM Employee Emp; -- Rename a Column & table temporarily
```

##### Inserting entrys into a table:
```sql
insert into <tablename> values(...); -- To insert a tuple

insert into <tablename>(attrib1,attrib3,..) values(); -- to supply a fixed nuber of attributes to repective columns.

insert into <tablename> values(&attribute1,'&attribute2',...); -- ASks for user input to insert into table attrib2 is string and attrib1 is interpreted as a number.
```
##### Adding a Foreign Key Constraint to the table:
```sql
ALTER TABLE child_table ADD CONSTRAINT fk_name FOREIGN KEY (child_column) REFERENCES parent_table(parent_column);
```

##### Modify pre-existing entrys in a table:
```sql
update emp set attrib = <val> where (Condition);
```

`View are virtual tables with fast access`

##### Creating a view
```sql
create view view_name as select * from table;
```

[[Constraint]]
[[Logical Operators in SQL| Logical Operators ]]
[[SQL_Node]] : Return to  SQL Root Node.
