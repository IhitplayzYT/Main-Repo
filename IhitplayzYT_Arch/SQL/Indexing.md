Index files contain entries of the form (search-key,pointer)

Types of indices:
1. Ordered Indices
2. Hashed Indices - Search keys are distributed into buckets on basis of hash fxn

Fast Access,Insertion,Deletion times and space overhead from indexing

Ordered Index : Sorted on the search-key

Primary Index/Clustering Index : Index whose search-key describes the sequential order of the file.i.e key points to a row sequentially `Direct Indexing into a row since tree is sorted`

Secondary Index/Non-clustering Index : Index whose search-key describes the diff order then the sequential order of the file.i.e key points to pointer that points to a non-sequential entry in table.
`Indexes into a lookup table to get a attrib which alows to index into tuple,done for unsorted`

```
           [x]
       /    |    \
 	[<x]   [x]   [>x]    --> Sparse indexes (Index into blocks/No dir access to tuple)
  / | \   / | \   / | \    -->Dense Index(Index for each entry in table)[Leaf level is also called primary index, rest all are secondary index, this whole structure is a multilevel index[B & B+ tree are a way to implement multilevel index in DBMS].
					
```

Index sequential File : Ordered sequential file with primary index.

#### Dense Index Files:
Index record for every search-key value in the file.

#### Sparse Index Files:
Index record only for few search-key value in file. This is possible as the some of the indexes can be omitted since the row and sequentially ordered in the file.To find a key we find a the largest record that is smaller then our search key and then start searching for it sequentially. They take less space and less overhead for insertion and deletion, but are slower for finding records. Basically points to a block and the contents of block might point to another block or maybe row index. Helps speed up lookup time in comparison to linear search lookup in primary index

Good trade off: Sparse index for each block in which we have to search for our key in that block.

#### Secondary Indices:
Key points to blocks which contain a list of indexes that point to in order entries in the table. Secondary indexes have to be dense.

![[10-11-2025==15-56_1.png]]
Here each index points addresses(buckets) which when followed point to the table.The table is sorted on id NOT ON SALARY.
Here the white table is secondary index.

#### Primary Indices:
Points directly to table.Sorted.Memory Intensive.Fast search
Secondary : Points to primary key which then point to row.Unsorted

#### Multilevel Index:
Outer index - Sparse index to primary indexes
Inner index - primary index

![[Pasted image 20251108092807.png]]

# Types of Indexes
```                
                  Indexes
              /      |      \
        Primary  Clustered  Secondary
        /    \
    Dense    Sparse    
    
```

#### Primary:Dense
Ptr to every entry
#### Primary:Sparse
Ptr to blocks(i.e index table will have 1,11,21,..) meaning 1 points to entries in range 1-10 ,11 point to 11 - 20 and so on... Only works for ordered. It is also a clustering index.
#### Clustering
Table contains duplicate on key so lets say key with val 1 are clustered together into a single block. Primary index is also a clustering and sparse index 
#### Secondary
Index table pointing to another index table that finally points to the database data.Unordered
eg:
Primary table have 0,101,201... where 0 points to secondary table having indexes 0,11,21,... and these indexes point to tuples in their respective ranges.
Secondary indexes have to be dense.
```
		/                 bucket  - Data
								 \ Data
		
secondary                        / Data
Index    __              bucket - Data
Table                           \ Data
	    \                  bucket

primary index table is sparse but each secondary index table is dense. Data can be unordered or ordered here.

```

Indexing has many benefits for searching,but db modification has to update every index.

Sequential scan with primary index is efficient but seq scan on secondary expensive.(Primary has ranges,secondary is dense for each entry on unsorted key)

Indexing has issues like when deletion is to occur the index entries for both dense and sparse have a overhead. Periodic reorganization of the file and performance degredation as the file grows have made the B+ - trees an alternative to indexed-sequential files.B & B+ trees are M-way tree with self balancing and expanding features added just by a few simple rules on a M-way tree.

# Index Updation
## Primary
#### Deletion
If record deleted in dense index updation is similar to deletion of that record,index is deleted.

If sparse index in index table the next indexes are shifted to the index of the deleted.
#### Insertion
In dense if it not appear simply insert
In sparse if entry in an already present block no change to be made but if a new block made for the entry the first search key of new block is inserted in index.

### Questions of Indexing.

Q1. Block size = 1000 , record size = 250, No of records = 10000. Calc space and time complexity without indexing.

Records per block = 1000 / 250 = 4
Total no blocks required = 10000 / 4 = 2500

Space complexity = 2500
Time : Worst = 2500
	   Best = 1

Q2. Block size = 1000 , record size = 250, No of records = 10000. Calc space and time complexity with indexing. Index entry = 20

Blocks for records = 2500
##### Dense:
Index for each record (Total no of indexes = 10000)
Blocks for all indexes = 10000 * 20 / 1000  = 200 blocks
Total blocks for dense = 2500+200 = 2700
Space complexity : 2700
Time : log<sub>2</sub> 200 + 1 
##### Sparse:
Index for each block (Total no of record blocks = 2500)
Blocks for all indexes(Total no of indexes/Index per block) = 2500 * 20 / 1000  = 50
Total blocks for sparse = 2500+50 = 2550
Time : log<sub>2</sub> 50 + 1

---
#### Disadvantages of Indexes:
Performance degrades as file grows(many blocks)
Periodic reorganization of file.

### Advantages of B+ trees:
Automatic reorganization with local changes
No need to reorganize the entire file.
#### Disadvantage of B+ trees:
Extra insertion and deletion overhead
Space Overhead

![[Pasted image 20251108093854.png]]

### Properties of B+ trees:
n - Max no of children a node can have

1. Each non-root and non-leaf node has between n/2 to n keys(Rule imposed on M-way tree to avoid growing linearly and making lookup linear) and n children
2. A leaf node has between (n-1)/2 and (n-1) values.
3. All paths from root to leaf are same(Height balanced)
4. If root not leaf it has min 2 children
5. If root node is leaf it can have 0 to n-1 values in it.

For k search key pairs and out degree of each node being n(Note n children means n-1 keys in parent node) the maximum height of the B+ tree if log<sub>n/2</sub> (k) 

![[Pasted image 20251108094626.png]]
Pi are child ptrs and Ki are search key values

#### Leaf level:
Pi are sorted and point to file records for Ki. Assuming n is max no of values a node holds Pn point to the node starting with Pn+1 and so on. Basically left leaf node point to right one soon and so forth.

#### Non-leaf level:
- Non leaf nodes form a multi level sparse index on the leaf nodes. 
- Each node in B+ tree can hold n ptrs and n-1 keys.i.e n children and n-1 search keys in the node itself.
- For total of K entrys the max height is log<sub>n/2</sub> k
- Typically the size of one node is same as size of a block(4KiB) , each index entry is 40 bytes so roughly 100 is the span of one node.
- If duplicates present in B tree we can guarentee k0 <= k1 <= ....

B<sup>+</sup> trees solve index file degradation and data file degradation when used in file organisation but there are much less records in a node then the pointers in the node.

B trees are like B+ trees except that as we see the record itself doesn't repeat in the sub-tree and an additional record pointer for each key is needed. Insertion and deletion is B tree is harder. B+ tree is preferred over B tree due to less overhead and easier to program.

![[Pasted image 20251108135715.png]]

B tree has a record pointer for all nodes in at each and every level.B+ tree has record pointer only at the leaf nodes. Making B+ tree be perfect for implementing multi-leveled indexing.

B-tree have advantage that they may use less nodes then B+ tree and it is possible to find key value ptr before reaching the leaf node.

B-tree disadvantage is non-leaf nodes are larger,so less fan out and hence are more deeper.Insertion deletion and implementation of B trees is also harder.

Composite search key - Search key containing more then one attrib and there is lexicographic ordering in the composite key.

eg : select * from x where x.a = a and x.b = b;

Using simple indexes means retrieving all entries with x.a = a and then manual searching for x.b = b  or x.b = b then manual searching for x.a = a.

Composite index returns the exact conditions in the query making it more efficient.
#### Covering index:
Add extra attribs to index so that some queries can avoid retrieving the actual record entries (useful for secondary indices)

# Hashing

### Static Hashing
Fixed no of buckets

Bad hashing fxn map to single bucket a ideal hashing fxn distributes the attrib uniformly and is random, the hash fxn usually works on the inner binary representation of the key.

##### Reasons for Bucket overflow
1. Less buckets
2. Skew in data
	1. Bad hash fxn
	2. Multiple records with same key


#### Overflow chaining:
Overflow buckets of the overflowing bucket is chained like a linked list.(Closed Hashing)Preferred for DBMS

Open hashing not preferred for DBMS

##### Hash Indexes:
Search Key → Hash Function → Bucket → Record Pointer → Actual Record

Static hashing problem:
Slows down with more entries as less buckets
If less entry it waste space
To fix shortcomings of the static hashing we allow the buckets to be modified dynamically.

### Dynamic Hashing
Hash fxn modified dynamically and the struct based on this modified hash.
1. Extendable Hashing: Generates n-bit number use only prefix of i bits to hash to index into table of bucket entries.

We use h(k) = 101010001....
we use the first i bits for indexing into the bucket address table then follow the ptr from the bucket address table to bucket where we search sequentially.

When the bucket gets filled or overflow happens we either increment i(local < global) or split a bucket into two and use another bit to distinguish the two(when local order == global order).

## Ordered indexing vs Hashing
- Cost over periodic re-organisation(B+ does re-balancing and incase of extendable hashing bucket splitting)
- Freq of insert and deletion(B+ handle high freq better)
- Desirable to optimize best case at expense of worst case(B+ has log N but extendable has worst O(n)(To many collisions) and best O(1))
- Hashing better for a specific value of key but ordered index is better for range of values of key