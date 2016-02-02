# tools
This repository contains the source code for a variety of useful tools that are used across projects at Aqueti. This code is developed in Linux, but eventually eventually be extended for cross-platform support. The current implementation generates a separate library for each class. The name of the library is lib<ClassName>.a or lib<ClassName>.so

## Classes
**Timer** - Class and functions for managing time. Provides functions for getting the system time in various forms as well as a class for tracking time.

**C11Thread** - Wrapper class for std::thread objects. 

**DataBuffer** - Templated class for managing arrays of memory

##Applications
**UnitTest** - Executable for performing a Unit Test on the code base



