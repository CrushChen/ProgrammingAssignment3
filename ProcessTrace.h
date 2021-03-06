/*
 * ProcessTrace - execute memory trace file in the following format:
 * 
 * Trace records contain multiple fields, separated by white space (blanks 
 * and tabs). Each line consists of a command field, followed by optional 
 * arguments to the command. The command field is case sensitive.
 * 
 * The trace file will contain the following record types. All numerical values 
 * (including counts) are hexadecimal (base 16), without a leading "0x". 
 * Any output should also use hexadecimal format for numeric data.
 * 
 * Memory Quota
 * quota pages
 * Sets the maximum number of pages that a trace (process) may use, not including
 * pages used for the page table itself. Changing the quota to fewer pages than are already
 * in use does not cause an error or cause any pages to be deallocated
 * 
 * Compare bytes
 * compare addr expected_values
 * Compares bytes starting at addr; expected_values is a list of byte values, 
 * separated by white space. If the actual values of bytes starting at addr 
 * don't match the expected_values, write an error message to standard error 
 * for each mismatch with the address, the expected value, and the actual value 
 * (all in hexadecimal).
 * 
 * Put Bytes
 * put addr values
 * Store values starting at addr; values is a list of byte values, separated 
 * by white space. Any destination page which is not Present (causing a page fault)
 * should be allocated, and the put operation should be resumed. If the process page 
 * quota is exceeded, the process should be terminated with an explanatory message in
 * the output. 
 * 
 * Fill Bytes
 * fill addr count value
 * Store count copies of value starting at addr. Any destination page which is not Present
 * (causing a page fault) should be allocated, and the fill operation should be resumed. If the 
 * process page quota is exceeded, the process should be terminated with an explanatory message 
 * in the output.
 * 
 * Copy Bytes
 * copy dest_addr src_addr count
 * Copy count bytes from src_addr to dest_addr. The source and destination 
 * ranges will not overlap. Any destination page which is not Present (causing a page fault) should be
 * allocated, and the copy operation should be resumed. If the process page quota is exceeded,
 * the process should be terminated with an explanatory message in the output.
 * 
 * Dump Bytes
 * dump addr count
 * 
 * Write a line with addr to standard output, followed on separate lines by 
 * count bytes starting at addr. Write 16 bytes per line, with a space between 
 * adjacent values. For example, to print the 24 bytes starting at 3fa700:
 * 
 * 3fa700
 *  00 12 f3 aa 00 00 00 a0 ff ff e7 37 21 08 6e 00
 *  55 a5 9a 9b 9c ba fa f0
 * 
 * Writable Status
 * writable vaddr size status
 * Change the writable status of size bytes of memory, starting at virtual 
 * address vaddr. The starting address, vaddr, and the byte count, size, must be 
 * exact multiples of the page size (0x1000). If status is 0, the Writable bit 
 * in the 2nd level page table should be cleared for all Present pages in the 
 * range, otherwise the Writable bit in the 2nd level page table should be set 
 * for all Present pages in the range. Any pages in the range which are not 
 * Present should be ignored. The 1st level page table should not be changed.
 * 
 * Comment
 * # comment text
 * The # character in the first column means the remainder of the line should 
 * be treated as a comment. The command should be echoed to output in the same 
 * way as other commands, but should otherwise be ignored.
 */

/* 
 * File:   ProcessTrace.h
 * Author: Mike Goss <mikegoss@cs.du.edu>
 * Modified By: Peter Gish
 */

#ifndef PROCESSTRACE_H
#define PROCESSTRACE_H

#include "PageFrameAllocator.h"

#include <MMU.h>

#include <fstream>
#include <string>
#include <vector>

class ProcessTrace {
public:
  /**
   * Constructor - open trace file, initialize processing
   * 
   * @param memory_ MMU to use for memory
   * @param file_name_ source of trace commands
   */
  ProcessTrace(mem::MMU &memory_,
               PageFrameAllocator &allocator,
               std::string file_name_, int id);
  
  /**
   * Destructor - close trace file, clean up processing
   */
  virtual ~ProcessTrace(void);

  // Other constructors, assignment
  ProcessTrace(const ProcessTrace &other) = delete;
  ProcessTrace(ProcessTrace &&other) = delete;
  ProcessTrace operator=(const ProcessTrace &other) = delete;
  ProcessTrace operator=(ProcessTrace &&other) = delete;
  
  /**
   * Execute - read and process commands from trace file
   * 
   */
  int Execute(int num_lines);
  int getID(){ return id_number; }
  int getLinesExecuted(){ return line_number; }
  
private:
  // Trace file
  std::string file_name;
  std::fstream trace;
  long line_number;
  int id_number;
  int allocated_pages;
  //Maximum number of pages the trace may use
  int QUOTA;

  // Memory contents
  mem::MMU &memory;
  
  // Virtual and physical mode PMCBs
  mem::PMCB vmem_pmcb;
  mem::PMCB pmem_pmcb;
  
  // Memory allocator
  PageFrameAllocator &allocator;
  
  
  
  /**
   * ParseCommand - parse a trace file command.
   *   Aborts program if invalid trace file.
   * 
   * @param line return the original command line
   * @param cmd return the command name
   * @param cmdArgs returns a vector of argument bytes
   * @return true if command parsed, false if end of file
   */
  bool ParseCommand(
      std::string &line, std::string &cmd, std::vector<uint32_t> &cmdArgs);
  
  /**
   * Command executors. Arguments are the same for each command.
   *   Form of the function is CmdX, where "X' is the command name, capitalized.
   * Put, Fill,and Copy all return a bool that indicates if the command completed
   * successfully. The condition is whether the process exceeded its quota, if so 
   * it returns false, otherwise true is returned.
   * @param line original text of command line
   * @param cmd command, converted to all lower case
   * @param cmdArgs arguments to command
   */
  void CmdQuota(const std::string &line,
                const std::string &cmd,
                const std::vector<uint32_t> &cmdArgs);
  void CmdCompare(const std::string &line, 
              const std::string &cmd, 
              const std::vector<uint32_t> &cmdArgs);
  bool CmdPut(const std::string &line, 
              const std::string &cmd, 
              const std::vector<uint32_t> &cmdArgs);
  bool CmdFill(const std::string &line, 
               const std::string &cmd, 
               const std::vector<uint32_t> &cmdArgs);
  bool CmdCopy(const std::string &line, 
               const std::string &cmd, 
               const std::vector<uint32_t> &cmdArgs);
  void CmdDump(const std::string &line, 
               const std::string &cmd, 
               const std::vector<uint32_t> &cmdArgs);
  void CmdWritable(const std::string &line, 
                   const std::string &cmd, 
                   const std::vector<uint32_t> &cmdArgs);
  
  /**
   * PrintAndClearException - print a memory exception and clear operation
   *   in PMCB.
   * 
   * @param type description of exception
   * @param e exception object
   */
  void PrintAndClearException(const std::string &type, 
                              mem::MemorySubsystemException e);
  
  /**
   * AllocateAndMapPage - allocate a new user page and add it to the page table
   * 
   * @param vaddr virtual address of page to be mapped
   */
  void AllocateAndMapPage(mem::Addr vaddr);
  
  /**
   * SetWritableStatus - set the writable status of the page
   * 
   * @param vaddr virtual address of page to modify
   * @param writable true to make writable, false to make read-only
   */
  void SetWritableStatus(mem::Addr vaddr, bool writable);
};

#endif /* PROCESSTRACE_H */

