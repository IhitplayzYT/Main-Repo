##### String Matching Operators:
Help in string matching.
**%** : Matches any sub-string, it matches 0 or more.
**`_`**  : Matches any char.
**||** : String Concatenation
##### Usage:
`%dar_` : Matches any word following pattern ...dar(a-zA-Z)
`___` : Matches any 3 letter word.
`%hell%` : Matches any word containing "hell" in it.

**Like** : Keyword used when matching strings

eg:
```sql
select dept_name from dept where dept_name like '%dar%'
```

[[Aggregate Functions]]
[[SQL_Node]] : Return to  SQL Root Node.