##### Set Operations:

**Union** : Union between two queries
**Intersect** : Intersection between two queries.i.e common entries of two or more queries.
**Except/Minus** : Subtraction,Presence of entries in only one of the Queries.
**Some** : At least one of the entries in the tuple satisfies the condition
**All** : All of them have to satisfy the condition.

##### Usage : 
eg:-
```sql
SELECT column1, column2 FROM table1 UNION SELECT column1, column2 FROM table2;

SELECT column1, column2 FROM table1 INTERSECT SELECT column1, column2 FROM table2;

SELECT ... FROM ... WHERE value < SOME (SELECT ... FROM ...)
```
[[Nested Queries]]
[[Aggregate Functions]]
[[SQL_Node]] : Return to  SQL Root Node.