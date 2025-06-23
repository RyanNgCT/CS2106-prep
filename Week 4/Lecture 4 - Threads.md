## A. Motivation & basic ideas
- using processes **are expensive** as they require:
	- creation in the `fork()` model $\implies \exists$ duplicate memory space and process context
	- context switching is required for the saving and restoration of process information

- communications between processes (IPC) $\implies$ no easy way of information passing and sharing due to processes occupying independent memory spaces

- threads were invented to *overcome the problems with using processes* as a "quick hack"

- main idea is to add in more threads of control to the same process so that multiple parts of the program could be executing at the same time, conceptually

- A *Single threaded machine* goes through the execution of code and functions sequentially
	- useful to instead execute multiple non-dependent functions at the same time (provided they are not reliant on each other's results)

### A1. Multithreading
> A **multithreaded process** is a single process can have multiple threads
- threads within the same process shares
	- **memory context:** text, data, heap
	- **OS context:** `pid`, other resources like files

**Unique information required by each thread**
1. Identification (`tid`)
2. Registers ($GPR$s and Special ones as well)
3. Stack

![process-n-thread-illustration](../assets/process-n-thread-illustration.png)

### A2. Benefits of using Threads
1. **Economical** $\implies$ requires much fewer resources to manage as compared to multiple processes
2. **Resource Sharing** $\implies$ the threads share the most of the resources of a process, but we don't need an additional mechanism for info sharing
3. **Responsiveness** $\implies$ multithreaded programs can appear much more responsive
4. **Scalability** $\implies$ multithreaded programs can take advantage of multiple CPUs

### A3. Problems with using Threads
1. System call concurrency
	- parallel execution of $\geq 1$ threads $\implies$ can perform parallel system calls

2. Process behaviour
	- impact on process operations
	- `fork()` a duplicate process or thread
	- if a single thread executes `exit()`, what would happen to the entire process?
	- if a single thread calls `exec()`, how about threads?

## B. Thread Models

### B1. Kernel versus User Thread Models
#### I. User thread
- threads are implemented as a user library
	- a runtime system in the process will handled thread-related operations
	
- kernel does not need to be aware of threads within the process

**Advantages**
- can have multithread program on any OS
- thread operations are just library calls
- user thread are more configurable and flexible $\implies$ customized thread scheduling policy

**Disadvantages**
- OS is unaware of threads, scheduling is performing at process level
	- one thread blocks results in the process being blocked and all threads within the process being blocked
	- cannot exploit the use of multiple CPUs

	![user-threads](../assets/user-threads.png)
#### II. Kernel thread
- threads are implemented in OS
	- thread operation is handled as system calls

- thread-level scheduling is possible because kernel schedules by threads and not processes

- kernel may make use of threads for its own execution

**Advantages**
- kernel can schedule on thread levels $\implies \geq 1$ thread in the same process can run simultaneously on multiple CPUs

**Disadvantages**
- thread operations are a system call so they are slower and more resource intensive
- generally less flexible
	- used by all multithreaded programs
	- if *implemented with many features*, can be expensive (and is an overkill for simpler programs)
	- if *implemented with few features*, may be inflexible for some programs

![kernel-thread](../assets/kernel-thread.png)

### B2. Hybrid Model
- has **both** user and kernel threads
	- OS schedule on kernel threads only
	- user thread binds to a kernel thread

- offers great flexibility as it can limit the concurrency of any process

	![hybrid-thread-model](../assets/hybrid-thread-model.png)

- threads started off as a software mechanism $\implies$ user space library

> **Simultaneous multi-threading:** hardware support $\exists$ on modern processes, such as the set of general purpose registers to allow threads to run natively and in parallel on the same core

![hybrid-thread-solaris](../assets/hybrid-thread-solaris.png)

## C. Threads in Unix
- `pthread` is a standard defined by IEEE to support most Unix variants
- IEEE standard defines the behaviour, but not the actual implementation $\implies \therefore \:$ `pthread` can implemented as user or kernel thread
- will show a few examples to highlight the differences between process & thread only
### C1. Example usage
```c
#include <pthread.h>

int main() {
	pthread_t threadId;            // thread id
	pthread_attr_t threadAttrib1;    // attributes
	...
}
```

**Compilation**
```bash
gcc <file>.c -lpthread
```
### C2. Creation
```c
int pthread_create(
       pthread_t  *tidCreated,  // thread id
       const pthread_attr_t *threadAttributes,   // attributes     
       void* (*startRoutine) (void*),   // function ptr to funct to be exec
       void *argForStartRoutine  // arguments for the startRoutine funct
);
```
- returns the value of `0` for success, returns non-zero if got errors
### C3. Termination
```c
void pthread_exit( void* exitValue );
```
- used to terminate a thread, if not the `pthread` will automatically when end of `startRoutine` is reached $\implies$ no exit value
### C4. Creation + Termination Example
```c
#include <stdio.h>
#include <pthread.h>
void* sayHello(void* arg)
{
     printf("Just to say hello!\n");
     pthread_exit( NULL );
}

int main()
{
    pthread_t tid;
    pthread_create( &tid, NULL, sayHello, NULL );
    printf("Thread created with tid %lu\n", tid);
    return 0;
}
```
### C5. Process Synchronization
- can use `pthread_join()` waits for the termination of another `pthread`
```c
int pthread_join( pthread_t threadID, void **status);
```
- returns the value of `0` for success, returns non-zero if got error
- `**status` is the exit value returned by the target `pthread`

### C6. Other features
- yielding by giving up CPU voluntarily
- advanced synchronization
- scheduling policies
- binding to kernel threads