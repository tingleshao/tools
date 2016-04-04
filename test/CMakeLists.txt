cmake_minimum_required(VERSION 2.8)

PROJECT(ATL)

add_definitions(-std=c++11 -fPIC)     #Use C++11 

#If we use clang, set up parameters
if( NOT CMAKE_COMPILER_SET )
   option(USE_CLANG "Use clang for compiler instead of gcc" ON)
   if( USE_CLANG)
      set(CMAKE_C_COMPILER "/usr/bin/clang")
      set(CMAKE_CXX_COMPILER "/usr/bin/clang++")
      option(WITH_ASAN "Enable Address sanitizer" OFF)

      if( WITH_ASAN )
         set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-omit-frame-pointer -fsanitize=address -fno-optimize-sibling-calls -DVOGL_USE_STB_MALLOC=0")
         set(CMAKE_CXX_FLAGS_LIST "${CMAKE_CXX_FLAGS_LIST}  -fno-omit-frame-pointer -fsanitize=address -fno-optimize-sibling-calls -DVOGL_USE_STB_MALLOC=0")
         set(CMAKE_EXE_LINK_FLAGS_LIST "${CMAKE_EXE_LINK_FLAGS_LIST} -fsanitize=address -fno-omit-frame-pointer")
         set(CMAKE_SHARED_LINK_FLAGS_LIST "${CMAKE_SHARED_LINK_FLAGS_LIST} -fsanitize=address -fno-omit-frame-pointer")
      endif()
   endif()
   #This flag is set to indicate that the compiler has been specified. This is
   #necessary for using the compiler specified by a Superbuild
   set(CMAKE_COMPILER_SET ON)
endif( NOT CMAKE_COMPILER_SET)


#specify location of include directories within this project
include_directories( ${CMAKE_SOURCE_DIR}/ATimer)
include_directories( ${CMAKE_SOURCE_DIR}/AThread)
include_directories( ${CMAKE_SOURCE_DIR}/ABuffer)
include_directories( ${CMAKE_SOURCE_DIR}/ASocket)
include_directories( ${CMAKE_SOURCE_DIR}/ThreadSafeQueue)

#Find OS Threads Library
find_package(Threads REQUIRED)

add_executable( WriteTest
   WriteTest.cpp
)

target_link_libraries( WriteTest
   ${CMAKE_THREAD_LIBS_INIT}               #For threaded operation
   ATL_static
)


#Specify the output executable of this file
add_executable( ATLTest
   ATLTest.cpp
)
#assign a library to link to this file. This shows a static library
target_link_libraries( ATLTest
   ATL_static
   ${CMAKE_THREAD_LIBS_INIT}               #For threaded operation
)

#Specify the output executable of this file
add_executable( SampleServer
   SampleServer.cpp
)
#assign a library to link to this file. This shows a static library
target_link_libraries( SampleServer
#   ATimer
#   AThread
#   ABuffer
#   ASocket
   ATL
   ${CMAKE_THREAD_LIBS_INIT}               #For threaded operation
)

#Create the SocketTerminal
add_executable( SocketTerminal
   SocketTerminal.cpp
)

target_link_libraries( SocketTerminal
#   ATimer
#   AThread
#   ABuffer
#   ASocket
   ATL
   ${CMAKE_THREAD_LIBS_INIT}               #For threaded operation
)



#Specify where the targets are installed when a "make install" is executed
install(TARGETS
   WriteTest
   SampleServer
   SocketTerminal
   RUNTIME DESTINATION bin
)

