##### Correlation Queries:

- **Exists** & **Not Exists** are the connecting keyword used for correlation queries.
- In correlation queries the outermost query is evaluated and checks if it exists or not in the inner query. It is the opposite of the **Nested Query**.Every time outer query moves forward whole inner query is executed inside again.
- Exists and not exists only care about if a tuple or value returned not what it contains or is.
##### Usage:
eg:-
```sql
SELECT * FROM emp e WHERE EXISTS (SELECT 1 FROM dept d WHERE d.eid = e.empid);
```

[[Nested Queries]]
[[SQL_Node]] : Return to  SQL Root Node.