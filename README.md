# tools
This repository contains the source code for a variety of useful tools that are used across projects at Aqueti. This code is developed in Linux, but eventually eventually be extended for cross-platform support. The current implementation generates a separate library for each class. The name of the library is lib<ClassName>.a or lib<ClassName>.so

## Classes
**Timer** - Class and functions for managing time. Provides functions for getting the system time in various forms as well as a class for tracking time.

**C11Thread** - Wrapper class for std::thread objects. 

**DataBuffer** - Templated class for managing arrays of memory

**ATSQueue** - Templated class for creating a queue/stack of data, used mainly for interprocess communication

##Applications
**UnitTest** - Executable for performing a Unit Test on the code base

##Known Issues
**valgrind and iostream** - Valgrind-3.11.0 generates "72,704 bytes in 1 blocks are still reachable in loss record 1 of 1" when iostream is included in a header. This is a valgrind error and does not represent a valid memory leak (https://bugzilla.redhat.com/show_bug.cgi?id=455644). This is not reflected in all versions of Valgrind

