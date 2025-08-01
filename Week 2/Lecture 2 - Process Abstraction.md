## A. Intro to Process Management
### A1. Motivation
> A **process** is a program ==in execution==. 
- note that process $\neq$ program
- **program** is a *sequence of executable instructions (`.text`) and data (`.data`)* needed by the processor to accomplish a specific task (is a **passive** entity)
- memory layout of a process varies from that of its state as a passive program entity
- **process** is an ***active entity***

- OS is expected to have efficient use of the hardware resource (managing programs executing on hardware)
- how do we utilize hardware resources effectively if **only when program executes at a time** (no concurrency)
	- allow for multiple programs to *share* the *same hardware processes*
### A2. Properties
- Program Switching by the OS requires the following
	- information regarding execution of program A (to be stored)
	- replacing program A's info with that of program B

> A **process** requires an abstraction to describe a running program
##### Topics
1. *Process Abstraction* - info describing an executing program
2. *Process Scheduling* - deciding which process gets to execute, when
3. *Inter-process Communication & Synchronization* - passing info btwn processes
4. *Threads* - a.k.a. a lightweight process (which is an abstraction of the processor), or more specifically a lightweight unit of execution within a process itself
##### Components of Processes
Each process can have:
1. Address space
2. $\geq 1$ thread(s) of control in the address space as above
3. additional system *state associated* with the process
	- open files, sockets, windows (display or monitors) etc.
## B. Process Abstraction
- a process, task or job is a **dynamic abstraction** for executing programs
	- has information required to describe running programs
	- a process is a *kernel entity* that executes a program (i.e. each running program **runs within** its own process)
	- provides a nicer interface as compared with raw hardware itself 

- *Memory Context:* contains code and data
	- storage for instruction & data

- *Hardware Context:* contains register and program counter $PC$

- *OS Context:* contains process properties and the resources used

	![comp-orgn-block-diagram](../assets/comp-orgn-block-diagram.png)
### B1. Component Descriptions
- *Memory* (RAM)
	- storage for instruction and data (as above)

- *Cache*
	- is a duplicate of memory inside the CPU for faster access
	- usually split into *instruction cache* and *data cache*

- *Fetch Unit* (IF)
	- retrieves instructions and load into Instruction memory
	- location is indicated by the special register: *Program Counter* $PC$

- *Functional Units* (EX)
	- carry out the instruction execution
	- dedicated to different instruction type

- *Registers*
	- internal storage for the faster access speed
	- **General Purpose Register** ($GPR$): accessible by user program that is visible to the compiler
	- **Special Registers**
		- Program Counter ($PC$)
		- Stack Pointer ($SP$)
		- Frame Pointer ($FP$)
		- Program Status Word ($PSW$) $\implies$ status bits reflecting the current CPU state
### B2. Basic Instruction Execution
**Sequence**
1. Instruction $X$ fetched (with memory location indicated by $PC$)
2. Instruction $X$ dispatched to the corresponding functional unit
	1. read operands from memory or $GPR$
	2. results computed at ALU
	3. write result value if applicable to memory or $GPR$

3. Instruction $X$ completes execution and $PC$ updated for the next instruction

==**Components**==
- binary executable consists two major components: instructions and data
- information generated during program execution
	- **Memory Context**: *text, data, stack and heap*
		- there are many other types of memory usages as well
	- **Hardware Context**: $GPR$, $PC$, `$sp`, `$fp` etc.
	- **OS Context:** Process ID, Process State etc.
## C. Function Calls 
- need to decide how to allocate memory space for variables `i`, `j` and `a`?
	- can make use of the **data** memory space

	![c-code-diff](../assets/c-code-diff.png)
### C1. Issues
- Issues with **control flow**
	- need to jump to function body and resume when the function call is done
	- minimally, need to store the $PC$ of the caller

- Issues with **data storage**
	- need to pass parameters to the function 
	- need to capture the `return` result
	- have local variables declaration

**Steps**
1. setup the parameters (passed in from caller)
2. transfer control to the callee
3. set up local variables
4. store result where applicable
5. return to caller (return value and another stuff is provided as a service to the caller program)

```c
void f(int a, int b)
{ 
	int c;
	c = g(a, b);  // 1
	...
}

int g(int i, int j)
{
	int a;  // 3
	...
	return ...;  // 4
}
```

- require new memory region that is dynamically used by function calls
### C2. Intro to Stack Memory & Address Spaces
- new memory region store info for function invocation $\implies$ is a special area of memory used for the storage for **temporary values**
	- *last in first out* data structure
	- useful when we ***don't have enough registers*** for storing essential values, especially when calling other subroutines

- info of function invocation is described by a stack frame
	- `return` address
	- arguments passed to function
	- storage of local variables

- allows for more than $4$ parameters and $2$ return values to be stored at once

- contains *automatic variables* (upon the invocation of a function)

> The **Address Space** is the set of accessible addresses and the state associated with them
- usually in powers of $2$
- is the memory region that has been allocated to a particular process
##### Stack Pointer `$sp`
- points to the top of the stack region (the first unused location)
	- most CPU has a specialized register for this purpose
	- holds the **last thing** that we have pushed into the stack
	- is the memory address of the current push or pop operation

- memory accesses are done via `lw` and `sw` instructions
	- `$sp` is used as the base address for these memory-related instructions

- by convention, we move the stack pointer **before we storing data** with `sw` and **after loading data** with `lw`

**Push Instruction**
- recall that stack grows upwards towards *smaller addresses*, so need to decrement address stored inside stack pointer by a word (since $1$ word $= 32$ bits in MIPS)
- value stored in `$t4` is what we wish to store temporarily (before being retrieved using `lw`)
```python
addi $sp, $sp, -4   # 0xABCD -> 0xABC9
sw   $t4, 0($sp)
```

**Pop Instruction**
```python
lw $t4, 0($sp)
addi $sp, $sp, 4
```

**Operations**
- stack frame is added on (the top) or pushed when the function is invoked $\implies$ the stack grows taller* $\implies$ grows to higher memory addresses
- stack frame is removed (popped) when the function execution completes $\implies$ stack shrinks*
- note that the memory layout of some systems are flipped (stack on top, text at bottom)

	![stack-block-diagram](../assets/stack-block-diagram.png)

##### Stack Frame Diagram Version 1
- there are *different ways* to setup the stack frames
	- no universal way as it is hardware and programming language requirement
- stack frame is an organized section of the stack containing registers to be preserved by the procedure

![stack-frame-v1](../assets/stack-frame-v1.png)

##### Setup Process
1. **Prepare** to make a function call (Caller)
	- pass parameters with the registers and / or stack
	- save the return $PC$ in `$ra` $\implies$ where to point too after Callee completes execution
		- get back to old location as indicated by the $PC$ after procedure calls
		- update $PC$ value to new offset or label memory location
 
2. **Transfer control** from the caller **to the callee** (Callee)
	- save the the registers used by the callee function, old stack pointer and frame pointer
	- allocate space for local variables of the callee function on the stack itself
	- adjust $SP$ to point to the new stack
		- position of saved $SP$  is between parameters and the `return` $PC$

##### Teardown Process
1. **Return** from the function call (Callee)
	- place the *return result*  (ret value) onto the stack where applicable, unless we are returning `void`
	- restore to the saved Stack Pointer $SP$ value, Frame Pointer $FP$ and saved registers (see below)

2. **Transfer** the **control back to the caller** using the saved $PC$ value (Caller)
	- utilize the return result, where applicable (which is the result produced)
		- can use the MIPS instruction `jr $ra`, which updates $PC$ back to the old location, i.e. the location of the caller
	- continues execution from the caller function
	
		![stack-ptr-function-return](../assets/stack-ptr-function-return.png)
##### Other Information in Stack Frame
1. **Frame Pointer** `$fp`
	- used to access various stack frame items, since stack pointer is hard to use (inherently $\Delta$)
	
	- some processors provide the Frame Pointer as a dedicated register $\implies$ `$fp` usage is platform dependent

	- frame pointer points to a **fixed location** in a stack frame
		- other items are accessed using frame pointer as reference (**relative displacement** from the `$fp`)
		- is **static** within the *context of a single procedure call* (has one caller and one callee only)
		- points to the bottom of the **current stack frame**

2. **Saved Registers** 
	- *a.k.a. Callee-saved registers or register spilling*
	
	- since the general purpose registers of most processors are severely limited, can use memory to temporarily hold the register values
		- MIPS has $32$ GPRs and x86 has $16$ GPRs

	- can use GPRs for other purposes and then restored after the operation to be performed is completed
		- function can spill registers that it intends to use before the function is executed and then *restore* those registers after it completes
		- related to the scoping of variables, particularly in the context of **function-local variables** and **variable lifetimes**
		- used to maintain the **correctness** of programs
	
	- is located inside the *register file or the stack* and can be used by the compiler
##### Stack Frame Diagram Version 2
- *parameters* are to be **passed into** the callee function and *return value* are to be **passed out** of the callee function, back to the caller for further computation
	- the return values could be a modified version of the parameters (if any), or that the return value depend on the parameters supplied
	- can use `$ax` where $x \in \{1, 2, 3\}$ to store arguments or parameters, which should be assumed to stay the same (i.e. the callee function should **not** act on these) $\implies$ `$ax` are stable / don't change, while `$vx` could change (i.e. they are variable)
		- responsibility of saving value in `$ax` lies on the callee function

- data in the stack frame can be accessed relative to the values stored inside `$fp` or `$sp`
	- advantage of using the `$fp` instead is consistent static access (predictable)

**Stack frame for function `g()`**
- we can store the saved `$fp` value for function`f()`, so that we can reference it again later on
- also stores any saved registers that we want to reference upon returning to function `f()`
- allocation of space for local variables which callee function `g()` can use
![stack-frame-v2](../assets/stack-frame-v2.png)

## D. Dynamically Allocated Memory
- most programming languages support **dynamically allocated memory** $\implies$ the allocation of memory during execution or run time
	- usually starts of with *less physical memory* than what the program actually ultimately requires $\implies$ page faults allocates stuff onto the heap
	- can be allocated dynamically through the **use of system calls** (request for memory through syscalls), which takes up resources and incurs a significant **performance penalty**
		- creates the **problem of external fragmentation** (whereby $\not \exists$ a contiguous space in the heap to store a large array, for instance)
		- problem can be solved using data structures, such as a linked list, but creates the problem of cache misses (recall from CS2100)

- Examples
	- `malloc()` function call in C
	- `new` keyword in Java / C++ to initialize 
#### Motivation
- cannot use **data** or **stack** memory because of
	- allocation of memory only at runtime, no static memory address location $\implies$ **cannot place** into **data memory** region
	- if we have no definite deallocation timing $\implies$ cannot place in stack memory region
	- memory is **too large** to be meaningfully stored onto the stack

- require a separate space to store these dynamically allocated items, contains the "data" maintained by `malloc(n)` and `free(ptr)`
	- is **shared** amongst all threads, shared libraries and DLL modules
	- `malloc()` takes parameter of size `n` (ensures the block to store the data is at least `n` units long)
	- `free()` takes in a pointer as parameter to memory region to be cleared $\implies$ mark region as free

	![memory-space-model](../assets/memory-space-model.png)

#### Managing Heap Memory
- harder, given the variable size of data that is stored inside the heap
	- allocations and deallocation requires the searching for available / occupied sub-regions in memory, which is time consuming

- there is a variable allocation and deallocation timings, as compared with stack and data memory
	- must be explicitly managed using `malloc()` and `free()` calls (instead of manual push and pop in the stack when a function is invoked / returns)

- $\exists$ a tricky scenario where heap memory is (de)allocated to create holes $\implies$ underutilization as we are not using contiguous chunks of memory
	- free memory could be squeezed in between occupied memory blocks

- harder to manage the data stored on the heap itself because data is not always compacted (unlike the stack) and is unpredictably stored $\implies$ has **no ordering** or organization
## E. Processes 
> A **process** is a program in the state of execution.
### E1. Process ID (PID)
- used to distinguish processes, to be a unique identifier

- some OS dependent issues
	- PID reuse (after a lease expires?)
	- is there a limited to the maximum no. of processes
	- reserved PIDs
### E2. Process State
> The **process state** indicates the status of execution of a given process

- in multitasking scenario, processes can have two states -- running or not running
- but process can be "ready to run", but is **not actually executing**
##### Process Model State Diagram
- the *set of states and transitions* are known as the process model
	- CPUs usually alternates between CPU execution and I/O wait states $\implies$ CPU burst $\iff$ I/O burst
	- the final CPU burst ends with a system request to terminate execution
![process-state-model](../assets/process-state-model.png)
1. **New:** new process created, may still be under initialization
2. **Ready:** the process is *waiting to run*
3. **Running:** the process is *being executed* on the CPU itself
4. **Blocked:** a process is waiting or sleeping for an event, until the event is available
	- example: I/O operations, awaiting input or syscalls
5. **Terminated:** process has finished execution but may require OS clean-up
##### State Transitions
1. **Create** (NIL $\to$ New): new process creation
2. **Admit** (New $\to$ Ready): process is ready to be scheduled for running
3. **Switch - Scheduled** (Ready $\to$ Running): process selected to run (from waiting state)
4. **Switch - Release CPU** (Running $\to$ Ready): process gives up CPU time voluntarily or pre-empted by the scheduler
5. **Event wait** (Running $\to$ Blocked): process requests for event that is not available or still in progress
	- system call or awaiting for I/O
6. **Event occurs** (Blocked $\to$ Ready): process can continue execution
	- each process may be in different states, with each process in different part of the state diagram
##### View of Process States
Given $n$ processes
- with $1$ CPU, $\exists \leq 1$ process in the running state, with conceptually $1$ transition at a time
- with $m$ CPUs, $\exists \leq m$ processes in the running state, with possible for parallel transitions
##### State Transitions
- $\geq 1$ process can be in the ready and blocked queues at once
- we can have separate event queues
![process-state-model-alt](../assets/process-state-model-alt.png)
### E3. Process Control Block (PCB)
- PCB contains the entire execution context for a process, which contains the Program Counter $PC$
- the kernel maintains the PCB for each individual process
	- conceptually stored as a single table for all processes on a machine

- when each process executes the `fork()` system call
##### Components
- each PCB has individual elements such as the $PC$, registers, stack etc. (refers to each process)
![process-control-block](../assets/process-control-block.png)
##### Issues
- scalability: how many concurrent processes can we have
- efficiency: how do we provide efficient access to each block, with minimal space wastage

	![process-table](../assets/process-table.png)
## F. Process interaction with Operating Systems
- done through system calls, exceptions and interrupt handlers
### F1. System Calls
- are essentially an API call to the OS, which provides a way of calling facilities or services provided by the kernel
- is **not the same** as a normal function call as have to go from user to kernel modes

- different OSes have different APIs
	- **UNIX variants:** follow the POSIX standards
	- **Windows variants:** uses the Windows API across different Windows versions
		- usually has a huge number of API calls

- system calls can be involved almost directly *as long as*:
	- **library version has the same name** and same parameters (acts as a function wrapper)
	- **user-friendly library version** which acts as a **function adapter**
	
- included via the `unistd.h` header file, which contains the POSIX APIs
	- based on the example below, `getpid()`, `write()` which is made by `printf()` are syscalls
```c
#include <unistd.h>
#include <stdio.h>
int main()
{
  int pid;
  pid = getpid();

  printf("process id = %d\n", pid);
  return 0;
}
```
### F2. Syscall Mechanisms
1. User program invokes library call, using a normal function call mechanism
2. Library call places the system call number in a designated location (i.e. in a register, usually in argument register 0 or `$a0`)
3. Library calls executes special instruction to switch from user mode to kernel mode (i.e. `TRAP`)
4. Call the appropriate system call handler in kernel mode
	- use the system call number as index
	- this is handled by the dispatcher

5. System call handler is executed (fulfilment of the request)
6. System call handler ends, which returns control to the library call
	- switching from kernel back to user mod happens here

7. Library call return to user program (via normal function return mechanism)
![syscall-mechanism-illustrated](../assets/syscall-mechanism-illustrated.png)
### F3. Exceptions
- could occur when executing a machine-level instruction
- exceptions are synchronous and occur due to program execution

**Examples**
1. Arithmetic Errors $\to$ Overflow, Underflow, Zero Division
2. Memory Access Errors $\to$ illegal memory address, misaligned memory access (i.e. a word not aligned to a word-sized boundary)

**Effects**
- have to execute exception handler, which is like a forced function call $\implies$ control should be transferred to the handler automatically
### F4. Interrupts
- external events (not the logic) that could interrupt the execution of programs
- usually hardware related (i.e. timer, mouse movements, key pressed)
- are asynchronous, which means that these events are **independent of program execution**

**Effects**
- program execution is suspended
- have to execute an interrupt handler
### F5. Handling Exceptions & Interrupts
- once an exception or interrupt occurs, we need to transfer control to handler immediately
- return from handler routine once complete
	- program execution can resume after that
	- may be possible for original program to behave as if nothing happened

```c
void handler() {
	// 1. Save the registers and CPU state
	
	// 2. Perform the handler subroutine

	// 3. Restore the registers and CPU state

	// 4. Return from the interrupt or exception
}
```
## G. Process Abstraction in Unix
Has the following process stages:
1. Identification
	- use of Process ID (`int` value to identify)

2. Information
	- contains things like process state (i.e. Running, Sleeping, Stopped, Zombie), PPID, Cumulative CPU time etc.

3. Creation
	- can create processes using the `fork()` method $\implies$ more below

4. Termination
5. Parent-Child Synchronization
## H. Process Creation 
### H1. using `fork()`
- main way to create new processes in Unix / Linux
	- returns **`PID` of the newly created process** in the parent process (the caller of `fork()`) $\implies$ return value 0
	- returns `PID` if $0$ in the child process, but the child's actual PID is nonzero
	```c
	#include <unistd.h>
	#include <sys/types.h>
	
	pid_t fork(void);
	```

- header files included are system dependent $\implies$ can use `man fork` to locate the appropriate system files that are required

- allows for multiple processes to be handled independently from one another (non-dependent on each other)

- creates new processes known as child process, which is a **duplicate** of the current executable image
	- has the same code, same address space, same state
	- but the *data* in the child process is a copy that is **not shared** and the program counter contents are **not the same** (it is an image)
	- whatever attributes that the parent has, the child also has

**Example 1**
- child process differs in terms of `PID`, `PPID` and the return value of the `fork()` method
	```c
	#include <stdio.h>
	#include <sys/types.h>
	#include <unistd.h>
	
	int main() {
	    printf("I am ONE\n");
	    fork();
	    printf("I am seeing DOUBLE\n");
	
	    return 0;
	}

	// output
	./fork0
	I am ONE
	I am seeing DOUBLE
	I am seeing DOUBLE // output gets printed twice due to  forked processes
	```

- both the parent and child processes continue executing after `fork()` is invoked
- common usage to use the parent and child processes differently
	- parent spawns child to carry out some work, then parent can take on another order

- can use the **return value of `fork()`** method to distinguish between parent and child processes

- `fork()` creates some independent memory space

**Example 2**
```c
int main() {
	pid_t pid;
	int x = 1;
	
	pid = fork();

	if (pid == 0) {
		printf("Child: x = %d\n", ++x);
		exit(0);
	}
	printf("Parent: x = %d\n", --x);
	exit(0);
	
	return 0;
}
```

**Example 2 Diagrams**
![example2-parent-child](../assets/example2-parent-child.png)

**Example 4**
![example4-parent-child](../assets/example4-parent-child.png)

##### Executing new programs
- need to provide the full code for the child process spawned off by `fork()`
	- technically cannot execute another existing program? $\implies$ utilize the `exec()` command instead
	- has variants: `execv()`, `execl()`\*, `execle()` etc.

##### Command-line arguments
- pass arguments to a C program using command line arguments
	- `argc`: \# of cmd line args including the program name itself
	- Argument Vector (`argv`): a character (string) array and each element within `argv` is a C character string

```c
int main(int argc, char* argv) {
	int i;
	
    for (i = 0; i < argc; i++){
        printf("Arg %i: %s\n",i, argv[i] );
    }
    return 0;
}
```
### H2. `execl()` system call
- **replace** the **existing** process image with **new one**
	- does replacement of the PID and *other info still intact* 
	- image is **replaced** (i.e. the code, data, heap and stack) $\implies$ hence also requires a change in the process control block as well

- executes the program

- arguments
	- `path`: the location, or the full path of the executable (whereas `execlp()` only requires the filename as an argument)
	- `NULL`: indicates the end of the arg list
```c
#include <unistd.h>
int execl(
	const char *path,
	const char *arg0,
	...,
	const char *argN,
	NULL
)
```

**Example usage**
- need to specify the program name and path as `arg0`, then the subsequent ones are the command to execute
```c
#include <unistd.h>
execl(
	"/bin/ls",
	"ls",
	"-l",
	NULL
);
```

**Example for `execlp()`**
```c
execlp("date", "date", NULL);
```
### H3. Combining `fork()` and `exec()`
- properties when we combine the two mechanisms:
	- spawn off a **child process** (performing a task through `exec()`)
		- child process can then overlay its image with a different program
	- can have the parent process is still around to accept a new request

- used to get a new process running for a new program

- each process has a *common ancestor* $\implies$ `init` process on Linux, which is created by the kernel at boot and has PID of `1`
	- `fork()` creates the process tree
## I. Process Termination
- to end the execution of a process
- status *returned to parent process* later
	- $0 \implies$ normal successful execution and termination of the program
	- non-zero $\implies$ problematic execution

- function **does not return**

- process finish execution and then **most system resources** used by process are released on exit
	- ***file descriptors***, which each opened file has, is released
	- some resources are not releasable
		- PID
		- status required (for parent-child synchronization)
		- process accounting info like CPU time

		![process-list](../assets/process-list.png)
### I1. Implicit `exit()`
- most programs don't have an `exit()` 
- a return from the `main()` program implicitly calls `exit()` $\implies$ any open files gets flushed
### I2. Parent/Child Synchronization
- parent process can use the `wait()` method to wait for child processes to terminate
	- returns PID of the terminated child process and the exit status
	- can use `NULL` if we don't need the info
```c
int wait(int *status);
```

- is a **blocking call**, where the parent process is blocked *until at least one child terminates*
	- the call cleans up remainder of the child system resource $\implies$ rid child processes *not removed* on `exit()` and zombie process

- Variants of `wait()`
	- `waitpid()` $\implies$ wait for specific child process
	- `waitid()` $\implies$ wait for any child process to **change status**
		![process-interaction-unix](../assets/process-interaction-unix.png)

### I3. Creating Zombie processes
- on process exit, a child process becomes a zombie
- cannot delete *all* process info
	- what if parent asks for info in a `wait()` call
	- remainder of the process data structure can be cleaned up only when `wait()` occurs

- cannot kill zombie process since it is already dead
###### Case 1: Parent process terminates before child
- `init` process becomes a "pseudo" parent of child processes (is not the actual parent, delegate to ancestor)
- child terminates sends signals to `init` and uses `wait()` for clean-up

###### Case 2: Child process terminates before the parent but parent didn't call `wait()`
- child process becomes a zombie
- can fill up process table but requires a reboot to clear the program on older Unix boxes
## J. Summary of process system calls
1. **`fork()`:** process creation
2. **`exec()` family:** change the executing image or program
3. **`exit()`:** process termination
4. **`wait()` family:** get the exit status and synchronize with child process, in the "suspended" state
5. **`getpid()` family:** obtain process information
	![process-state-diagram-unix](../assets/process-state-diagram-unix.png)
## K. `fork()` implementation
- intended behaviour is to "clone" its parent process

**Implementation**
1. Create the address space of the child process

2. Allocate $p'$, which is the new PID

3. Create kernel process data structures

4. Copy the kernel environment of the parent

5. Initialize the child process context
	- `PID` = $p'$ , `PPID`, zero CPU time (?)

6. Copy the memory regions from the parent via `memcpy()`$\implies$ *program, data and stack* of the program
	- is a very expensive operation that can be optimized
	- *Observations*: child process will not access the whole memory range right away
		- **child process read**: location remains unchanged and use a shared version
		- **write to location:** two independent copies required

	- *Optimization:* Copy on Write
		- only duplicates the memory location by child process **when write is required**
		- otherwise, parent and child processes share the same memory location

	- *Properties of Memory*
		- organized into memory pages, which are contiguous sets of memory locations
		- memory is managed in pages instead of individual locations, which is $4kB=4096B$

7. Acquires shared resources such as opened files and the current working directory

8. Initialize hardware context for child by copying registers and other required data from the parent process

9. Child process is now ready to run (can be added to the scheduler queue)

**Modern Take on `fork()`**
- is a system call part of the Unix design that is inherited by most \*nix variants
- not versatile as can only duplicate process fully and **not partially**

- can use `clone()` as an alternative to `fork()`
## L. Protection Mechanisms*
- OS helps to block processes from other processes (i.e. cannot read / write into storage or memory by the other process)
	- isolates processes from each other
	- results in segmentation fault in case of violation
	
- programmer does not need to worry about access violations etc.

- OS synthesizes a protection boundary which
	- protects process running on top of the hardware virtualization layer
	- prevent incorrect behaviour

Program Address fits into some valid part of the program
- We conduct some checks regarding the **physical addresses** (i.e. `base_addr`$\leq$ `prog_addr` and `prog_addr` $\lt$ `bound_addr`) and only allow program execution when it fulfils the checks above
	- addresses are translated only when the program is loaded
	- use the $PC$ to determine the program start address (i.e. `prog_addr`)
	- performs the operation of **Address Space Translation**

