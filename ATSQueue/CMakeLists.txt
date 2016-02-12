cmake_minimum_required(VERSION 2.8)

PROJECT(AquetiTools)

#Set compiler/linker options
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -pthread -fexpensive-optimizations")

#specifies a list of source files to be compiled
set( BASE_SOURCES 
   ATSQueueTest.cpp
)

add_executable(ATSQueue_test ${BASE_SOURCES})

install(FILES
   ATSQueue.tcc
   DESTINATION include
)
