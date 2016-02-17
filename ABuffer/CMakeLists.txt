cmake_minimum_required(VERSION 2.8)

PROJECT(AquetiTools)

#Set compiler/linker options
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -fexpensive-optimizations -pthread")

#Find OS Threads Library
find_package(Threads REQUIRED)



#This tells cmake to install header files when make install is executed
include(GenerateExportHeader)

#specifies a list of source files to be compiled
set( LIB_SOURCES 
   DataBuffer.cpp
#   ExtendedBuffer.cpp
#   TSArray.cpp
)

#specifies the library to create. The name will be libbase_static.a and libbase.so in this case.
add_library( ABuffer SHARED ${LIB_SOURCES})
add_library( ABuffer_static ${LIB_SOURCES})

#specifies an executable for testing
add_executable( bufferTest
   BufferTest.cpp
   TypeBuffer.cpp
   ExtendedBuffer.cpp
   TSArray.cpp
   TSMatrix.cpp
)

target_link_libraries( bufferTest
   ABuffer_static
   pthread
)




#install library files
install(TARGETS 
   bufferTest
   ABuffer
   ABuffer_static
   LIBRARY DESTINATION lib
   ARCHIVE DESTINATION lib
   INCLUDES DESTINATION include
   PUBLIC_HEADER DESTINATION include
   RUNTIME DESTINATION bin
)

#install include files
install(FILES
   DataBuffer.h
   ExtendedBuffer.tcc
   TypeBuffer.tcc
   TSArray.tcc
   TSMatrix.tcc
   DESTINATION include
)
