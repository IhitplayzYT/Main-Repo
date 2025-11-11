##### Aggregate Functions 
**COUNT** :	Counts rows or non-NULL values
**SUM** : Calculates the total sum of numeric values
**AVG** : Calculates the average of numeric values
**MIN** : Returns the smallest value
**MAX** : Returns the largest value

- Generally used with the **Group By  & Having** Functions.

**Group By** : To group-by a specific attribute.
**Having** : To apply a filter on the result from the group by function.

##### Usage :
eg:-
```sql
SELECT COUNT(salary) FROM employees;

SELECT department_id, AVG(salary) FROM employees GROUP BY department_id; --Counts the avg salary of each department

SELECT department_id, COUNT(*) AS num_employees FROM employees [OPTIONAL WHERE CLAUSE] GROUP BY department_id HAVING COUNT(*) > 5;
-- Counts the number of employess from each department but only prints those with the count greater then 5.

```

[[String Operators]]
[[Nested Queries]]
[[Correlation Queries]]
[[SQL_Node]] : Return to  SQL Root Node.