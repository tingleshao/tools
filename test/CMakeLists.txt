cmake_minimum_required(VERSION 2.8)

PROJECT(AquetiTools)

#specify location of include directories within this project
include_directories( ${CMAKE_SOURCE_DIR}/Timer)
include_directories( ${CMAKE_SOURCE_DIR}/C11Thread)
include_directories( ${CMAKE_SOURCE_DIR}/DataBuffer)
include_directories( ${CMAKE_SOURCE_DIR}/AquetiSocket)
include_directories( ${CMAKE_SOURCE_DIR}/ThreadSafeQueue)

#Specify the output executable of this file
add_executable( AquetiToolsTest
   AquetiToolsTest.cpp
)

#Find OS Threads Library
find_package(Threads REQUIRED)

#assign a library to link to this file. This shows a static library
target_link_libraries( AquetiToolsTest
   Timer_static
   C11Thread_static
   DataBuffer_static
   ThreadSafeQueue_static
   AquetiSocket_static
   AquetiTools
   ${CMAKE_THREAD_LIBS_INIT}               #For threaded operation
)

#Specify where the targets are installed when a "make install" is executed
install(TARGETS
   AquetiToolsTest
   RUNTIME DESTINATION bin
)

