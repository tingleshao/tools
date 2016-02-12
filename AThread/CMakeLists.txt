cmake_minimum_required(VERSION 2.8)

PROJECT(AquetiTools)

#Set compiler/linker options
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -fexpensive-optimizations")

#specifies a list of source files to be compiled
set( BASE_SOURCES 
   AThread.cpp
)

#specifies the library to create. The name will be libbase_static.a and libbase.so in this case.
add_library( AThread_static ${BASE_SOURCES})
add_library( AThread SHARED ${BASE_SOURCES})

#install library files
install(TARGETS 
   AThread
   AThread_static
   LIBRARY DESTINATION lib
   ARCHIVE DESTINATION lib
   INCLUDES DESTINATION include
   PUBLIC_HEADER DESTINATION include
)

install(FILES
   AThread.h
   DESTINATION include
)

