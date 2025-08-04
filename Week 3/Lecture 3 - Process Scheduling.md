## A. Concurrent Execution
> **Parallelism** is the simultaneous execution of multiple tasks or subtasks by multiple processing units, such as multi-core processors or multiple CPUs, to improve performance and efficiency.

- concept for multi-tasked processes
	- virtual or physical parallelism $\implies$ assume both of them are the same in this context

| Concurrency via Time-slicing              | Interleaved Execution via context switching           |
| ----------------------------------------- | ----------------------------------------------------- |
| ![timeslicing](../assets/timeslicing.png) | ![context-switching](../assets/context-switching.png) |
##### Types
- $\mathbf{1}$ **CPU** is time-sliced for the execution of $n$ tasks (only one process can run at a time)
- **Multiprocessor:** the timeslicing occurs on $m$ CPUs
## B. Process Scheduling
- **The scheduling problem:** having $n$ tasks or processes and only $m$ CPUs occur when $n \gt m$
	- same concept can be applied on the thread level as would be the same

- **scheduling** is the act of assigning some time for doing a particular task and is a technique used by the OS to determine which task or process gets to use the CPU at a given time, based on *some specific criteria*
	- **CPU utilization** $\implies \%$ of time whereby the CPU is *actively executing processes* (should be doing useful work)
	- **Throughput** $\implies \#$ processes completed *per time unit*
	- **Turnaround Time** (not always the best metric) $\implies$ total time taken from process creation to completion ($\sum t_{\text{ready queue}} + \sum t_{\text{blocked}} + \sum t_{\text{running on the CPU}}$ )
	- **Wait Time** $\implies$ time that a process spends on the queue to wait for the CPU (Ready State)
	- **Response Time** $\implies$ time from submission of a request to when the first response or output has been produced $\neq$ total time

- scheduling is the **basis** of multi-programmed OSes $\implies$ makes the computer more productive

- A typical process goes through the *phases* of:
	- **CPU Activity:** computation like number crunching (will definitely be the first time lapse in any given process execution)
	- **I/O Operations**: requesting and receiving services from I/O devices
		- majority of the time is spent as I/O bound process
	- a.k.a. *CPU and I/O bursts* $\implies$ **time lapses** taken to complete CPU or I/O tasks respectively

- Most processes have many short CPU bursts and only a few long CPU bursts $\implies$ scheduling algo needs to account for long I/O burst (waiting or idle time)
	- CPU should be allocated to other process that isn't waiting for I/O operation during the long idle periods to ensure CPU is **effectively utilized**
#### IO and CPU bound
![IO-CPU-bound](../assets/IO-CPU-bound.png)
#### Key Components - Scheduler & Dispatcher
> **Scheduler** determines which process should use the CPU next, through the use of scheduling policy to manage the ready queue.
- Selection process is carried out by the (short-term) **CPU scheduler** in the OS when the CPU becomes idle
	- scheduler also needs to *allocate CPU to the chosen process* to execute at the specific time slot (based on "timer ticks")

> The **dispatcher**\* is the module that gives the control of the CPU to the process selected by the CPU scheduler
- has to be *very quick in nature* because there are lots of processes in the CPU (requires lots of process switching occurring) $\implies$ however **dispatch latency** (time needed to stop one process and start another is inevitable (see below), though usually not illustrated
- gives control of the CPU to the process at the head of the ready queue
- highly optimized component of the OS

> **Dispatch latency**\* is the time taken for the dispatcher to stop one process and start another
### B1. Definitions & Terminology
> **Scheduler** is the part of the OS that makes the scheduling decision

> **Scheduling Algorithm:** self-explanatory (selects the process to run and allocate the CPU)
- each process has different requirements of the CPU time that it requires, based on **process behaviour**
	- process environments are considered by the scheduling algos
### B2. Types of processing environments
1. *Batch processing* $\to$ **no user interaction required** and no need to be responsive
2. *Interactive* $\to$ with active user interacting w system, should be responsive and consistent with response time
3. *Real-time processing* $\to$ has a deadline to meet and usually takes the form of a periodic process
### B3. Criteria
- many criteria for evaluating scheduling algorithms
- criteria for all processing environments
	- **Fairness:** should get a fair share of CPU time (per process or per user basis) and also means no process starvation
	- **Balance:** all parts of the computing system should be utilized
### B4. Circumstances
![process-state-model](../assets/process-state-model.png)
1. **Event Wait:** Process $P$ switches from *running to blocked state* (i.e. waiting for the CPU, usually I/O burst)
2. **Release CPU / Interrupt:** Process $P$ switches from *running to ready state* (i.e. when interrupt occurs) $\implies$ process is enqueued to ready queue due to scheduling policy
3. **Event Occurs**: Process $P$ switches from *blocked to ready state* (i.e. completion of I/O operation)
4. **Process Termination (exit):** Process $P$ calls `exit()`, CPU is reallocated to another process
	- may not be possible for web server processes which are designed to constantly alternate between ready, running and blocked/waiting
![process-state-model-alt](../assets/process-state-model-alt.png)

Scheduling options are available for (2) and (3). 

**Alternate Diagram**
- process in other states other than running are technically "waiting" as well in some form of queue
- queues are used to manage the execution order of processes
- interrupt is not illustrated below (i.e. Running $\to$ Ready)

	![process-state-model-alt2](../assets/process-state-model-alt2.png)

**Usage of the dispatcher is shown below**
- allocate the CPU to the first process in the *ready queue* $\implies$ i.e. perform the context switch
- PCB structure of a given process is saved in the ready queue and this contains the CPU state (inclusive of registers and $PC$) which can be restored once the process to resume execution in the running state
- if supported, dispatcher is also responsible for switching back to user mode before handler control to the process

	![dispatcher](../assets/dispatcher.png)
### B5. Policies
These are the ways that CPU scheduling can occur.
1. **Non-preemptive / Cooperative**
	- process currently in the running state stays as such until it *blocks* or *gives up CPU voluntarily* (process cannot be interrupted)
	- usually has less overhead as compared to preemptive ones

2. **Preemptive**
	- process is provided a time quota to run, but it can **block** or ==choose to **give up**== the time slice early (process can be interrupted)
	- at the end of the quota, *another process* gets selected where available and the running process is suspended
	- Given that process $A$ which has a longer remaining CPU burst than process $B \implies$ priority given for process $B$

The order of process execution will change depending on which of the above is used.
### B6. Steps
- scheduler is triggered and the OS takes over
- if context switch is required, then the current running process context is saved and placed on the blocked or ready queue
- pick suitable process $P$ to run the base scheduling algo
- setup the context for $P$
- process $P$ runs
## C. Processing Environments
### C1. Batch Processing System
#### Properties
- No user interaction
- Non-preemptive scheduling is predominant
- easier to understand and implement 
#### Criteria (see above for the full list)
- $\text{Turnaround Time} = \text{End Time} - \text{Start Time}$ (a.k.a. the total time taken, which is related to the waiting time for the CPU, which is $t_{\text{completion}} - t_{\text{arrival}}$)
- Throughput $=$ no. of tasks completed *per unit time*
- CPU Utilization $= \%$ of time when CPU is working on a task
#### Examples
##### 1. First Come First Served (FCFS)
- CPU is allocated to the first process that requests for it
	- "CPU is allocated to process $x$" $\implies PC$ value is at the `.text` section  of the process, which enables the CPU to execute instructions
	- any form of *context switching* means that the $PC$ value is updated to a new section 

- process itself is not pushed into the FIFO queue (since they are not discrete data structures) but rather, the **Process Control Block structures** of each process are pushed into this queue

- guaranteed to have no starvation as \# tasks in front of task $x$ in FIFO queue is ***always decreasing***
	- task $x$ which is in the queue, will eventually get its CPU time
	- this condition is guaranteed based on the arrival order
	
- Convoy Effect may require simple reordering (usually because of multiple I/O bound tasks and 1 or 2 slow CPU bound tasks)
	- I/O bound tasks sit idle in the ready queue while waiting for CPU-bound process to complete $\implies$ let the tasks that require shorter CPU burst (time in the CPU) go first

- problems occur when we have processes that are *designed to run indefinitely*, without termination $\implies$ results in delays
	- i.e. using `syscall` for I/O operation waits etc.

- manages the next CPU burst
	![fcfs-processing](../assets/fcfs-processing.png)
##### 2. Shortest Job First (SJF)
- selects the task with the **smallest total CPU time** as the *first to be processed* (smallest next CPU burst)

- **total CPU execution time** is a need to know
	- can be *estimated* based on previous CPU-bound phases (impossible to determine for sure how long any task will take)
	- if two processes have the same burst length, then FCFS is used as the tie-breaker
	
- implementation is done using a priority queue, allows processes with shorter burst to overtake that of longer CPU burst
	- single priority scheduling scheme

- provides a fixed set of tasks to **minimize the average waiting time** (reduce queueing of processes)
	- enables for the minimum average waiting time per process
	- impossible to *implement perfectly*

- **lowest** average response times $\implies$ needed for interactive programs

- up to *scheduler policy* to decide whether to interrupt process that takes longer to complete and swap out for new I/O bound process that has arrived in ready queue
	- preemptive or non-preemptive

	![SJF-illustration](../assets/SJF-illustration.png)
- let $P_1, \ldots, P_4$ overtake $P_5$ whenever they are in the ready queue

> **Starvation** occurs when the ==*scheduling policy* causes a process to wait *indefinitely*== in the ready queue (i.e. cannot go to running state)
- usually occurs to process that is CPU-bound and requires lots of time in the CPU (high CPU burst)
- is a consequence of the scheduling policy itself
##### Exponentially Weighted Moving Average 
- a.k.a. exponential average
	- $\alpha = 0 \implies$ prediction depends on past events, *excluding* the most recent event $n$ (i.e. $[0 \:, n-1]$)
	- $\alpha = 1 \implies$ prediction depends solely on most recent event (only $n$), ignore the reality of previous predictions
	- common to make $0 \lt \alpha \lt 1$
$$
	\begin{aligned}
	\textbf{Predicted}_{n + 1} &= \left(\alpha \times \textbf{Actual}_n \right)+ \left((1 - \alpha) \times \textbf{Predicted}_n \right) \\\\
	\textit{where } n &= \text{the most recent CPU burst,} \\
	\text{Actual}_n &= \text{most recent CPU time consumed / observed,} \\
	\text{Predicted}_n &= \text{past history of CPU time consumed,} \\
	\text{Predicted}_{n+1} &= \text{latest CPU burst (i.e. \textbf{prediction}),} \\
	\alpha &= \text{weight on recent event, where we assume this value} < 1
	\end{aligned}
	$$
	
- does not hold up well when we have long CPU burst and then followed directly by a shorter one
##### 3. Shortest Remaining Time Next (SRTN)
- is a variant of **SJF**, but we use the remaining time and is **preemptive**
- selection of the job with the shortest (expected) time *remaining*

- new job with shorter remaining time can be used to preempt the current running job
	- pause current longer job and run the shorter one first before going to finish the longer one
	
- provides a good service of a short job even when it arrives late
### C2. Scheduling System
##### Criteria
1. **Response Time** $\to$ time between request and response by the system
2. **Predictability** $\to$ variation in response time, less variation $\implies$ more predictable and vice versa
##### Periodic Scheduling
*Note:* preemptive scheduling algorithms are used to ensure good response time as scheduler needs to run periodically
- we use a timer interrupt that goes off periodically based on the hardware clock to "take over" the CPU
- the OS ensures that the timer interrupt **cannot be intercepted by other programs**
##### Terminology
> The **time quantum** is the fixed time slice given to each process in a preemptive scheduling algorithm (like Round Robin).
- It is *typically constant* for all processes in a system (but could be variable in some systems)
- It is usually chosen as a *multiple of the system clock interval* (interval of timer interrupt)
- It is *not arbitrarily large* — while it can be tuned, very *large* time quantums reduce preemption and essentially turn Round Robin into FCFS
- Very *small* time quantums **increase context switching overhead** $\implies$ more switching between processes are required as they are interrupted very frequently
	- context switch time is treated as pure overhead $\implies$ no useful work is done by the processor during these idle periods

> The **interval of timer interrupt** is typically $1$ms or $10$ms
- no of time blocks allocated $= \frac{\text{Time Quantum}}{\text{Interval of Timer Interrupt}}$
## D. Scheduling Algorithms
Good scheduling algorithms will need to effectively balance CPU allocation (time) and overhead time (i.e. time for context switching), to optimize turnaround time and throughput.

- Scheduling is done on processes, but also on threads in modern systems
### D1. Round Robin (RR)
- tasks are stored in a first in first out (FIFO) queue

- is a preemptive scheduling algorithm $\implies$ allows for process switching (preemptive FCFS), since interrupts are made available and supported by the processor

- a small unit of time $\textemdash$ time quantum or slice is *arbitrarily defined* for each process to run for

- the first item is dequeued and ran **until** any of the following is met:
	- **fixed time slice expires** ($\exists$ a timer which is physically embedded within the CPU circuitry itself) $\implies$ RR can only be implemented if the hardware supports having a timer and preemptive scheduling
	- the task **gives up** the **CPU voluntarily**
	- the task will **block** (due to I/O operations)

- tasks are then placed at the **tail of the FIFO / ready queue** to wait for subsequent turns 

- blocked tasks will be moved to another queue to wait for its request to be granted
	- when blocked tasks are ready, then they are placed at the end of the main FIFO queue again

- response time guarantee
	- given $n$ tasks and the time quantum $q$, the **waiting time** before the CPU gets the CPU again is calculated using the formula: $\lceil(n-1) \times q \rceil$ (bounded by this figure)
	- each process with receive $\frac{1}{n}$ of the CPU time, of at most $q$ time units (given time quantum $= q$)

- timer interrupt is required for the timer to check quantum expiry $t_0 \leq q$, where $t_0$ is the current time

- *choice* of time quantum duration $q$ is **important**
	- ==*larger quantum*== $\to$ better CPU utilization, but *longer waiting times* (most processes will enter an I/O burst before being preempted)
		- average *response time* will **increase**
		- average turnaround time can be improved if processes finish the next CPU burst in a single time quantum $\implies$ context switching only when necessary
		
	- ==*smaller quantum*== $\to$ larger overhead and worse CPU utilization, but shorter waiting times (more interruptions and is pure overhead)
		- don't want to have so many grey regions ($\uparrow \text{greys }$as $\text{time} \downarrow$)

	![rr-time-quantums](../assets/rr-time-quantums.png)
##### Advantages
- one of the simplest ways to **achieve concurrency**
- allows for a **fair distribution** of CPU time
##### Disadvantages
- **fails** to maintain **responsiveness** (may take a very long time to produce its first output)
	- waiting time will also increase, as $n \: \#$ tasks is being increased 
### D2. Priority-based Scheduling
- prioritize **more important processes** as compared with the less important ones in the queue $\implies$ don't treat all of them as equal (but need to select those that require higher priority carefully)
- assign a priority value to all the tasks and then select the one with the highest priority value (usually that of an integer value)
	- can be implemented using both a ready and a waiting queue
	
- can be *combined* together with Round Robin as well (but may require $O(n)$ search to determine the process with highest priority because of the single FIFO queue used) $\implies$ solution is to use MLFQ as below

- Main example: Shortest Job First Algorithm
##### Variants
1. **Preemptive version:** process w higher priority preempts those running processes with lower priority $\implies$ lower priority process which arrive later in the queue can cause the current execution progress to be dequeued
	- preempted processes **do not** go into the waiting / blocked state, but rather transition from $\text{Running} \xrightarrow[]{\text{ Interrupted }} \textbf{Ready}$ directly

2. **Non-preemptive version:** a late-coming high priority process waits for the next round of scheduling
##### Starvation
- Low priority process can **starve** because other higher priority ones can *hog the CPU*, which is worse comparatively in the preemptive than non-preemptive variant

- Resolve by **decreasing the priority of the current running process** after each quantum (to make it of a higher priority in the long run)
	- can also **use aging** which does the opposite, increasing the priority of processes waiting in the system for a long time, to *prevent starvation* from occurring

- Hard to control the exact amount of CPU allotted to processes using this priority system
##### Priority Inversion
- occurs when a lower priority task preempts a higher priority one, often due to I/O resource locking etc.
### D3. Multi-Level Feedback Queue (MLFQ)
##### Motivation $\textemdash$ Multi-Level Queue Scheduling
- having separated queues for each different priority. If we have priority levels of $\{0, 1, 2, \ldots, n\} \implies$ we will have $(n + 1)$ queues in total, with $n$ being the lowest priority
- processes in queues with higher priority will be dequeued first, followed by subsequent processes in the lower priority queues (i.e. queue no. 0's elements will get dequeued before queue no. 3)
- used to separate important processes (i.e. OS related) that are critical to the machine running smoothly from not so important ones (i.e. idle user application etc.)

	![multi-level-queue-scheduling](../assets/multi-level-queue-scheduling.png)
- different queue layers can utilize different scheduling algorithms
###### Disadvantage of Multi-level queueing
- processes are permanently "tagged" to a particular queue $\implies$ has inflexibility involved when want to change priority etc.
	- email client in the background most of the time versus when having the GUI of the email client displayed which should be responsive
##### Properties
- is an **adaptive algorithm** that **learns** the process behaviour automatically / dynamically
	- solves the issue posed in multi-level queueing (without feedback)
	
- minimizes both *response time* for I/O-bound processes and *turnaround time* for CPU-bound processes
	- sort of demotes the priority of CPU-bound processes

- possible to have features like using historical data to change the priority of various processes, i.e. exponential weighted average, $\text{Predicted}_{n + 1}$

	![MLFQ](../assets/MLFQ-ex.png)
##### Rules
1. $\text{Priority}(A) \gt \text{Priority}(B) \implies \text{run } A$
2. $\text{Priority}(A) = \text{Priority}(B) \implies \text{run } A \text{ and } B \text{ with round robin algo}$
##### Priority Settings
1. each new job has the highest priority (queue)

2. if a job fully *utilized its time slice* (single time quantum $q$), then its priority will be **reduced** (i.e. moved down to a lower priority queue)
	- lower priority queues have **larger time quantums** $\implies$ allows for lesser time spent on context switching
	- move those which have completed I/O tasks from lower to higher priority queues
	
3. if a job *gives up or blocks* **before** finishing the time slice, then its priority is **maintained**

	![mlq-vs-mlfq](../assets/mlq-vs-mlfq.png)
### D4. Lottery Scheduling
- providing the various lottery tickets for processes which need various system resources
- when scheduling decision is required $\implies$ randomly choose one amongst many tickets
	- the winning process is granted the resource in question

- trend: in the long run, process holding $X\%$ of the tickets can win $X\%$ of the lottery held
###### Properties
1. **Responsive:** new processes can participate in the next lottery
2. **Good Control:** process can be given $Y$ lottery tickets and distribute it to its children processes
	- more important child process can be given more ticket (for higher chance of being selected and to control the proportion of usage)
	- each resource can have its own set of tickets (different proportion of usage per resource, per task)