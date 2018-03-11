/*
 * Scheduler class to implement Round-Robin
 * scheduling algorithm
 */

/*
 * Takes in references to the MMU and PageFrameAllocator as well as the time_slice (number
 * of lines each process executes while being run) and a vector of file_names. 
 * The constructor passes the file_names to ParseFiles method which creates instances of 
 * ProcessTraces for each file name. These are stored in a global vector.
 * When Execute is called, the processes are executed in a Round-Robin fashion with each
 * process executing TIME_SLICE number of lines. When a process terminates (either from exceeding
 * its page frame quota or reaching the end of the file) a termination line is printed
 * and the process is removed from the process queue (the global vector of processes)
 * 
 */

/* 
 * File:   Scheduler.h
 * Author: Peter Gish
 *
 * Created on February 10, 2018, 5:36 PM
 */


#ifndef SCHEDULER_H
#define SCHEDULER_H

#include "PageFrameAllocator.h"
#include "ProcessTrace.h"
#include <MMU.h>

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

    /**
     * Function to begin multiprogramming
     */
    void Execute();


private:
    int TIME_SLICE; //number of lines to process 
    int NUM_PROCESSES; //number of processes currently running
    // Memory contents
    mem::MMU &memory;
    // Memory allocator
    PageFrameAllocator &allocator;
    //vector of all processes
    std::vector<ProcessTrace*> processes;

    /**
     * Extracts information from input file 
     * -Name: file name
     * -ID: process id
     * 
     * Creates a new instance of ProcessTrace for every file name that is given
     * Stores ProcessTraces in global vector
     * @param file_name_
     */
    void ParseFiles(std::vector<std::string> &file_names_);

    /**
     * Sets the currentIndex to the next valid index of the process list
     * (wraps the list)
     * @param currentIndex
     */
    void getNextIndex(int& currentIndex);
};

#endif /* SCHEDULER_H */

