cmake_minimum_required(VERSION 2.8)

PROJECT(AquetiTools)

#Set compiler/linker options
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -fexpensive-optimizations")

#add external include files
include_directories( include ../ATimer)
include_directories( include ../ABuffer)
include_directories( include ../AThread)

#specifies a list of source files to be compiled
set( BASE_SOURCES 
   BaseSocket.cpp
   SocketServer.cpp
)


#specifies the library to create. The name will be libbase_static.a and libbase.so in this case.
add_library( ASocket SHARED ${BASE_SOURCES})
add_library( ASocket_static ${BASE_SOURCES})

link_directories(${CMAKE_BINARY_DIR}/lib)

#install library files
install(TARGETS 
   ASocket
   ASocket_static
   LIBRARY DESTINATION lib
   ARCHIVE DESTINATION lib
   INCLUDES DESTINATION include
   PUBLIC_HEADER DESTINATION include
)

#install include files
install(FILES
   BaseSocket.h
   DESTINATION include
)
