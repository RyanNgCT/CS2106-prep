## A. Race Condition
> A **cooperating process** is one that can affect or be affected by other processes executing in the system.
- directly share a logical address space (shared memory) or are allowed to share data only through messages
- cannot have concurrent access to shared data by cooperating processes $\implies$ leads to **data inconsistency**

> **concurrency** is the technique used by processors (alongside CPU scheduling) that distributes computing resources amongst executable entities
- creates the illusion that the processes are all running at once

- however, when $\geq 2$ processes execute concurrently in *interleaving fashion* or share modifiable resources $\implies$ can cause synchronization problems
	- whereby processes are **no longer independent** and *require interaction*

- execution of a *single sequential process* is **deterministic in nature**, as the repeated execution yields the same result
	- each line of code **doesn't** always **correspond to only one instruction** when compiled ($\geq 1$ instruction per line of code)
	- most operations in source code, i.e. declaration $+$ assignment requires multiple CPU steps
	```c
	int c = a + b;
	```

- Race Conditions: execution of concurrent process **may not be deterministic** and the outcomes depends on the order which the shared resource is accessed or modified
	- $P_1$ and $P_2$ modify the same variable value

```c
X = X + 1000
```
1. `Load X` $\implies$ `register1`
2. `Add 1000` to the variable stored in `register1`
3. `Store register1` $\implies$ `X`

![sequential-execution](../assets/sequential-execution.png)

- execution of two concurrent in interleaving fashion
	- share modifiable resources
	- can cause synchronization problems
![race-condition](../assets/race-condition.png)
#### Solution
> **Synchronization** is the coordination among threads, usually regarding shared data
- incorrect execution is due to unsynchronized access to shared modifiable resource
- solution outline $\implies$ designate code segment *with race condition* as critical section and only one process can execute in the critical section
- in the *producer-consumer model,* both parties need to be synchronized s.t. consumer does not try to consumer an item that has yet to be produced
## B. Critical Section
> A **critical section** is code that exactly one thread can execute at once.

> A **lock** is an object that only one thread can hold at a time to *provide the mutual exclusion property.*
- implemented using a tree-based set data structure (use depth first search algorithm) $\implies$ with the `init` process as the root of the tree in Linux
- **`lock.acquire()`** $\textemdash$ wait until lock is free then mark as busy (function returning results in thread "holding" the lock)
	- other threads are put to sleep when they attempt to acquire a lock which is marked
- **`lock.release()`** $\textemdash$ mark the lock previously held by a thread as free

- implemented in the `pthread` library as `mutex`
#### Properties of correct Critical Section
- **Mutual Exclusion:** if process $P_i$ is executing in critical section, then all the process are prevented from entering the critical section
	- achieves the goal of ensuring only one thread does one thing at once
	
- **Progress:** if no process is in a critical section, then one of the waiting processes should not be granted access

- **Bounded Wait:** After process $P_i$ requests to enter critical section, then $\exists$ upper bound to \# times other processes can enter the critical section before $P_i$

- **Independence:** process not executing in critical section should never block other processes
#### Symptoms of Incorrect Synchronization
1. **Deadlock:** if all processes are blocked $\implies$ there is no progress
2. **Livelock**: is a deadlock avoidance mechanism, whereby processes keep changing their state to avoid deadlock, but *no progress is made*
3. **Starvation:** processes are blocked forever (from entering the running state and having a CPU burst)
## C. Implementations of Critical Sections
1. Assembly-level implementation by the processor
2. High-level language implementation which uses programming constructs
3. High-level abstractions which provide additional useful features and are commonly implemented by assembly level mechanisms

### C1. Assembly: Test and Set
- uses a machine instruction to aid synchronization of threads
```nasm
TestAndSet reg, [mem]
```
**Behaviour**
- Load current content of `[mem]` into `reg`
- store a `1` into `[mem]`

**Observations**
- works, but employs busy waiting (i.e. waste of processing power)
- variants of the `TestAndSet` instruction $\exists$ on other processors
	- compare and exchange
	- atomic swap
	- load link, store conditional

### C2. High-level language
- 

## D. Classical synchronization problems
