##### Terms : 
- **Entity** : A real world object that is distinguishable from the rest.(Can be thought of as a tuple of a table)
- **Entity Set** : A collection of entities.(Can be thought of being a collection of entities(NOT A TABLE).)
- Scheme/Table is also called a Entity Type.
- In ER diagrams rectangle is an Entity NOT A Entity set.
- **[[Relational Model |Domain & Attributes]]** 
- **Relation** : A association between multiple entities.
- **Relation Set** : A table/set of all relationships between two or more entities.
- **Participation** : The association between entity sets.
- **Entity's Role** : The function that an entity plays in a relationship
- **Descriptive Attributes** : A relationship may also have attributes to add additional information to a relation.
- **Recursive Relationship** : If the same entity sets participates in a relationship set more than once in different roles.
- **Degree** : No of entity sets participating in a relation.
- **n-ary** : "n" no of entity sets participating in a relation.

##### Types of Attributes:
1. Simple : Indivisible and atomic.(eg - First Name)
2. Composite : Have several components in them.(eg - Full Name)
3. Derived : Derived from some other attribute.(eg - Age can be derived from DOB).
4. Multivalued : Can have more then one value(eg : gmail can be many for each person.)

Eg : Student **ENROLLS IN** Course. Is a relation

Sid | Cid  -> Is a relation set
1   |  2   ->Each element is a relation set instance
3   |  9

![[Screenshot_20250828_162813.png]]

- Here date is a descriptive attribute 
- Advisor is the relationship set.
- Instructor & Student are entity sets
- Id is primary key.

## Rules in ER Diagrams:
- Rectangles represent entity sets.
- Diamonds represent relationship set.
- Attributes listed inside Ovals
- Underline indicates primary key attributes
- Derived attributes are shown with dashed circle
- Multivalued attributes are shown with two ovals.
- Foreign keys are shown by dashed underline. 
####                      ER Diagram
![[Screenshot_20250828_163258.png]]
####            Table for Above ER Diagram
![[Screenshot_20250828_163430.png]]
 
#### Cardinality Constraints:
1. one to one - One entity set from A can at most be related to one entity set of B.
2. one to many - One entity set of A can have more then one relationship with entity set B,but reverse not true.
3. many to one - One entity set of B can have more then one relationship with entity set A,but reverse not true.
4. many to many - One entity set A can be related to many entity sets B and vice versa.

**-> - One
-- - Many**

**Cardinality can also be denoted in l..h format where l is lower bound and h is upper bound for participation**

#### Participation of Entity in Relationship
- Total Participation Denoted by =====
- Total Participation : Every entity in entity set participates in at least one relation.
- Partial Participation : Not all the entities in entity set participate in the relation.

#### Types of Entity sets:
- Strong : Entity set with primary key.
- Weak : Doesn't have enough attributes to form primary key.
- Weak entity denoted by double rectangle.

`For weak entity set to exist it needs an identifying entity set which has be TOTAL PARTICPATION AND ONE TO MANY....ie ONE entity set from identifying entity set to 1 or many entity in weak entity set.`

- **An Identifying relation is denoted by double diamond in the identifying relation.**

- **Discriminator/Partial Key** : Set of all attribute that discriminate between the tuples of the weak entity set that are inside the weak entity set.

- In weak entity set we have dashed-line for attribute that are discriminators.

# Reduction of ER Diagrams to Relational Schema:

### 1. Strong Entity
- **Composite Attribute** : ONLY ADD CHILDREN TO THE SCHEMA NOT THE COMPOSITE ATTRIBUTE.
- **Derived Attributes** : IGNORE DERIVED ATTRIBUTES.
- **Multivalued Attribute** : DENOTE AS A SEPARATE TABLE **FOR EACH ** MULTIVALUED ATTRIBUTE. THIS TABLE HAS TO INCLUDE THE PRIMARY KEY OF THE ENTITY IT ATTRIBUTE OF.
---
### 2. Weak Entity
- **Weak Entity** : HERE WEAK ENTITY COMBINED WITH IDENTIFYING RELATION AND THE PRIMARY KEY OF IDENTIFYING ENTITY IS ALSO ADDED TO THIS SCHEMA. THE PRIMARY KEY OF THE SCHEMA IS A COMPOSITE KEY CONSISTING OF THE PRIMARY KEY AND THE DISCRIMINATING KEY OF THIS WEAK ENTITY.
---
### 3. Relations
- **Relations** : IN THIS THE RELATION IS CONVERTED TO ANOTHER TABLE THAT CONTAINS OF THE DESCRIPTIVE ATTRIBUTES AND THE PRIMARY KEY OF ALL THE ENTITY SETS IT'S RELATED TO. For many-many cardinality.
- ALONGSIDE THE NORMAL TABLES FOR BOTH THE ENTITY SETS.
---
### 4. Cardinality
1. ONE-ONE : IN THIS 2 POSSIBLE ANSWERS. COMBINE RELATION AND ONE TABLE THIS COMBINED TABLE ALSO HAS TO INCLUDE THE PRIMARY KEY OF OTHER TABLE. SO IN BINARY CARDINALITY THERE ARE 2 TABLES.
2. MANY-ONE OR ONE-MANY : IN THIS THE MANY IS COMBINED WITH THE RELATION AND HAS TO INCLUDE THE PRIMARY KEY OF THE OTHER TABLE. SO A TOTAL OF 2 TABLES IN BINARY CARDINALITY.
3. MANY-MANY : IN THIS THE RELATION HAS ITS OWN SEPARATE TABLE THAT CONSISTS OF PRIMARY KEYS OF BOTH THE TABLE IN THE RELATION TABLE. SO A TOTAL OF 3 TABLES IN BINARY CARDINALITY.
---
### 5. Participation IN BINARY CARDINALITY
1. Total Participation on one side : THE TOTAL PARTICIPATION SIDE AND THE RELATION TABLE ARE COMBINED THIS COMBINED TABLE CONTAINS THE PRIMARY KEY OF THE OTHER ENTITY.
2. Total Participation on both sides : BOTH ENTITIES AND RELATION ARE COMBINED TOGETHER.

[[SQL_Node]]
