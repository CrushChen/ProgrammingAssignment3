/*
 * Scheduler class to implement Round-Robin and Shortest Process Next
 * scheduling algorithms 
 */

/*
 * -All numeric values in the input file are decimal integers
 * (Time unit doesn't matter)
 * -Input file contains 1 line per process
 * -Lines are sorted in increasing order of arrival time in the system
 * 
 * -Line format --> name arrival_time total_time block_interval
 *  name: a sequence of non-blank characters representing the name of the process
 *  arrival_time: the time at which the process arrives in the system
 *  total_time: the total amount of CPU time which will be used by the process
 *  block_interval: interval at which will block for I/O. When a process blocks,
 *                  it is unavailable to run for the time specified by block_duration
 *                  in the scheduler parameter file
 * 
 * OUTPUT: --> all output should be written to standard output
 * For each scheduling algorithm:
 * - A single line with the name of the scheduling algorithm (RR or SPN),
 *   followed by the block_duration (for both algorithms) and time_slice (for RR)
 *   as specified on the command line. Values should be separated by spaces.
 * - One line for each interval during which a process is running or the system is 
 *   idle. The line should consist of a single space, followed by the current simulation
 *   time (starting at 0), followed by the process name (or "<idle>" if no process is
 *   running), the length of the interval and a status code:
 *      -"B" for blocked
 *      -"S" for time slice ended
 *      -"T" if the process terminated
 *      -"I" for an idle interval
 *   The fields should be separated by the tab character, '\t'.
 * - After all jobs have terminated, write a line consisting of a single space, the 
 *   simulation time at which the last job terminated, a tab character, the string
 *   "<done>", another tab character, and the average turnaround time of all
 *   processes (floating point value)
 * 
 * Turnaround Time: termination time - arrival time
 */

/* 
 * File:   Scheduler.h
 * Author: Peter Gish
 *
 * Created on February 10, 2018, 5:36 PM
 */


#ifndef SCHEDULER_H
#define SCHEDULER_H

#include "ProcessTrace.h"

#include <queue>
#include <vector>
#include <string>
#include <fstream>

class Scheduler {
public:
    /**
     * Constructor - initialize processing
     */
    Scheduler(std::vector<std::string> &file_names_, mem::MMU &memory_,
               PageFrameAllocator &allocator_, int time_slice_);

    /**
     * Destructor - clean up processing
     */
    virtual ~Scheduler();

    /**
     * Rule of 5:
     * All other constructors/assignments are not needed
     */
    Scheduler(const Scheduler &other) = delete;
    Scheduler(Scheduler &&other) = delete;
    Scheduler operator=(const Scheduler &other) = delete;
    Scheduler operator=(Scheduler &&other) = delete;



private:

    /**
     * struct to hold all necessary information about a process
     */
    struct Process {
        ProcessTrace trace; //name of process
        int lines_executed; //number of lines the process has executed
   };
    
    int TIME_SLICE; //number of lines to process 
    int NUM_PROCESSES;
    // Memory contents
    mem::MMU &memory;
    // Memory allocator
    PageFrameAllocator &allocator;
    //vector of all processes
    std::vector<Process> processes;

    /**
     * Extracts information from input file (allocates data into Process structs)
     * -Name: file name
     * -ID: process id
     * Format:
     *  name arrival_time total_time block_interval
     * 
     * -All numeric values are decimal integers
     * -1 line per process (formatted as shown above)
     * @param file_name_
     */
    std::vector<Process> ParseFiles(std::string file_name_);
    
    
    /**
     * Function to begin multiprogramming 
     * @param processes
     */
    void Execute(std::vector<Scheduler::Process> processes);

    
    /**
     * Sets the currentIndex to the next valid index of the process list
     * (wraps the list)
     * @param currentIndex
     * @param numProcesses
     */
    void getNextIndex(int& currentIndex, int& numProcesses);
};

#endif /* SCHEDULER_H */

