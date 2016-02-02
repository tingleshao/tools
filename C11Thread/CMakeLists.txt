cmake_minimum_required(VERSION 2.8)

PROJECT(AquetiTools)

#Set compiler/linker options
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -fexpensive-optimizations")


#This tells cmake to install header files when make install is executed
include(GenerateExportHeader)

#specifies a list of source files to be compiled
set( BASE_SOURCES 
   C11Thread.cpp
)

#specifies the library to create. The name will be libbase_static.a and libbase.so in this case.
add_library( C11Thread_static ${BASE_SOURCES})
add_library( C11Thread SHARED ${BASE_SOURCES})

#install library files
install(TARGETS 
   C11Thread
   C11Thread_static
   LIBRARY DESTINATION lib
   ARCHIVE DESTINATION lib
   INCLUDES DESTINATION include
   PUBLIC_HEADER DESTINATION include
)

install(FILES
   C11Thread.h
   DESTINATION include
)

