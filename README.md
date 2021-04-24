# myps-command
## Background
In this project you will write a simplified version of the ps command found on Linux/Unix based systems. The purpose of this command is to display the current processes on the system and some basic metadata including the process id number (PID) as well as the associated command (COMM/CMD).
## Objectives
The myps project will use
- dynamic memory allocation
- create/destroy design pattern
- file stream processing and file system navigation
- fundamental C language components: structs, arrays and pointers </br>

to produe code that has been well tested and is free of memory errors/warnings.

## Specifications
For this project we are particularly interested in the stat file located within each process directory. We will only be extracting the below 7 data points from the stat file and displaying it to the user with options provided to change the order that processes are displayed. For debugging/testing purposes, an option will also be added to specify an alternate directory to load process data from.

    - pid - The process id number of the process
    - ppid - The process id number of the parent process
    - comm - The filename of the executable
    - state - The state of the process (Running, Sleeping, etc)
    - utime - The amount of time that the process has been scheduled in user mode
    - stime - The amount of time that the process has been scheduled in kernel mode
    - num_threads - The number of threads used by by this process
    - 
**Command line option:**
```
Usage: ./myps [-d <path>] [-p] [-c] [-z] [-h]
        -d <path> Directory containing proc entries (default: /proc)
        -p        Display proc entries sorted by pid (default)
        -c        Display proc entries sorted by command lexicographically
        -z        Display ONLY proc entries in the zombie state 
        -h        Display this help message
 ```
 
## Output 
**Test Sort By PID (-p)**
```
./myps -d test_data/onyx_proc -p | head -5
    PID    PPID STATE UTIME STIME THREADS CMD                       STAT_FILE           
      1       0     S     1     3       1 (systemd)                 test_data/onyx_proc/1/stat
      2       0     S     0     0       1 (kthreadd)                test_data/onyx_proc/2/stat
      4       2     S     0     0       1 (kworker/0:0H)            test_data/onyx_proc/4/stat
      6       2     S     0     0       1 (ksoftirqd/0)             test_data/onyx_proc/6/stat 
```
**Test Sort By Command (-c)**
```
./myps -d test_data/onyx_proc -c | head -5
    PID    PPID STATE UTIME STIME THREADS CMD                       STAT_FILE           
   1623       1     S     0     0       3 (NetworkManager)          test_data/onyx_proc/1623/stat
  19089       1     S     0     0       3 (abrt-dbus)               test_data/onyx_proc/19089/stat
   1539       1     S     0     0       1 (abrt-watch-log)          test_data/onyx_proc/1539/stat
   1488       1     S     0     0       1 (abrtd)                   test_data/onyx_proc/1488/stat
```
**Test Show Zombies (-z)**
```
./myps -d test_data/onyx_proc -z | head -5
    PID    PPID STATE UTIME STIME THREADS CMD                       STAT_FILE           
   1368       2     Z     0     0       1 (kworker/37:2)            test_data/onyx_proc/1368/stat
   2159       2     Z     0     0       1 (nfsd)                    test_data/onyx_proc/2159/stat
   2224       2     Z     0     0       1 (nfsd)                    test_data/onyx_proc/2224/stat
  19068       2     Z     0     0       1 (kworker/32:0)            test_data/onyx_proc/19068/stat
```
**Check For Memory Errors**
```
make memtest-myps 
valgrind --tool=memcheck --leak-check=yes --show-reachable=yes ./myps > /dev/null
==218067== Memcheck, a memory error detector
==218067== Copyright (C) 2002-2017, and GNU GPL'd, by Julian Seward et al.
==218067== Using Valgrind-3.15.0 and LibVEX; rerun with -h for copyright info
==218067== Command: ./myps
==218067== 
==218067== 
==218067== HEAP SUMMARY:
==218067==     in use at exit: 0 bytes in 0 blocks
==218067==   total heap usage: 1,871 allocs, 1,871 frees, 547,332 bytes allocated
==218067== 
==218067== All heap blocks were freed -- no leaks are possible
==218067== 
==218067== For lists of detected and suppressed errors, rerun with: -s
==218067== ERROR SUMMARY: 0 errors from 0 contexts (suppressed: 0 from 0)
```
