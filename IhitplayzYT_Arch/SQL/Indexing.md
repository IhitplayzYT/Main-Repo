Index files contain entries of the form (search-key,pointer)

Types of indices:
1. Ordered Indices
2. Hashed Indices - Search keys are distributed into buckets on basis of hash fxn

Fast Access,Insertion,Deletion times and space overhead fro indexing

Ordered Index : Sorted on the search-key

Primary Index/Clustering Index : Index whose search-key descibes the sequential order of the file.i.e key points to a row sequentially

Secondary Index/Non-clustering Index : Index whose search-key describes the diff order then the sequential order of the file.i.e key points to pointer that points to a non-sequential entry in table

Index sequential File : Ordered sequential file with primary index.

#### Dense Index Files:
Index record for every search-key value in the file.

#### Sparse Index Files:
Index record only for few search-key value in file. This is possible as the some of the indexes can be omitted since the row and sequentially ordered in the file.To find a key we find a the largest record that is smaller then our search key and then start searching for it sequentially. They take less space and less overhead for insertion and deletion, but are slower for finding records.

Good tradeoff: Spare index for each block in which we have to search for our key in that block.

#### Secondary Indices:
Key points to buckets which contain a list of indexes that point to unorder entries in the table. Secondary indexes have to be dense.

Primary : Points directly to tulple in table.Sorted.Memory Intensive.Fast search
Secondary : Points to primary key which then point to row.Unsorted

#### Multilevel Index:
Outer index - sparse index of primary indexes
Inner index - primary index

![[Pasted image 20251101142447.png]]

Indexing has issues like whjen deletion is to occur the index entries for both dense and sparse have a overhead. Periodic reorganisation of the file and performance degredation as the file grows have made the B+ - trees an alternative to indexed-sequential files

### Advantages of B+ trees:
Automatic reorganisation with local changes
No need to reorganise the entire file.
#### Disadvantage of B+ trees:
Extra insertion andc deletion overhead
Space Overhead
![[Pasted image 20251101143044.png]]


Properies of B+ trees:
1. Each non-root and non-leaf node has between n/2 and n  children
2. A leaf node has between (n-1)/2 and (n-1) values.

![[Pasted image 20251101143452.png]]


For k search key pairs and outdegree of each node being n the maximum height of the B+ tree if log<sub>n/2</sub> (k) 

B trees are liek B+ trees except that as we see the record itself doesn't repeat in the subtree. Insertion and deletion is B tree is harder. B+ tree is preffered over B tree due to less overhead and easier to program.


