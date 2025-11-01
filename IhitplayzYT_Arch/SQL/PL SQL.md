##### Basic Block
```sql
BEGIN
DECLARE
END;
/
```
##### Declaration,Printing and User Input

```sql
BEGIN
x INT;
y Student.Roll%TYPE;
z Student%ROWTYPE;
DECLARE
x := &input_val_of_x;
DBMS_OUTPUT.PUT_LINE("Ans : " || x);
END;
/
```

##### If-else ladder
```sql

IF COND THEN
ELSIF COND THEN
ELSE
END IF;
```

##### Loops
```sql
LOOP
END LOOP;

WHILE COND LOOP
END LOOP;

FOR I IN [REVERSE] LL..UL LOOP
END LOOP;

GOTO hello
<<hello>>
```

##### Exceptions
```sql
BEGIN
myexcep Exception;
DECLARE
THROW myexcep;
EXCEPTION
WHEN excep1 THEN
    <>
WHEN OTHERS THEN
	<>
END;
/

-- SOME DEFINED ERRORS

-- DUP_VAL_ON_INDEX
-- NO_DATA_FOUND
-- ZERO_DIVIDE
-- VALUE_ERROR
-- TOO_MANY_ROWS

```


##### Cursors
```sql
-- Implicit cursors
BEGIN
DELETE FROM Student WHERE dept = dname;

IF SQL%FOUND THEN
ELSIF SQL%NOTFOUND THEN
END IF;

END;
/

-- Explicit Cursors
BEGIN
Cursor c1 is select a,b,c from emp;  -- Normal Cursora
Cursor c2(x int) is select a,b,c from emp where emp.x = x;
val emp%rowtype;
DECLARE

open c1;
loop
fetch c1 into val
exit when c1%notfound
end loop
close c1

for i in c1(x) loop
end loop;

END;
/
```

##### Functions & Programs
```sql
CREATE OR REPLACE PROCEDURE proced_name(x IN INT,y OUT Float,z IN OUT char) IS 
-- var declare here
BEGIN
END;
/
-- In - Non modifyable
-- Output field modifyable but value passed out is zeroed out
-- Modifyable field but whatever value provided will be used



CREATE OR REPLACE FUNCTION fxn_name(x INT,y FLOAT) RETURN INT IS 
BEGIN
END;
/

```

##### Triggers
```sql
CREATE OR REPLACE TRIGGER trig [BEFORE | AFTER | INSTEAD OF] UPDATE or DELETE or INSERT ON table
FOR EACH ROW
BEGIN
END;
/
-- :old & :new give access to varib before and after the DML command would be carried out
```





