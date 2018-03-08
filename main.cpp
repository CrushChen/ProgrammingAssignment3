/*
 * COMP3361 Winter 2018
 * Programming Assignment 2 Sample Solution
 */

/* 
 * File:   main.cpp
 * Author: Mike Goss <mikegoss@cs.du.edu>
 *
 * Created on October 28, 2017, 10:08 PM
 */

/*
 * The command line arguments of the program will specify the time slice and a list of
 * trace files to execute as "processes". The first command line argument will be an 
 * integer number specifying the time slice. Following that, the other arguments will each
 * be the name of a trace file (the same file may be used multiple times to start multiple processes
 * executing the same input).
 * 
 * The process number is the position of the trace file name in the list, where the first process is 1,
 * the second 2, etc/
 */

#include "PageFrameAllocator.h"
#include "ProcessTrace.h"
#include "Scheduler.h"

#include <MMU.h>

#include <cstdlib>
#include <iostream>
#include <iomanip>
#include <algorithm>

/*
 * 
 */
int main(int argc, char* argv[]) {
  // Use command line argument as file name
  /*
    if (argc != 2) {
    std::cerr << "usage: Lab2 trace_file\n";
    exit(1);
  }
   * */
    
  
  //create an instance of the MMU with 1024 page frames
  //(4MB of simulated physical memory)
  mem::MMU memory(1024); 
  
  //each process (instance of Execute Trace) will need to have
  //its own set of page frames allocated.
  PageFrameAllocator allocator(memory);
  std::vector<std::string> file_names;
  int time_slice = std::stoi(argv[1]);
  for(int i = 2; i < argc; ++i){
      file_names.push_back(argv[i]);
  }
  
  Scheduler* scheduler = new Scheduler(file_names, memory, allocator, time_slice);
  scheduler->Execute();
  
  return 0;
}
