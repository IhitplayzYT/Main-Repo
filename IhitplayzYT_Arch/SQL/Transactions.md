**Transaction** : Sequence of steps executed
**Schedule** : Sequence and ordering of Transactions
Transaction has only two result commit and abort

**Active** - State the txn stays in during execution
**Partially Committed** - After the final statement of the txn executed.
**Failed** - After discovery that normal execution can't take place
**Aborted** - After txn is rollbacked it can either be restarted or killed/skipped.
**Committed** - After successive execution

write & commit both write the changes to the DB

If a txn aborts it rollbacks and either repeats the txn or skips it in order to satisfy the ACID properties of the DB.

Commit to DB is complete commit

Serial schedule in which before another txn is started the txn before has to be full complete(commit or abort)

In concurrent schedule the consistency property of ACID isn't satisfied most of the time

Isolation: Intermediate results of the txn shouldn't show up and affect the values of progress of another txn

Two schedules are equivalent is the values of varib A,B,C,... are equal AND the sum of all of them is also equal.

If sum preserved but value not equal txn is Non-equivalent.
##### Why concurrent preferred?
1. Increased disk and processor utilization
2. Reduce avg response time

#### Problems during Concurrent Txns:
1. Dirty Read problem
2. Lost Update problem/Blind Write problem
3. Incorrect Summary problem
4. Unrepeatable Read problem

##### Dirty Read Problem
T2 takes intermediary value from T1 but T1 aborts so T1 should rollback and the value of variables should also ,BUT T2 has already started using the incorrect old failed txn value resulting in a failing the Atomicity property.
eg:
		T1   |   T2
		...
				R(A)	
		abort
				...
				
##### Lost Update Problem/Blind Write Problem
T2 performs some operations but when T1 comes it doesnt read the new value instead it does a write operation which means the new value from T2 is overridden by an Old value of T1 making T1 do wrong operations and also T2. This fails the Isolation property.
eg:
		T1   |   T2
				R(A)
		R(A)
		A+=10
		W(A)
				W(A)
				...
				commit
		commit	
			

##### Incorrect Summary Problem
A summation fxn or varib or any aggregating operation is carried out in T1 but T2 modifies a varib midway causing the sum in T1 to be wrong
eg : 

		T1    |    T2
				R(a)
				sum(a)
	   R(b)
	  b += 10
				R(b)
				sum(b)
				
##### Unrepeatable Read Problem
When in a txn T1 even if no operation happened between two adjacent reads in T1 the value from both reads is diff, this happens since the varib was changed in T2 and written causing the back to back read in T1 unrepeatable.
eg :
		T1   |   T2
		R(A)
				R(A)
				A += 10
				W(A)
		R(A)


##### Serializability:
Ability to convert a concurrent schedule to a serial schedule.

#### Conflict Serializablity Methods
1. Conflict Instruction Method
2. Precedence Graph

Conflict Pairs:
R(A) W(A)
W(A) R(A)
W(A) W(A)
Non-conflict Pairs:
R(A) R(A) / R(A) R(B) 
R(A) W(B)
W(A) W(B)

#### Conflict Instruction Method
We can swap adjacent non-conflicting pairs if schedule after swap is serial it is said to be conflict serializable.
(a  )  -> (b  ) 
(  b)     (  a) 

#### Precedence Graph
Create Nodes for each Txn.
Then we go according to schedule for operation in Tx we check for conflict pairs possible in entirety of remaining Ty  where y != x.
If conflict pair possible we draw an edge.If any cycle or loop in schedule then it is NOT conflict Serializable. As we proceed in txn we cancel out the previous operations.

## View Serializability:
If conflict serializability fails txn can still be serializable by view serialisability.

For view serializability we just swap two operations between txns and then take sample output and see if it produces the same output.
If same output then txn is serializable.

# Concurrent Control

Lock is a mechanism to control concurrent access to a data item.
##### Locked Modes
1. Exclusive(X) - Both read and write
2. Shared(S) - Only read
Lock request are made to the concurrent-control manager of the program

Lock Compatibility Matrix
	S     X
S  true  false
X  false false

A lock is granted to txn only if the lock it already has from other txn is compatible with the lock being given to it.

A txn can have any number of Shared locks, but if a txn hold  exclusive lock no other txn can hold any lock on that item.

Locking Protocol -  Set of rules to be followed by all txns when requesting and releasing locks. This ensures conflict serializable schedules.

Phase 1: Growing phase 
Txn may obtain locks but may not release locks.
Phase 2: Shrinking phase
Txn may release locks but may not obtain locks

The protocol ensures serializability in order of their lock points.(Points where txn obtained it's final lock.)

After commit and abort all locks are removed

Two phase locking is required for conflict serializability.
Lock conversions:

Phase 1:
Lock-s
Lock-x
Upgrade of Lock-s to Lock-x

Phase 2:
Unlock-s
Unlock-x
Downgrade of Lock-x to Lock-s

**Reading**:

if T has lock-s:
	read(X)
else:
	Wait until no txn has Lock-x on X:
	grant X with Lock-s
	read(X)
end

**Writing**:
if T has lock-x:
	write(X)
else:
	Wait until no txn has any lock on X
	if T has lock-s:
		upgrade lock-s to lock-x
		write(X)
	else:
		grant X with Lock-x
		write(X)
end

Lock manager manages a DS called lock table in order to manage and grant txns with locks.

Timestamp based protocols:
Each txn gives a timestamp on entering system
if Ti older then Tj then TS(Ti) < TS(Tj)

Concurrent execution managed such that TS determine serialisability order.

**W-timestamp** : Largest timestamp that execute write successfully
**R-timestamp** : Largest timestamp that execute read successfully

Identify txn and if it rollbacks or commits:
	A B C ... (varibs)
RTS 0 0 0 ...
WTS 0 0 0 ...

if Ti does read(a) :
	if WTS(a) > TS(Ti):
		rollback Ti
	else:
		exec read(a)
		RTS(A) = max(RTS(A),TS(Ti))
if Ti does write(a):
	if RTS(A) > TS(Ti) OR WTS(A) > TS(Ti)
		rollback Ti
	else:
		WTS(A) = max(WTS(A),TS(Ti))

The timestamp ordering protocol guarentees serializability
since all the arcs in precedence graphs are of type:

	  TXN    ------>    TXN
	small TS           big TS
	
So no cycles or loops,this protocol offers freedom from deadlocks,but the txn will not be cascade-free or may not be recoverable.

Cascading rollback - Chain of rollbacks

Non recoverable ie lets say T2 takes value from T1 and T2 commits but T1 aborts the changes to T2 can't be roll-backed since it committed hence txn not recoverable. Furthermore any txn that read from T1 then even they have to abort causing a cascading rollback.

Solution:
- All writes at end of its processing
- All writes are atomic,no txn must execute while txn being written.
- Txn that aborts restarted with a new TS
- Limited form of locking,wait for data to be committed before reading.