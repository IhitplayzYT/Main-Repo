**Row/Entrys/Tuple/Instance/Record** : A single record in a table. Also called a **record**.

**Column/Field/Attribute** :  A field in a table. Each column has a name and a data type.

**Table/Schema/Relation**: The structure of the database (tables, columns, data types, relationships).

**Constraints** : A **[[Constraint]]** is a rule applied to columns to ensure data integrity.

**DML** : Is the query written to interact,access and manipulate and get data or set data in a data model.
- **Procedural** : User specifies what data and how to get.	
- **Declarative(Non-Procedural)** : Specifies what data but not how to get it.

**DDL** : Is the query required for creating the data base.

- A data base must have high **ACID ( Atomisity, Consistency, Isolation & Durability)**.

**Atomisity** : Either all steps 
must be done or none.

DDL compiler generates table templates stored in a data dictionary. The data dictionary also consists of table metadata.

**Parent Table/Referenced Table** : Table containing the primary key that is being referenced by another table's foreign key.

**Child Table/Referencing Table** : Table containing the foreign key.

`Physical Data Independence – The ability to modify the physical schema without changing the logical schema`


## Why not use files for data storage

1. Data inconsistency and redundancy.
2. Difficult to access data
3. Data isolation
4. Integrity problems: Hard to add constraints and checks
5. Atomicity of updates(Partial updates make database inconsistent )
6. Concurrency issues
7. Security problems: Hard to provide access.


## Example:

| EmployeeID | Name        | Age | Department | Salary |
| ---------- | ----------- | --- | ---------- | ------ |
| 1          | Alice Smith | 30  | HR         | 50000  |
| 2          | Bob Johnson | 45  | IT         | 60000  |

- `1, Alice Smith, 30, HR, 50000` & `2, Bob Johnson, 45, IT, 60000` are two **Rows or Tuples or Entrys or Instance**.
- `EmployeeID`, `Name`, `Age`, `Department`, and `Salary` are all **Columns/Fields/Attributes**.
- `create table employee` is **DDL** and `Select * from employee` is **DML** 

[[Constraint]]
[[SQL_Node]] : Return to  SQL Root Node.