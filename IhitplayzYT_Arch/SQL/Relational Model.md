##### Relational Model

- Relational Database is based on the relational model which consists of tables and the relations among them.
###### Properties of a Relation (Table/ Set of rows):
1. Each relation contains only one record type.
2. Each relation has a fixed number of columns that are explicitly named. 
3. Each attribute name within a relation is unique.
4. No two rows(tuples) in a relation are the same.
5. Each item or element in the relation is atomic i.e can't be broken down to more rows.
6. Rows have no ordering associated with them.
7. Columns have no ordering associated with them.

**Degree of a relation** : Number of columns in the relation
**Cardinality of a relation** : Number of rows in the relation
**N-ary relation** : Relation with degree N.
**Domain** : Set of allowed values for each attribute.

- Domain is atomic if all its members are atomic.
- NULL is a part of domains.

r(R) - Denotes a relation r on relational schema R (Table).

##### Types of Keys:
1. `Primary` : NOT NULL and UNIQUE and can be used to uniquely identify a row in a table. (Single value or Combination)
2. `Foreign` : Refers to primary key of another table,DOESN'T GUARANTEE UNIQUENESS. (Single Value)
3. `Candidate` : A set on minimal attributes that could have been chosen as Primary key,also includes the primary key. (Single value or Combination)
4. `Super` : A set of individual AND COMBINATIONS of all primary and candidate keys. (Single values and also made of combination)
5. `Alternate` : A candidate key not chosen as the primary key.All candidate keys excluding the primary key. 
6. `Composite` : A candidate key made of COMBINATIONS.


- CANDIDATE KEY IS ALSO CALLED MINIMAL SUPER KEY.
- MINIMAL SUPER KEY CONTAINS ONLY THE MINIMUM NO OF ELEMENTS TO UNIQUELY IDENTIFY A ROW.

- Relation appears as box with Relation name at the top and Attributes listed inside.
- Primary key attributes are underlined.
- Foreign key dependencies appear as arrows from the foreign key to the primary key.
Instructor.dept_id  ───────►  Department.dept_id
    (Foreign Key)                (Primary Key)

[[Relational Algebra]]
[[SQL_Node]] : Return to  SQL Root Node.