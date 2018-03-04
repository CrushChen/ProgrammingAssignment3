# ProgrammingAssignment3

# Intro:
 - The purpose of this program is to execute memory reference traces 
 - Must implement a multiprogramming environment with several traces executing
   in round-robin fashion. Each trace will simulate a process with its own 
   separate memory space.
 - Memory allocation will be on-demand. No memory will be pre-allocated to a process. 
   Instead, memory page frames should be allocated and mapped as they are written to.
   Each process will have a quota specifying the max number of pages it may allocate.

# Memory Reference Traces:
 - There is no alloc command. Pages will be allocated automatically when they are 
   first written to.
 - The quota command is added. It specifies the maximum number of pages that a process
   may use (not including page table pages)

# Difference in Trace Files:
 - Memory Quota --> quota pages
    Sets the maximum number of pages that a trace (process) may use, not including pages
    used for the page table itself. Changing the quota to fewer pages than are already in use
    does not cause an error or cause any pages to be deallocated

 - Put Bytes --> put addr values
    Store values starting at addr; values is a list of byte values, separated by white space.
    Any destination page which is not Present (causing a page fault) should be allocated,
    and the put operation should be resumed. If the process page quota is exceeded, the process
    should be terminated with an explanatory message in the output

 - Fill Bytes --> fill addr count value
    Store count copies of value starting at addr. Any destination page which is not Present (causing
    a page fault) should be allocated, and the fill operation should be resumed. If the process
    page quota is exceeded, the process should be terminated with an explanatory message in the output

 - Copy Bytes --> copy dest_addr src_addr count
    Copy count bytes from src_addr to dest_addr. THe source and destination ranges will not overlap. Any
    destination page which is not Present (causing a page fault) should be allocated, and the copy operation 
    should be resumed. If the process page quota is exceeded, the process should be terminated
    with an explanatory message in the output.

--> Every other command is same as Project 2

# Multiprogramming:
    Your program should run multiple traces (processes) in a multiprogramming environment. The traces
    should run in simplified round-robin scheduling fashion. A time slice will be specified in the command
    line argument. The time slice will be an integer line count. Your round-robin scheduler should request each
    trace in turn to execute the number of lines specified by the time slice. The count includes comment lines.
    After all other traces have executed, then a trace will execute another time slice lines, and so on. Traces 
    which terminate should be removed from the scheduler queue. When a trace terminates, write a message to output
    with the last line number in the trace, the process number, and the message "TERMINATED".
    For example, if process 4 terminates at line 16, the message would be:
    16:4:TERMINATED

