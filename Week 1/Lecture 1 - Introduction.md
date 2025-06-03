## A. Operating Systems Basic Concepts
> An **operating system** is a *program* that acts as an intermediary between a user and computer hardware
- hardware itself
- user (can be a person, or application programs)

- OS can exist on computers or laptops, smartphones and other hardware (i.e. game consoles and home appliances)
### A1. The first computers
- they had no Operating Systems (i.e. the user program **directly interfaces** with hardware)
	- re-programming is made by changing the physical configuration of hardware
##### Advantage:
- having minimal overhead
##### Disadvantage:
- programs written are not portable
- is an inefficient use of the computer

### A2. Mainframes
**Mainframes** have:
- *no* interactive interface
- accept programs in the forms of paper and magnetic tape, as well as punch cards
- only supports batch processing
- are very costly and are usually rented instead of owned (unlike computers today)
##### Properties
- uses a batch OS which executes user programs (i.e. jobs) **one at a time,** successively
- user jobs still **interact with the hardware directly** but with *additional information* from the OS
##### Improvements Required
- Simple Batch processing is inefficient $\implies$ CPU has idle time waiting for I/O operations to be completed
	- use of multiprogramming or time sharing to make full use of this idle time
##### Time-sharing
- allows many users to interact using terminals (i.e. `tty`)
- user job scheduling provides the illusion of concurrency
- has memory management features available as well
