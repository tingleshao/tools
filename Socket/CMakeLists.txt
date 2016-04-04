cmake_minimum_required(VERSION 2.8)

PROJECT(AquetiTools)

#add external include files
include_directories( include ../Timer)
include_directories( include ../Buffer)
include_directories( include ../Thread)

#specifies a list of source files to be compiled
set( BASE_SOURCES 
   BaseSocket.cpp
   SocketServer.cpp
   ../Timer/Timer.cpp
   ../Buffer/DataBuffer.cpp
   ../Thread/Thread.cpp
)


#specifies the library to create. The name will be libbase_static.a and libbase.so in this case.
add_library( Socket SHARED ${BASE_SOURCES})
add_library( Socket_static ${BASE_SOURCES})
#assign a library to link to this file. This shows a static library


link_directories(${CMAKE_BINARY_DIR}/lib)

#install library files
install(TARGETS 
   Socket
   Socket_static
   LIBRARY DESTINATION lib
   ARCHIVE DESTINATION lib
   INCLUDES DESTINATION include
   PUBLIC_HEADER DESTINATION include
)

#install include files
install(FILES
   BaseSocket.h
   SocketServer.h
   DESTINATION include
)
