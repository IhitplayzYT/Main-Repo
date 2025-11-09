### Functional Dependancy

**Functional Dependence** : Relation of one attribute with another attribute in the table.

FD : X -> Y
X is called Determinant.
Y is called Dependent variable/Discriminant.
Y is functionally dependent on X

FD : X -> Y means that for every tuple with X being same (tuple1.X == tuple2.X ) then for FD to hold , ONLY THEN the (tuple1.Y = tuple2.Y)

1. Partial Functional Dependancy : Even if we remove attrib/Col from X and the FD holds.
### Decomposition :
- Table broken into 2 or more sub-tables
##### Types of Decomposition:
1. Lossy Decomposition : Joining all tables doesn't result in original table
2. Non-Lossy Decomposition : Joining the tables return the original table.

### Conditions for loss-less
1. R1 U R2 == R 
2. R1 intesection R2 != Null
3. R1 or R2 or both have to have candidate key in them.

#### Matrix method to find if lossy or loss-less
1. Make a matrix of rows(R1 AND R2) and cols members of R (A,B,C,D,...).
2. Fill all the attrib in related daughter tables in matrix
3. Use the FD relation, if the determinant column is fully filled then the discriminant col can also be fully filled.
4. If any row is full filled then it is Loss-less.

eg : R(A,B,C,D,E) ->  R1(A,B,C) & R2(BCD) & R3(CDF)
FD:{AB -> CD , A -> E ,C -> D}

		A  B  C  D  E
	R1  x  x  x  o
	R2     x  x  x
	R3        x  x  x

So it is lossy.

### Normalization
1. The join has to be lossless
2. The functional dependency has to be preserved.

##### Closure
eg R(A,B,C,D) FD{A->B,B->C,C->D}
A<sup>+</sup> = ABCD -- A gives B, B gives C and C gives D --> All attributes included hence A is a candidate key.
	 ^-Closure of each attribute has to be self determinant(Reflexive property)	
B<sup>+</sup> = BCD  -- B gives C and C gives D.
C<sup>+</sup> = CD 
D<sup>+</sup> = D

Set of  KEY = {A,AB,AC,AD,ABC,ABD...}
SET OF CANDIDATE KEYS = {A} (Since candidate key is minimal)
prime_attrib = {} -- since prime_attrib != CK
- Since if attrib is candidate key then all the combinations containing candidate key also form a candidate key.

- Any attribute whose closure contains all the attributes of the relation is called as a candidate key.

- Every attribute in each candidate key is a called prime attributes.

R(ABCDE) FD:{A->B,BC->D,E->C,D->A}
(AE)<sup>+</sup> = AEBCD --> AE is due to reflexive property.
A gives B and E gives C and BC gives E hence its a candidate key.

Normalization is the process of **spiting data in a database** to small tables:
1. **Eliminate redundancy (duplicate data)**
2. **Avoid anomalies** (insertion, update, deletion problems)
3. **Ensure data integrity**

eg : R(ABCD) to R1(AB) R2(BC) and FD:{A->B,B->C,C->D,D->B} 

R1(AB)              |          R2(BC)
A -> B	(valid in 
FD)          		|	 B->C  (Valid in FD)
B -> A	(B<sup>+</sup> doesnt 
contain A hence 
	invalid)		|	 C->B (Valid C+ contains B)
					|	
					|	
Note all valid FD
NEW_FD {A->B,B->C,C->B}
- IF NEW_FD DOESN'T CONTAIN ALL/CLOSURE OF OLD_FD NOT PRODUCIBLE BY NEW FD OF FD THEN THE TRANSFORM NOT DEPENDENCY PRESERVING.

ROW LEVEL DUPLICATION - Two tuples are common
COLUMN LEVEL DUPLICATION - Two columns are common

1NF - Only atomic values

2NF - Relation must be in 1NF & there shouldn't be no partial dependency

3NF - Relation must be in 2NF & there shouldn't be no transitive dependency

BCNF - Relation must be in 3NF & Determinant in FD must be candidate key.

4NF - No multivalued dependencies

5NF - No join


#### To make 1NF:
1. Multiple tuple with each tuple taking one attrib of composite attrib(Composite pk)
2. Adding another attribute(Adds NULL to some columns)
3. Decompose table

#### To make 2NF:
1. Must be in 1NF
2. No partial dependency - No non-prime attribute is partially dependent on a prime attribute.(Prime attrib of CK shouldn't determine any non-prime attributes). 
### Entirety of CK isn't termed as prime attribute.

#### To make 3NF:
1. Must be in 2NF
2. No transitive dependency
3. (
		for all FD: X->Y
	 1. X has to be a SK or CK
     2. Every attrib of Y is prime(Part of some CK)
    )

#### To make BCNF:
1. Table must be in 3NF
2. All FD:X -> Y ,X must be SK/CK
