/*
 * File:   Scheduler.cpp
 * Author: Peter Gish
 *
 * Created on February 10, 2018, 5:36 PM
 */

#include "Scheduler.h"

#include <iostream>
#include <iomanip>
#include <sstream>
#include <algorithm>

using std::cin;
using std::cout;
using std::cerr;
using std::getline;
using std::istringstream;
using std::string;
using std::vector;
using std::ifstream;

Scheduler::Scheduler(vector<std::string> &file_names_, mem::MMU &memory_,
               PageFrameAllocator &allocator_, int time_slice_)
: memory(memory_), allocator(allocator_), TIME_SLICE(time_slice_) {
    NUM_PROCESSES = file_names_.size();
    ParseFiles(file_names_);
}

Scheduler::~Scheduler() {
}

std::vector<Scheduler::Process> Scheduler::ParseFiles(vector<std::string> &file_names_) {
    int id = 0;
    for(auto s : file_names_){
        Process temp = new Process();
        temp.trace = new ProcessTrace(memory, allocator, s, id);
        temp.lines_executed = 0;
        processes.push_back(temp);
        ++id;
    }
}

void Scheduler::Execute() {
    //process objects are reset by the AverageTurnaroundTime function
    int index = 0;
    int lines;
    Process current_proc;
    while(!processes.empty()){
        current_proc = processes.at(index);
        lines = current_proc.trace->Execute(TIME_SLICE);
        if(lines != TIME_SLICE){
            std::cout << (current_proc.lines_executed + lines) 
                    << ":" << +current_proc.trace.getID() << ":TERMINATED" << std::endl;
            processes.erase(processes.begin()+index);
            --NUM_PROCESSES;
        } else {
            current_proc.lines_executed += TIME_SLICE;
            getNextIndex(index);
        }
    }
}


void Scheduler::getNextIndex(int& currentIndex) {
    if (currentIndex + 1 == NUM_PROCESSES) {
        currentIndex = 0;
    } else {
        ++currentIndex;
    }
}
