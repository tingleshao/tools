cmake_minimum_required(VERSION 2.8)

PROJECT(AquetiTools)

#Set compiler/linker options
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -fexpensive-optimizations")

#add external include files
include_directories( include ../Timer)
include_directories( include ../DataBuffer)
include_directories( include ../C11Thread)

#specifies a list of source files to be compiled
set( BASE_SOURCES 
   BaseSocket.cpp
   SocketServer.cpp
)


#specifies the library to create. The name will be libbase_static.a and libbase.so in this case.
add_library( AquetiSocket SHARED ${BASE_SOURCES})
add_library( AquetiSocket_static ${BASE_SOURCES})

#target_link_libraries( AquetiSocket
#   Timer
#   DataBuffer
#   C11THread
#)

#install library files
install(TARGETS 
   AquetiSocket
   AquetiSocket_static
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
