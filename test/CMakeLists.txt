cmake_minimum_required(VERSION 2.8)

PROJECT(ATools)

#specify location of include directories within this project
include_directories( ${CMAKE_SOURCE_DIR}/ATimer)
include_directories( ${CMAKE_SOURCE_DIR}/AThread)
include_directories( ${CMAKE_SOURCE_DIR}/ABuffer)
include_directories( ${CMAKE_SOURCE_DIR}/ASocket)
include_directories( ${CMAKE_SOURCE_DIR}/ThreadSafeQueue)
SET(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} "-std=c++11 -pthread")

#Find OS Threads Library
find_package(Threads REQUIRED)

#Specify the output executable of this file
add_executable( AToolsTest
   AToolsTest.cpp
)
#assign a library to link to this file. This shows a static library
target_link_libraries( AToolsTest
#   ATimer_static
#   AThread_static
#   ABuffer_static
#   ThreadSafeQueue
#   ASocket_static
   ATools
   ${CMAKE_THREAD_LIBS_INIT}               #For threaded operation
)

#Specify the output executable of this file
add_executable( SampleServer
   SampleServer.cpp
)
#assign a library to link to this file. This shows a static library
target_link_libraries( SampleServer
#   ATimer_static
#   AThread_static
#   ABuffer_static
#   ASocket_static
   ATools
   ${CMAKE_THREAD_LIBS_INIT}               #For threaded operation
)

#Create the SocketTerminal
add_executable( SocketTerminal
   SocketTerminal.cpp
)

target_link_libraries( SocketTerminal
   ATools
   ${CMAKE_THREAD_LIBS_INIT}               #For threaded operation
)



#Specify where the targets are installed when a "make install" is executed
install(TARGETS
   AToolsTest
   SampleServer
   SocketTerminal
   RUNTIME DESTINATION bin
)

