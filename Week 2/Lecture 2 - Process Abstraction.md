## A. Intro to Process Management
### A1. Motivation
- OS is expected to have efficient use of the hardware resource (managing programs executing on hardware)
- how do we utilize hardware resources effectively if **only when program executes at a time** (no concurrency)
	- allow for multiple programs to *share* the *same hardware processes*
### A2. Properties
- Program switching by the OS requires the following
	- information regarding execution of program A (to be stored)
	- replacing program A's info with that of program B

> A **process** requires an abstraction to describe a running program

**Topics**
1. *Process Abstraction* - info describing an executing program
2. *Process Scheduling* - deciding which process gets to execute, when
3. *Inter-process Communication & Synchronization* - passing info btwn processes
4. *Threads* - a.k.a. a lightweight process
## B. Process Abstraction
- a process, task or job is a dynamic abstraction for executing programs
	- has information required to describe running programs

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

**Components**
- binary executable consists two major components: instructions and data
- information generated during program execution
	- **Memory Context**: text & data
		- there are many other types of memory usages as well
	- **Hardware Context**: $GPR$, $PC$
## C. Function Calls
- need to decide how to allocate memory space for variables `i`, `j` and `a`?
	- can make use of the **data** memory space

	![c-code-diff](../assets/c-code-diff.png)
### C1. Issues
- control flow
	- need to jump to function body and resume when the function call is done
	- minimally, need to store the $PC$ of the caller

- data storage
	- need to pass parameters to the function 
	- need to capture the `return` result
	- have local variables declaration

**Steps**
1. setup the parameters (passed in from callee)
2. transfer control to the callee
3. set up local variables
4. store result where applicable
5. return to caller

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
### C2. Intro to Stack Memory
- new memory region store info for function invocation
- info of function invocation is described by a stack frame
	- `return` address
	- arguments passed to function
	- storage of local variables

##### Stack Pointer
- points to the top of the stack region (the first unused location)
	- most CPU has a specialized register for this purpose

**Operations**
- stack frame is added on (the top) or pushed when the function is invoked $\implies$ the stack grows taller* $\implies$ grows to higher memory addresses
- stack frame is removed (popped) when the function execution completes $\implies$ stack shrinks*
- note that the memory layout of some systems are flipped (stack on top, text at bottom)

	![stack-block-diagram](../assets/stack-block-diagram.png)

**Stack Frame Diagram V1**
- there are different ways to setup the stack frames
- there is no universal way as it is hardware and programming language requirement

![stack-frame-v1](../assets/stack-frame-v1.png)

**Setup**
1. Prepare to make a function call
	- pass  parameters with the register and / or stack
	- save the return $PC$, updated to be $PC + 4$ to the stack
 
2. Transfer control from the caller to the callee
	- save the old stack pointer
	- allocate space for local variables of the callee function on the stack itself
	- adjust $SP$ to point to the new stack
		- position of saved $SP$  is between parameters and the `return` $PC$

## D. OS interaction w process
