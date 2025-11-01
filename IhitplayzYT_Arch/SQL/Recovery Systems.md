Txn failure cause:
1. Logical: Internal error like program logic,etc.
2. System: Error like deadlock.

System crash: Power or hardware/software failure caused txn to terminate.

Fail-stop assumption: Non-volatile storage contents assumed to not be corrupted.

Disk Failure:Head or similar disk crash that destroys all or part of the storage. Destruction is assumed to be detectable using checksums and parity bits.


Recovery Algorithms steps:
1. Actions/steps done in txn
2. Actions to be taken afterwards recover DB contents to ensure ACID property.

#### Data access:
Physical Blocks - Blocks on disk
Buffer Blocks - Temporary blocks residing in main memory.
input(a) - Moves physical block B to main memory
output(a) - Moves main memory block B to physical memory


#### Log based Recovery:
Log is kept on stable storage
Log record is is sequence of Logs

When Txn starts `<Ti start>log record`
When write on X `<Ti,X,old_val,new_val>`
When end `<Ti commit>`

Two approaches based om Logs:
1. Deferred Database modification
2. Immediate Database modification

#### Immediate Database Modification:
Updates of uncommited txns made to buff or disk before txn commits.
Update log record before db is written to 

#### Deffered Database Modification:
Updates to the buffer/disk only at time of txn commit.

Txn said to be committed when the commit log is output to a stable storage.

In concurrent txns all the txns share a single disk buffer and log.

Log records of two or more txns might be interweaved in the log.

undo(Ti) - All values affected by Ti are restored to the their old values
 
redo(Ti) - Sets all variables affected by Ti to their new values going forward from the first log.

Txn needs to be undo if log of txn contains start but no commit or abort

Txn needs to be redo if log of txn contains start and also commit or abort.

If txn undoes and then redoes it called repeating history.

If both start and commit/abort present we do a redo
if only start there no ending tags we do an undo

Undo/Redo are time consuming hence to streamline we use checkpoints concept `<checkpoint L>` onto stable storage where L is list of txns active at time of checkpoint.

During recovery we consider the most recent txn before checkpoint and all txn after the checkpoint 

The operation that caused system failure need to be undo, the txn before failure causing ops and checkpoint need to be redone and any txns before checkpoint dont need to be touched.








